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

#include <unistd.h>

#include "im2row.hpp"

using namespace tt;
using namespace tt::tt_metal;
using namespace std;

#define TILE_WIDTH 32 // bfloats
#define TILE_HEIGHT 32 // bfloats

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
    constexpr uint32_t rows = 32, cols = 32;
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
    const uint32_t cols_i2r = (stencil_order*4)+1; 
    const size_t i2r_buffer_count = rows * cols * ((stencil_order*4)+1);
    const size_t i2r_buffer_size = i2r_buffer_count * sizeof(bfloat16); 
    //! direct from the input

    if (i2r_buffer_size < single_tile_size){
        cerr << "Error: problem size must be at least " << single_tile_size << " elements." << endl;
        return -1;
    }

    // Initilize starting buffers
    uint32_t uint32_count = buffer_size / sizeof(uint32_t);
    vector<uint32_t> input_vec(uint32_count);
    input_vec = create_constant_vector_of_bfloat16(buffer_size, 5.5f);

    bfloat16* init_input_ptr = reinterpret_cast<bfloat16*>(input_vec.data());
    for(int i = 0; i<uint32_count*2; i++){
        init_input_ptr[i] = bfloat16((float)i);
    }

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

    vector<uint32_t> input_vec_i2r(i2r_buffer_count);
    im2row_5p(input_vec, input_vec_i2r, rows_pad, cols_pad);
    
    //* ----------
    //* ALIGNEMENT
    //* ----------


    cout << "DRAM buffer size (bytes): " << i2r_buffer_size << endl;

    uint32_t dram_buffer_size = align_vector_size(input_vec_i2r, i2r_buffer_size, single_tile_size);
    dram_buffer_size = align_vector_size(input_vec, pad_buffer_size, single_tile_size);

    uint32_t diff_dram = (dram_buffer_size - i2r_buffer_size) / sizeof(bfloat16);

    std::cout << "Input: " << std::endl;
    printMat(input_vec, rows_pad, cols_pad);

    // std::cout << "Input Padded im2row-ed Aligned: " << std::endl;
    // printMat(input_vec_i2r, rows_i2r+diff_dram/5, cols_i2r);

    //! OUTPUT Device handling
    vector<uint32_t> output_vec(dram_buffer_size/sizeof(uint32_t));
    output_vec = create_constant_vector_of_bfloat16(dram_buffer_size, 0.0f);
    //! THIS SECTION HAVE TO BE ADAPATED TO THE TT KERNEL
    //! Now works only for copy! 

    // ---------------------------------------------------------
    // ---------------------------------------------------------
    // HOST INITIALIZATION
    // ---------------------------------------------------------
    // ---------------------------------------------------------

    cout << "Initializing..." << endl;
    
    // Create the device and the program
    int device_id = 0;
    IDevice* device = CreateDevice(device_id);
    CommandQueue& cq = device->command_queue(); // Take the command_queue from the device created
    Program program = CreateProgram();

    constexpr CoreCoord core = {0, 0};  
    const uint32_t num_tiles = dram_buffer_size / single_tile_size; // Number of tiles 
    DataFormat data_format = DataFormat::Float16_b;
    DataFormat::Float32;
    MathFidelity math_fidelity = MathFidelity::HiFi4;

    cout << "Number of tiles: " << num_tiles << endl;

    // ---------------------------------------------------------
    // DRAM BUFFER CREATION: OFFCHIP GDDR6 MEMORY 12GB
    // ---------------------------------------------------------
    
    cout << "Creating DRAM buffers..." << endl;

    // Both input and output have the same configuration, in this case I have chosen Interleaved instead of Shreaded

    // device, size, page_size, buffer_type
    tt_metal::InterleavedBufferConfig dram_config{.device = device, 
                                                  .size = dram_buffer_size, 
                                                  .page_size = single_tile_size,  
                                                  .buffer_type = tt_metal::BufferType::DRAM};
    std::shared_ptr<tt::tt_metal::Buffer> input_dram_buffer = CreateBuffer(dram_config);
    std::shared_ptr<tt::tt_metal::Buffer> output_dram_buffer = CreateBuffer(dram_config);

    // ! In this case you should declare the starting address and the size of the region in the shared cache
    // ! Then Is necessary a method to avoid contention, but for stencil could be avoided by the nature of the problem
    

    // ---------------------------------------------------------
    // SRAM BUFFER CREATION: TOTAL SHARED BETWEEN CORES 108MB HIGH-SPEED REGISTERS 
    // ---------------------------------------------------------

    cout << "Creating SRAM buffers..." << endl;

    uint32_t num_sram_tiles = num_tiles;
    uint32_t cb_input_index = CBIndex::c_0;  // 0
    // size, page_size
    CircularBufferConfig cb_input_config( single_tile_size * num_sram_tiles, 
                                          {{cb_input_index, data_format}}
    );
    cb_input_config.set_page_size(cb_input_index, single_tile_size);

    uint32_t cb_output_index = CBIndex::c_16;  // 16
    // size, page_size
    CircularBufferConfig cb_output_config( single_tile_size * num_sram_tiles, 
                                           {{cb_output_index, data_format}}
    );
    cb_output_config.set_page_size(cb_output_index, single_tile_size);

    CBHandle cb_input = tt_metal::CreateCircularBuffer(program, core, cb_input_config);
    CBHandle cb_output = tt_metal::CreateCircularBuffer(program, core, cb_output_config);

    // ---------------------------------------------------------
    // KERNELS CREATION: We need a reader, writer and then a compute
    // ---------------------------------------------------------

    cout << "Creating kernels..." << endl;

    bool input_is_dram = input_dram_buffer->buffer_type() == tt_metal::BufferType::DRAM ? 1 : 0;
    std::vector<uint32_t> reader_compile_time_args = {(uint32_t)input_is_dram};

    bool output_is_dram = output_dram_buffer->buffer_type() == tt_metal::BufferType::DRAM ? 1 : 0;
    std::vector<uint32_t> writer_compile_time_args = {(uint32_t)output_is_dram};

    auto reader_kernel_id = tt_metal::CreateKernel( program, "/home/lpiarulli_tt/stencil-wormhole/tt-metal_stencil/src/kernels/dataflow/reader_input.cpp",
        core, tt_metal::DataMovementConfig{ .processor = DataMovementProcessor::RISCV_1, 
                                            .noc = NOC::RISCV_1_default,
                                            .compile_args = reader_compile_time_args}
    );

    
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
    // ENQUEUE WRITE BUFFERS: Write data on the allocated buffers
    // ---------------------------------------------------------
    
    cout << "Enqueueing write buffers..." << endl;

    //! This is the first memcpy, it should be done only one time at the beginnning
    //! WHY IT'S WORKING? input dram buffer is less than input_vec size
    EnqueueWriteBuffer(cq, input_dram_buffer, input_vec.data(), false);  // E' UNA MEMCPY, NULLA DI PIÙ
    //EnqueueWriteBuffer(cq, output_dram_buffer, output_vec.data(), false);  // E' UNA MEMCPY, NULLA DI PIÙ          
    
    // ---------------------------------------------------------
    // SETUP RUNTIME ARGS: Set the runtime arguments for the kernels
    // ---------------------------------------------------------

    cout << "Setting up runtime arguments..." << endl;

    const uint32_t input_bank_id = 0;
    const uint32_t output_bank_id = 0;

    // QUI SETTO RUNTIME ARGS PER READER
    tt_metal::SetRuntimeArgs( program, reader_kernel_id, core,
            {input_dram_buffer->address(), num_tiles, input_bank_id, input_dram_buffer->size()});
    // QUI SETTO RUNTIME ARGS PER WRITER
    tt_metal::SetRuntimeArgs(program, writer_kernel_id, core, 
            {output_dram_buffer->address(), num_tiles, output_bank_id, output_dram_buffer->size()});
    tt_metal::SetRuntimeArgs(program, stencil_kernel_id, core, {num_tiles});
    

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

    sleep(10);

    std::cout << "Output: " << std::endl;
    printMat(output_vec, rows_pad, cols_pad);

    //saveGridCSV("result.csv", res_temp, dim, dim);
    return 0;
}
