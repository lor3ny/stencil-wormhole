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

#include <tt-metalium/tilize_utils.hpp>
#include <tt-metalium/tensor_accessor_args.hpp>

#include <unistd.h>

#include "im2row.hpp"

using namespace tt;
using namespace tt::tt_metal;
using namespace std;

#define TILE_WIDTH 8 // bfloats
#define TILE_HEIGHT 8 // bfloats

//export TT_METAL_DPRINT_CORES="(0,0)-(7,7)"


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
    constexpr uint32_t rows = 4, cols = 4;
    constexpr uint32_t stencil_order = 1;
    //! To define by the input

    //! direct from the input
    size_t buffer_size = rows * cols * sizeof(bfloat16);
    //* padding
    const uint32_t rows_pad = rows + stencil_order * 2; 
    const uint32_t cols_pad = cols + stencil_order * 2;
    const uint32_t padding_elements = 2*(rows_pad+cols_pad - 2);
    const size_t pad_buffer_size = rows_pad * cols_pad * sizeof(bfloat16);
    //* i2r
    const uint32_t rows_i2r = rows * cols;
    const uint32_t cols_i2r = (stencil_order*4)+1 + 3; //! plus 3 is for the padding to become 8 
    const size_t i2r_buffer_count = rows_i2r * cols_i2r;
    const size_t i2r_buffer_size = i2r_buffer_count * sizeof(bfloat16); 
    //! direct from the input

    if (i2r_buffer_size < single_tile_size){
        cerr << "Error: problem size must be at least " << single_tile_size << " elements." << endl;
        return -1;
    }

    //! INPUT BUFFER INITIALIZATION
    //! STENCIL BUFFER INITIALIZTION
    uint32_t input_bfp16_count = buffer_size / sizeof(bfloat16);
    vector<bfloat16> input_vec(input_bfp16_count);
    for(int i = 0; i<input_bfp16_count; i++){
        input_vec[i] = bfloat16(1.0f); //bfloat16((float)i);
    }

    uint32_t stencil_buffer_size = TILE_WIDTH * TILE_HEIGHT * sizeof(bfloat16) * 2;
    uint32_t stencil_bfp16_count = stencil_buffer_size / sizeof(bfloat16);
    vector<bfloat16> stencil_vec_i2r(stencil_bfp16_count, 1.0f);

    for (int s_j = 0; s_j < TILE_WIDTH; s_j++){
        stencil_vec_i2r[2*TILE_WIDTH + s_j] = bfloat16(0.25f);
        stencil_vec_i2r[9*TILE_WIDTH + s_j] = bfloat16(0.25f);
    }
    cout << "Stencil:" << endl;
    printMat(stencil_vec_i2r, TILE_HEIGHT*2, TILE_WIDTH);
    //! INPUT BUFFER INITIALIZATION
    //! STENCIL BUFFER INITIALIZTION

    //* ----------
    //* PADDING
    //* ----------

    // Pad the input, and the output but it's not necessary
    input_vec = pad_with_zeros(input_vec, rows, cols, 1);

    cout << "Problem shape: " << rows << "x" << cols << endl;
    cout << "Stencil order: " << stencil_order << endl;
    cout << "Padded shape: " << rows_pad << "x" << cols_pad << endl;
    cout << "i2r shape: " << rows_i2r << "x" << cols_i2r  << endl;

    //* ----------
    //* im2row CONVERTION
    //* ----------

    vector<bfloat16> input_vec_i2r(i2r_buffer_count);
    im2row_5p(input_vec, input_vec_i2r, rows_pad, cols_pad);

    //* ----------
    //* ALIGNEMENT
    //* ----------

    cout << "DRAM buffer size (bytes): " << i2r_buffer_size << endl;

    uint32_t dram_buffer_size = align_vector_size(input_vec_i2r, i2r_buffer_size, single_tile_size);
    uint32_t diff_dram = (dram_buffer_size - i2r_buffer_size) / sizeof(bfloat16);

    std::cout << "Input I2R: " << std::endl;
    printMat(input_vec_i2r, rows_i2r, cols_i2r);

    // std::cout << "Input Padded im2row-ed Aligned: " << std::endl;
    // printMat(input_vec_i2r, rows_i2r+diff_dram/5, cols_i2r);

    //! OUTPUT Device handling
    vector<bfloat16> output_vec(dram_buffer_size/sizeof(bfloat16), 0.0f);
    //! THIS SECTION HAVE TO BE ADAPATED TO THE TT KERNEL
    //! Now works only for copy! 

    // ---------------------------------------------------------
    // ---------------------------------------------------------
    // HOST INITIALIZATION
    // ---------------------------------------------------------
    // ---------------------------------------------------------

    //! TILIZZAZIONE SEMBRA NECESSARIA, CAPIRE
    //?src0_vec = tilize_nfaces(src0_vec, M, K);
    //?src1_vec = tilize_nfaces(src1_vec, K, N);
    //! TILIZZAZIONE SEMBRA NECESSARIA, CAPIRE

    cout << "Initializing..." << endl;
    
    // Create the device and the program
    int device_id = 0;
    IDevice* device = CreateDevice(device_id);
    CommandQueue& cq = device->command_queue(); // Take the command_queue from the device created
    Program program = CreateProgram();

    constexpr CoreCoord core = {0, 0};  
    const uint32_t num_tiles = dram_buffer_size / single_tile_size; // Number of tiles 
    const uint32_t stencil_tiles = 2;

    DataFormat data_format = DataFormat::Float16_b;
    MathFidelity math_fidelity = MathFidelity::HiFi4;

    cout << "Number of tiles: " << num_tiles << endl;
    cout << "Number of stencil tiles: " << stencil_tiles << endl;

    // ---------------------------------------------------------
    // DRAM BUFFER CREATION: OFFCHIP GDDR6 MEMORY 12GB
    // ---------------------------------------------------------
    
    cout << "Creating DRAM buffers..." << endl;

    // Both input and output have the same configuration, in this case I have chosen Interleaved instead of Shreaded

    // device, size, page_size, buffer_type
    tt_metal::InterleavedBufferConfig dram_inout_config{
            .device = device, 
            .size = num_tiles * single_tile_size, 
            .page_size = single_tile_size,  
            .buffer_type = tt_metal::BufferType::DRAM};

    tt_metal::InterleavedBufferConfig dram_stencil_config{
            .device = device, 
            .size = stencil_tiles * single_tile_size, 
            .page_size = single_tile_size,  
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
    CircularBufferConfig cb_input_config(single_tile_size * num_tiles, 
                                          {{cb_input_index, data_format}}
    );
    cb_input_config.set_page_size(cb_input_index, single_tile_size);
    CBHandle cb_input = tt_metal::CreateCircularBuffer(program, core, cb_input_config);

    uint32_t cb_stencil_index = CBIndex::c_1; 
    CircularBufferConfig cb_stencil_config(single_tile_size * stencil_tiles, 
                                           {{cb_stencil_index, data_format}}
    );
    cb_stencil_config.set_page_size(cb_stencil_index, single_tile_size);
    CBHandle cb_stencil = tt_metal::CreateCircularBuffer(program, core, cb_stencil_config);
    

    uint32_t cb_output_index = CBIndex::c_16; 
    CircularBufferConfig cb_output_config(single_tile_size * num_tiles, 
                                           {{cb_output_index, data_format}}
    );
    cb_output_config.set_page_size(cb_output_index, single_tile_size);
    CBHandle cb_output = tt_metal::CreateCircularBuffer(program, core, cb_output_config);

    // ---------------------------------------------------------
    // KERNELS CREATION: We need a reader, writer and then a compute
    // ---------------------------------------------------------

    cout << "Creating kernels..." << endl;


    std::vector<uint32_t> reader_compile_time_args;
    TensorAccessorArgs(*input_dram_buffer).append_to(reader_compile_time_args);
    TensorAccessorArgs(*stencil_dram_buffer).append_to(reader_compile_time_args); 

    auto reader_kernel_id = tt_metal::CreateKernel( program, "/home/lpiarulli_tt/stencil-wormhole/tt-metal_stencil/src/kernels/dataflow/reader_input.cpp",
        core, tt_metal::DataMovementConfig{ .processor = DataMovementProcessor::RISCV_1, 
                                            .noc = NOC::RISCV_1_default,
                                            .compile_args = reader_compile_time_args}
    );

    std::vector<uint32_t> writer_compile_time_args;
    TensorAccessorArgs(*output_dram_buffer).append_to(writer_compile_time_args); 
    
    auto writer_kernel_id = tt_metal::CreateKernel( program, "/home/lpiarulli_tt/stencil-wormhole/tt-metal_stencil/src/kernels/dataflow/writer_output.cpp",
        core, tt_metal::DataMovementConfig{ .processor = DataMovementProcessor::RISCV_0,
                                            .noc = NOC::RISCV_0_default,
                                            .compile_args = writer_compile_time_args}
    );
    

    std::vector<uint32_t> compute_args = {};
    KernelHandle stencil_kernel_id = tt_metal::CreateKernel( 
        program, 
        "/home/lpiarulli_tt/stencil-wormhole/tt-metal_stencil/src/kernels/compute/stencil.cpp",
        core, 
        tt_metal::ComputeConfig{ 
            .math_fidelity = math_fidelity,
            //?.fp32_dest_acc_en = false, 
            //?.math_approx_mode = false,
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

    // QUI SETTO RUNTIME ARGS PER READER
    tt_metal::SetRuntimeArgs( program, reader_kernel_id, core,
            {input_dram_buffer->address(), num_tiles, input_dram_buffer->size(),
             stencil_dram_buffer->address(), stencil_tiles, stencil_dram_buffer->size()});
    // QUI SETTO RUNTIME ARGS PER WRITER
    tt_metal::SetRuntimeArgs(program, writer_kernel_id, core, 
            {output_dram_buffer->address(), num_tiles, output_dram_buffer->size()});
    // QUI SETTO RUNTIME ARGS PER COMPUTE
    tt_metal::SetRuntimeArgs(program, stencil_kernel_id, core, {num_tiles});
    

    // ---------------------------------------------------------
    // ENQUEUE WRITE BUFFERS: Write data on the allocated buffers
    // ---------------------------------------------------------
    
    cout << "Enqueueing write buffers..." << endl;

    //! This is the first memcpy, it should be done only one time at the beginnning
    //! This is a memcpy, so output doesn't have to copied
    EnqueueWriteBuffer(cq, input_dram_buffer, input_vec_i2r.data(), false);   
    EnqueueWriteBuffer(cq, stencil_dram_buffer, stencil_vec_i2r.data(), false);    
    EnqueueWriteBuffer(cq, output_dram_buffer, output_vec.data(), false);     
    
    // ---------------------------------------------------------
    // LAUNCH AND WAIT TERMINATION: Set the runtime arguments for the kernels
    // ---------------------------------------------------------

    cout << "Enqueueing kernels..." << endl;	

    //! The final aim is to avoid memory overhead so only the EnqueueWriteBuffer
    int times = 1;
    for(int i = 0; i<times; i++){

        EnqueueProgram(cq, program, false);
        EnqueueReadBuffer(cq, output_dram_buffer, output_vec.data(), true); // Read the result from the device, works also as a barrier i think
   
        //output_vec = pad_with_zeros(output_vec, rows, cols, 1);

        //! Convert the output in im2row format
        // vector<uint32_t> conv_out_vec;
        // im2row_5p(output_vec, conv_out_vec, 1);

        //! SE CI FOSSE LA UPM NON DOVREI FARE QUESTA SEZIONE
        //? POSSIAMO MIGLIORARLA FACENDO TUTTO SULLO STESSO BUFFER, FORSE NON SU PUÒ PER VIA DEL PIPELINING
        if (i != times-1){
            EnqueueWriteBuffer(cq, input_dram_buffer, input_vec.data(), false);  
            EnqueueWriteBuffer(cq, output_dram_buffer, output_vec.data(), false);  
        }  
    }

    Finish(cq);
    printf("Core {0, 0} on Device 0 completed the task.\n");
    CloseDevice(device);

    // ---------------------------------------------------------
    // ---------------------------------------------------------
    // SAVE RESULTS AND CLEANING
    // ---------------------------------------------------------
    // ---------------------------------------------------------

    //! TILIZZAZIONE SEMBRA NECESSARIA, CAPIRE
    //?output_vec = untilize_nfaces(result_vec, M, N);
    //! TILIZZAZIONE SEMBRA NECESSARIA, CAPIRE

    std::cout << "Output: " << std::endl;
    printMat(output_vec, rows_i2r, cols_i2r);

    //saveGridCSV("result.csv", res_temp, dim, dim);
    return 0;
}
