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
#include <chrono>

#include "im2row.hpp"

using namespace tt;
using namespace tt::tt_metal;
using namespace std;

#define TILE_WIDTH 32 // bfloats
#define TILE_HEIGHT 32 // 
#define TILE_SIZE 2048 // bfloats*TILE_WIDTH*TILE_HEIGHT

// #define ITERATIONS 10000
// #define ROWS 64
// #define COLS 64


// I wanto to have all the SRAM available, maybe I could use also multiple CBs
#define SRAM_TILES 128


int matmul_ttker(vector<bfloat16>& input, 
    vector<bfloat16>& stencil, 
    vector<bfloat16>& output, 
    uint32_t n_tiles,
    uint32_t rows,
    uint32_t cols,
    uint32_t iterations,
    IDevice* device){
    
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
            .size = input.size() * sizeof(bfloat16), 
            .page_size = TILE_SIZE,  
            .buffer_type = tt_metal::BufferType::DRAM};

    tt_metal::InterleavedBufferConfig dram_stencil_config{
            .device = device, 
            .size = stencil.size() * sizeof(bfloat16), 
            .page_size = TILE_SIZE,  
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
    CircularBufferConfig cb_input_config(TILE_SIZE * SRAM_TILES, 
                                          {{cb_input_index, data_format}}
    );
    cb_input_config.set_page_size(cb_input_index, TILE_SIZE);
    CBHandle cb_input = tt_metal::CreateCircularBuffer(program, all_cores, cb_input_config);

    uint32_t cb_stencil_index = CBIndex::c_1; 
    CircularBufferConfig cb_stencil_config(TILE_SIZE * SRAM_TILES, 
                                           {{cb_stencil_index, data_format}}
    );
    cb_stencil_config.set_page_size(cb_stencil_index, TILE_SIZE);
    CBHandle cb_stencil = tt_metal::CreateCircularBuffer(program, all_cores, cb_stencil_config);
    

    uint32_t cb_output_index = CBIndex::c_16; 
    CircularBufferConfig cb_output_config(TILE_SIZE * SRAM_TILES, 
                                           {{cb_output_index, data_format}}
    );
    cb_output_config.set_page_size(cb_output_index, TILE_SIZE);
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
            //cout << core.x*8 + core.y << endl;
            tt_metal::SetRuntimeArgs( program, reader_kernel_id, core, {
                input_dram_buffer->address(), start_tile_idx, work_per_core1, input_dram_buffer->size(),
                stencil_dram_buffer->address(), core.x*8 + core.y, work_per_core1, stencil_dram_buffer->size()
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
                stencil_dram_buffer->address(), core.x*8 + core.y, work_per_core2, stencil_dram_buffer->size()
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
    
    cout << "Memcpy and compute launch..." << endl;

    //! This is the first memcpy, it should be done only one time at the beginnning
    //! This is a memcpy, so output doesn't have to copied

    vector<bfloat16> new_in(rows*cols);
    vector<bfloat16> new_in_pad((rows+2)*(cols+2), 0.0f);
    vector<bfloat16> new_in_i2r(rows*cols*TILE_WIDTH);

    input = tilize_nfaces(input, rows * cols, TILE_WIDTH);
    stencil = tilize_nfaces(stencil, TILE_HEIGHT, TILE_WIDTH);

    EnqueueWriteBuffer(cq, input_dram_buffer, input.data(), true);   
    EnqueueWriteBuffer(cq, stencil_dram_buffer, stencil.data(), true);
    
    double elapsed_cpu = 0.0;
    double elapsed_memcpy = 0.0;
    double elapsed_wormhole = 0.0;
    std::chrono::duration<double, std::milli> elapsed;
    std::chrono::_V2::system_clock::time_point start_total, end_total, start_wormhole, end_wormhole, start_memcpy, end_memcpy, start_cpu, end_cpu;

    start_total = std::chrono::high_resolution_clock::now();

    for(i = 0; i<iterations; i++){

        start_wormhole = std::chrono::high_resolution_clock::now();
        EnqueueProgram(cq, program, true);
        end_wormhole = std::chrono::high_resolution_clock::now();
        elapsed = end_wormhole - start_wormhole;
        elapsed_wormhole += elapsed.count();

        start_memcpy = std::chrono::high_resolution_clock::now();
        EnqueueReadBuffer(cq, output_dram_buffer, output.data(), true);
        end_memcpy = std::chrono::high_resolution_clock::now();
        elapsed = end_memcpy - start_memcpy;
        elapsed_memcpy += elapsed.count();

        if (i != iterations-1){
            start_cpu = std::chrono::high_resolution_clock::now();
            output = untilize_nfaces(output, rows*cols, TILE_WIDTH);
            vec2stencil_5p(output, new_in, TILE_HEIGHT, n_tiles);
            new_in[(rows/2)*cols + cols/2] = 100.0f;
            pad_with_zeros(new_in, new_in_pad, rows, cols, 1);
            stencil2vec_5p(new_in_pad, new_in_i2r, (rows+2), (cols+2));
            new_in_i2r = tilize_nfaces(new_in_i2r, rows*cols, TILE_WIDTH);
            end_cpu = std::chrono::high_resolution_clock::now();
            elapsed = end_cpu - start_cpu;
            elapsed_cpu += elapsed.count();

            start_memcpy = std::chrono::high_resolution_clock::now();
            EnqueueWriteBuffer(cq, input_dram_buffer, new_in_i2r.data(), true);  
            end_memcpy = std::chrono::high_resolution_clock::now();
            elapsed = end_memcpy - start_memcpy;
            elapsed_memcpy += elapsed.count();

        } else {
            start_cpu = std::chrono::high_resolution_clock::now();
            output = untilize_nfaces(output, rows*cols, TILE_WIDTH);
            end_cpu = std::chrono::high_resolution_clock::now();
            elapsed = end_cpu - start_cpu;
            elapsed_cpu += elapsed.count();
        }
    }

    end_total = std::chrono::high_resolution_clock::now();
    elapsed = end_total - start_total;
    cout << "-TOTAL- " << elapsed.count() << " ms" << endl;
    cout << "-CPU- " << elapsed_cpu << " ms" << endl;
    cout << "-MEMCPY- " << elapsed_memcpy << " ms" << endl;
    cout << "-WORMHOLE- " << elapsed_wormhole << " ms" << endl;

    Finish(cq);

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

    uint32_t iterations;
    uint32_t rows;
    uint32_t cols;

    if (argc >= 4) {
        iterations = atoi(argv[1]);
        rows = atoi(argv[2]);
        cols = atoi(argv[3]);
        cout << "Using arguments: ITERATIONS=" << iterations << ", ROWS=" << rows << ", COLS=" << cols << endl;
    } else {
        cout << "Usage: " << argv[0] << " <iterations> <rows> <cols>" << endl;
        return -1;
    }


    //! To define by the input 
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


    if (i2r_buffer_size < TILE_SIZE){
        cerr << "Error: problem size must be at least " << TILE_SIZE << " elements." << endl;
        return -1;
    }

    //* ----------
    //* INPUT ALLOCATION
    //* ----------

    vector<bfloat16> input_vec(rows * cols);
    for(i = 0; i<rows*cols; i++){
        input_vec[i] = bfloat16(0.0f);
    }
    input_vec[(rows/2)*cols + cols/2] = 100.0f;

    //* ----------
    //* PADDING
    //* ----------

    // Pad the input, and the output but it's not necessary
    vector<bfloat16> input_vec_pad(rows_pad * cols_pad, 0.0f);
    vector<bfloat16> output_vec_pad(rows_pad * cols_pad, 0.0f);
    pad_with_zeros(input_vec, input_vec_pad, rows, cols, 1);

    golden_stencil(input_vec_pad, output_vec_pad, rows_pad, cols_pad, iterations);


    //* ----------
    //* im2row CONVERTION
    //* ----------

    vector<bfloat16> input_vec_i2r(rows_i2r * cols_i2r, 0.0f);
    stencil2vec_5p(input_vec_pad, input_vec_i2r, rows_pad, cols_pad);

    //* ----------
    //* ALIGNEMENT
    //* ----------

    dram_buffer_size = align_vector_size(input_vec_i2r, i2r_buffer_size, TILE_SIZE);
    diff_dram = (dram_buffer_size - i2r_buffer_size) / sizeof(bfloat16);

    num_tiles = dram_buffer_size / TILE_SIZE;
    stencil_tiles = num_tiles;

    cout << "Problem shape: " << rows << "x" << cols << endl;
    cout << "Stencil order: " << stencil_order << endl;
    cout << "Padded shape: " << rows_pad << "x" << cols_pad << endl;
    cout << "i2r shape: " << rows_i2r << "x" << cols_i2r  << endl;
    cout << "DRAM buffer size (bytes): " << i2r_buffer_size << endl;
    cout << "Number of tiles: " << num_tiles << endl;
    cout << "Number of stencil tiles: " << stencil_tiles << endl;


    //! KERNEL AREA

    int device_id = 0;
    IDevice* device = CreateDevice(device_id);
    auto [num_cores, all_cores, core_group_1, core_group_2, work_per_core1, work_per_core2] = split_work_to_cores(device->compute_with_storage_grid_size(), num_tiles);
    vector<bfloat16> output_vec(dram_buffer_size/sizeof(bfloat16), 0.0f);
    vector<bfloat16> stencil_vec_i2r(TILE_WIDTH * TILE_HEIGHT * num_cores, 0.0f);
    initialize_laplace_5p(stencil_vec_i2r.data(), TILE_HEIGHT, TILE_WIDTH, num_cores);

    matmul_ttker(input_vec_i2r, stencil_vec_i2r, output_vec, num_tiles, rows, cols, iterations, device);

    CloseDevice(device);

    //! KERNEL AREA

    cout << "Output: " << endl;
    vec2stencil_5p(output_vec, input_vec, TILE_HEIGHT, num_tiles);
    input_vec[(rows/2)*cols + cols/2] = 100.0f;
    printMat(input_vec, rows, cols);

    return 0;
}
