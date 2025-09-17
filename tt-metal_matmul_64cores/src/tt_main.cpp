// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <tt-metalium/host_api.hpp>
#include <tt-metalium/device.hpp>
#include <tt-metalium/command_queue.hpp>
#include <tt-metalium/constants.hpp>
#include <tt-metalium/util.hpp>
#include <tt-metalium/bfloat16.hpp>
#include <tt-metalium/command_queue.hpp>
#include <tt-metalium/work_split.hpp>
#include <tt-metalium/tilize_utils.hpp>
#include <tt-metalium/tensor_accessor_args.hpp>

#include <unistd.h>

#include "im2row.hpp"

using namespace tt;
using namespace tt::tt_metal;
using namespace std;

#define TILE_WIDTH 32 // bfloats
#define TILE_HEIGHT 32 // bfloats
#define ROWS 64
#define COLS 64


// I wanto to have all the SRAM available, maybe I could use also multiple CBs
#define SRAM_TILES 128


int matmul_ttker(vector<bfloat16>& input, vector<bfloat16>& stencil, vector<bfloat16>& output, uint32_t n_tiles, uint32_t stencil_n_tiles, uint32_t tile_size, IDevice* device){
    
    //* ---------------------------------------------------------
    //* HOST INITIALIZATION
    //* ---------------------------------------------------------

    int i, j;
    
    // Create the device and the program
    CommandQueue& cq = device->command_queue(); // Take the command_queue from the device created
    Program program = CreateProgram();

    DataFormat data_format = DataFormat::Float16_b;
    MathFidelity math_fidelity = MathFidelity::HiFi4;
    const auto core_grid = device->compute_with_storage_grid_size();

    auto [num_cores, all_cores, core_group_1, core_group_2, work_per_core1, work_per_core2] = split_work_to_cores(core_grid, n_tiles);

    cout << " Number of tensixes: " << num_cores << endl;
    cout << " Tensixes group 1 has " << work_per_core1 << " tiles" << endl;
    cout << " Tensixes group 2 has " << work_per_core2 << " tiles" << endl;

    //? If you have 64 cores, you at least 64 tiles so at least 32x64: if you have less you need to reduce cores
    //? I need to understand if it is better to have 1 tile per 64 core or less cores more tiles per core [MEASURE]
 
    // ---------------------------------------------------------
    // DRAM BUFFER CREATION: OFFCHIP GDDR6 MEMORY 12GB
    // ---------------------------------------------------------
    
    cout << "Creating DRAM buffers..." << endl;

    // Both input and output have the same configuration, in this case I have chosen Interleaved instead of Shreaded

    // device, size, page_size, buffer_type
    tt_metal::InterleavedBufferConfig dram_inout_config{
            .device = device, 
            .size = n_tiles * tile_size, 
            .page_size = tile_size,  
            .buffer_type = tt_metal::BufferType::DRAM};

    tt_metal::InterleavedBufferConfig dram_stencil_config{
            .device = device, 
            .size = stencil_n_tiles * tile_size, 
            .page_size = tile_size,  
            .buffer_type = tt_metal::BufferType::DRAM};

    std::shared_ptr<tt::tt_metal::Buffer> input_dram_buffer = CreateBuffer(dram_inout_config);
    std::shared_ptr<tt::tt_metal::Buffer> output_dram_buffer = CreateBuffer(dram_inout_config);
    std::shared_ptr<tt::tt_metal::Buffer> stencil_dram_buffer = CreateBuffer(dram_stencil_config);

    // ! In this case you should declare the starting address and the size of the region in the shared cache
    // ! Then Is necessary a method to avoid contention, but for stencil could be avoided by the nature of the problem
    

    // ---------------------------------------------------------
    // SRAM BUFFER CREATION: TOTAL SHARED BETWEEN CORES 108MB HIGH-SPEED REGISTERS 
    // ---------------------------------------------------------

    cout << "Creating SRAM buffers..." << endl;

    uint32_t cb_input_index = CBIndex::c_0;
    CircularBufferConfig cb_input_config(tile_size * SRAM_TILES, 
                                          {{cb_input_index, data_format}}
    );
    cb_input_config.set_page_size(cb_input_index, tile_size);
    CBHandle cb_input = tt_metal::CreateCircularBuffer(program, all_cores, cb_input_config);

    uint32_t cb_stencil_index = CBIndex::c_1; 
    CircularBufferConfig cb_stencil_config(tile_size * SRAM_TILES, 
                                           {{cb_stencil_index, data_format}}
    );
    cb_stencil_config.set_page_size(cb_stencil_index, tile_size);
    CBHandle cb_stencil = tt_metal::CreateCircularBuffer(program, all_cores, cb_stencil_config);
    

    uint32_t cb_output_index = CBIndex::c_16; 
    CircularBufferConfig cb_output_config(tile_size * SRAM_TILES, 
                                           {{cb_output_index, data_format}}
    );
    cb_output_config.set_page_size(cb_output_index, tile_size);
    CBHandle cb_output = tt_metal::CreateCircularBuffer(program, all_cores, cb_output_config);

    // ---------------------------------------------------------
    // KERNELS CREATION: We need a reader, writer and then a compute
    // ---------------------------------------------------------

    cout << "Creating kernels..." << endl;


    std::vector<uint32_t> reader_compile_time_args;
    TensorAccessorArgs(*input_dram_buffer).append_to(reader_compile_time_args);
    TensorAccessorArgs(*stencil_dram_buffer).append_to(reader_compile_time_args); 

    auto reader_kernel_id = tt_metal::CreateKernel( program, "/home/lpiarulli_tt/stencil-wormhole/tt-metal_matmul_64cores/src/kernels/dataflow/reader_input.cpp",
        all_cores, tt_metal::DataMovementConfig{ .processor = DataMovementProcessor::RISCV_1, 
                                            .noc = NOC::RISCV_1_default,
                                            .compile_args = reader_compile_time_args}
    );

    std::vector<uint32_t> writer_compile_time_args;
    TensorAccessorArgs(*output_dram_buffer).append_to(writer_compile_time_args); 
    
    auto writer_kernel_id = tt_metal::CreateKernel( program, "/home/lpiarulli_tt/stencil-wormhole/tt-metal_matmul_64cores/src/kernels/dataflow/writer_output.cpp",
        all_cores, tt_metal::DataMovementConfig{ .processor = DataMovementProcessor::RISCV_0,
                                            .noc = NOC::RISCV_0_default,
                                            .compile_args = writer_compile_time_args}
    );
    

    std::vector<uint32_t> compute_args = {};
    KernelHandle stencil_kernel_id = tt_metal::CreateKernel( 
        program, 
        "/home/lpiarulli_tt/stencil-wormhole/tt-metal_matmul_64cores/src/kernels/compute/stencil.cpp",
        all_cores, 
        tt_metal::ComputeConfig { 
            .math_fidelity = math_fidelity,
            .fp32_dest_acc_en = false, 
            .math_approx_mode = false,
            .compile_args = compute_args,
        }
    );

    // Compute config has a lot of arguments, but I don't know them now:
    // - .math_approx_modes
    // - .fp32_dest_acc_en
    // - .defines
    // If think many others

    // ---------------------------------------------------------
    // SETUP RUNTIME ARGS: Set the runtime arguments for the kernels
    // ---------------------------------------------------------

    cout << "Setting up runtime arguments..." << endl;

    int start_tile_idx = 0;
    for(const auto& core_range : core_group_1.ranges()){
        for(const auto& core : core_range) {
            tt_metal::SetRuntimeArgs( program, reader_kernel_id, core, {
                input_dram_buffer->address(), start_tile_idx, work_per_core1, input_dram_buffer->size(),
                stencil_dram_buffer->address(), start_tile_idx, work_per_core1, stencil_dram_buffer->size()
            });

            tt_metal::SetRuntimeArgs(program, writer_kernel_id, core, {
                output_dram_buffer->address(), start_tile_idx, work_per_core1, output_dram_buffer->size()
            });

            tt_metal::SetRuntimeArgs(program, stencil_kernel_id, core, {
                work_per_core1
            });
            
            start_tile_idx += work_per_core1;
        }
    }
    for(const auto& core_range : core_group_2.ranges()){
        for(const auto& core : core_range) {
            tt_metal::SetRuntimeArgs( program, reader_kernel_id, core, {
                input_dram_buffer->address(), start_tile_idx, work_per_core2, input_dram_buffer->size(),
                stencil_dram_buffer->address(), start_tile_idx, work_per_core2, stencil_dram_buffer->size()
            });

            tt_metal::SetRuntimeArgs(program, writer_kernel_id, core, {
                output_dram_buffer->address(), start_tile_idx, work_per_core2, output_dram_buffer->size()
            });

            tt_metal::SetRuntimeArgs(program, stencil_kernel_id, core, {
                work_per_core2
            });
            
            start_tile_idx += work_per_core2;
        }
    }
    

    // ---------------------------------------------------------
    // ENQUEUE WRITE BUFFERS: Write data on the allocated buffers
    // ---------------------------------------------------------
    
    cout << "Enqueueing write buffers..." << endl;

    //! This is the first memcpy, it should be done only one time at the beginnning
    //! This is a memcpy, so output doesn't have to copied
    EnqueueWriteBuffer(cq, input_dram_buffer, input.data(), false);   
    EnqueueWriteBuffer(cq, stencil_dram_buffer, stencil.data(), false);       
    
    // ---------------------------------------------------------
    // LAUNCH AND WAIT TERMINATION: Set the runtime arguments for the kernels
    // ---------------------------------------------------------

    cout << "Enqueueing kernels..." << endl;	

    //! The final aim is to avoid memory overhead so only the EnqueueWriteBuffer
    int times = 10;
    for(i = 0; i<times; i++){

        cout << "times: " << i << endl;

        EnqueueProgram(cq, program, false);
        EnqueueReadBuffer(cq, output_dram_buffer, output.data(), true); // Read the result from the device, works also as a barrier i think

        if (i != times-1){

            output = untilize_nfaces(output, ROWS*COLS, TILE_WIDTH);
            vector<bfloat16> new_in(ROWS*COLS);
            vec2stencil_5p(output, new_in, TILE_HEIGHT, n_tiles);

            vector<bfloat16> new_in_pad((ROWS+2)*(COLS+2));
            pad_with_zeros(new_in, new_in_pad, ROWS, COLS, 1);

            vector<bfloat16> new_in_i2r(ROWS*ROWS*TILE_WIDTH);
            stencil2vec_5p(new_in_pad, new_in_i2r, (ROWS+2), (COLS+2));

            new_in_i2r = tilize_nfaces(new_in_i2r, ROWS*COLS, TILE_WIDTH);
            EnqueueWriteBuffer(cq, input_dram_buffer, new_in_i2r.data(), false);  
        }  
    }

    Finish(cq);
    cout << "Core {0, 0} on Device 0 completed the task!" << endl;

    return 0;
}


int main(int argc, char** argv) {


    // ---------------------------------------------------------
    // ---------------------------------------------------------
    // PROBLEM DATA - We are solving for 2D matrices
    // 
    // E.g.
    // - Stencil of order 1: box of 9 elements, star of 5. Always 1 of padding.
    // - Stencil of order 2: box of 25 elements, start of 9 elements, star of 13 elements. Always 2 of padding. 
    // ..
    // ---------------------------------------------------------
    // ---------------------------------------------------------

    const size_t single_tile_size = TILE_WIDTH * TILE_HEIGHT * sizeof(bfloat16); // bytes

    //! To define by the input 
    constexpr uint32_t rows = ROWS, cols = COLS;
    constexpr uint32_t stencil_order = 1;
    //! To define by the input

    //! direct from the input
    size_t buffer_size = rows * cols * sizeof(bfloat16);
    //* stencil
    const uint32_t stencil_rows = TILE_HEIGHT;
    const uint32_t stencil_cols = TILE_WIDTH; 
    //* padding
    const uint32_t rows_pad = rows + stencil_order * 2; 
    const uint32_t cols_pad = cols + stencil_order * 2;
    const uint32_t padding_elements = 2*(rows_pad+cols_pad - 2);
    const size_t pad_buffer_size = rows_pad * cols_pad * sizeof(bfloat16);
    //* i2r
    const uint32_t rows_i2r = rows * cols;
    const uint32_t cols_i2r = (stencil_order*4)+1 + 27; //! plus 28 is for the padding to become 32
    const size_t i2r_buffer_size = rows_i2r * cols_i2r * sizeof(bfloat16); 
    //! direct from the input

    uint32_t num_tiles, stencil_tiles, dram_buffer_size, diff_dram;

    int ret, i, j;


    if (i2r_buffer_size < single_tile_size){
        cerr << "Error: problem size must be at least " << single_tile_size << " elements." << endl;
        return -1;
    }

    //* ----------
    //* INPUT ALLOCATION
    //* ----------

    vector<bfloat16> input_vec(rows * cols);
    for(i = 0; i<rows * cols; i++){
        input_vec[i] = bfloat16(0.0f);
    }
    input_vec[(rows/2)*cols + cols/2] = 1.0f;

    //* ----------
    //* PADDING
    //* ----------

    // Pad the input, and the output but it's not necessary
    vector<bfloat16> input_vec_pad(rows_pad * cols_pad, 0.0f);
    pad_with_zeros(input_vec, input_vec_pad, rows, cols, 1);

    cout << 1 << endl;

    //* ----------
    //* im2row CONVERTION
    //* ----------

    vector<bfloat16> input_vec_i2r(rows_i2r * cols_i2r);
    stencil2vec_5p(input_vec_pad, input_vec_i2r, rows_pad, cols_pad);

    //* ----------
    //* ALIGNEMENT
    //* ----------

    dram_buffer_size = align_vector_size(input_vec_i2r, i2r_buffer_size, single_tile_size);
    diff_dram = (dram_buffer_size - i2r_buffer_size) / sizeof(bfloat16);

    num_tiles = dram_buffer_size / single_tile_size;
    stencil_tiles = num_tiles;

    //! Stencil creation, output creation
    vector<bfloat16> output_vec(dram_buffer_size/sizeof(bfloat16), 0.0f);

    vector<bfloat16> stencil_vec_i2r(stencil_rows * stencil_cols * stencil_tiles, 1.0f);
    for(i = 2; i<TILE_HEIGHT * stencil_tiles; i+=32){
        for (j = 0; j< TILE_WIDTH; j++){
            stencil_vec_i2r[i*TILE_WIDTH + j] = bfloat16(0.25f);
        }
    }
    //! Stencil creation, Output creation


    cout << "Problem shape: " << rows << "x" << cols << endl;
    cout << "Stencil order: " << stencil_order << endl;
    cout << "Padded shape: " << rows_pad << "x" << cols_pad << endl;
    cout << "i2r shape: " << rows_i2r << "x" << cols_i2r  << endl;
    cout << "DRAM buffer size (bytes): " << i2r_buffer_size << endl;
    cout << "Number of tiles: " << num_tiles << endl;
    cout << "Number of stencil tiles: " << stencil_tiles << endl;

    cout << "Input: " << endl;
    printMat(input_vec_pad, rows_pad, cols_pad);
    cout << endl;

    // cout << "Input I2R: " << endl;
    // printMat(input_vec_i2r, rows_i2r, cols_i2r);
    // cout << endl;

    // cout << "Stencil I2R:" << endl;
    // printMat(stencil_vec_i2r, TILE_HEIGHT*num_tiles, TILE_WIDTH);
    // cout << endl;


    //! KERNEL AREA
    int device_id = 0;
    IDevice* device = CreateDevice(device_id);

    input_vec_i2r = tilize_nfaces(input_vec_i2r, rows_i2r, cols_i2r);
    stencil_vec_i2r = tilize_nfaces(stencil_vec_i2r, TILE_HEIGHT*2, TILE_WIDTH);

    matmul_ttker(input_vec_i2r, stencil_vec_i2r, output_vec, num_tiles,stencil_tiles, single_tile_size, device);
    
    output_vec = untilize_nfaces(output_vec, rows_i2r, cols_i2r);

    CloseDevice(device);
    //! KERNEL AREA

    cout << "Output: " << endl;
    vec2stencil_5p(output_vec, input_vec, TILE_HEIGHT, num_tiles);
    printMat(input_vec, rows, cols);

    return 0;
}
