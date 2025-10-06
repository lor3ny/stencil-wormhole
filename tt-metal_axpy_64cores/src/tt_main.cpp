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

#include "submatrices.hpp"

using namespace tt;
using namespace tt::tt_metal;
using namespace std;

#define TILE_WIDTH 32 // bfloats
#define TILE_HEIGHT 32 // bfloats
#define TILE_SIZE 2048 // bfloats

// I wanto to have all the SRAM available, maybe I could use also multiple CBs
#define SRAM_TILES 64


int axpy_ttker(
    vector<bfloat16>& input, 
    vector<bfloat16>& up,
    vector<bfloat16>& left,
    vector<bfloat16>& right,
    vector<bfloat16>& down, 
    vector<bfloat16>& scalar, 
    vector<bfloat16>& output, 
    uint32_t n_tiles,  
    uint32_t rows, 
    uint32_t cols, 
    uint32_t iterations, 
    IDevice* device 
) {
    
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
            .size = up.size() * sizeof(bfloat16), 
            .page_size = TILE_SIZE,  
            .buffer_type = tt_metal::BufferType::DRAM};

    tt_metal::InterleavedBufferConfig dram_scalar_config{
            .device = device, 
            .size = scalar.size() * sizeof(bfloat16), 
            .page_size = TILE_SIZE,  
            .buffer_type = tt_metal::BufferType::DRAM};

    std::shared_ptr<tt::tt_metal::Buffer> input_dram_buffer = CreateBuffer(dram_inout_config);
    std::shared_ptr<tt::tt_metal::Buffer> up_dram_buffer = CreateBuffer(dram_inout_config);
    std::shared_ptr<tt::tt_metal::Buffer> left_dram_buffer = CreateBuffer(dram_inout_config);
    std::shared_ptr<tt::tt_metal::Buffer> right_dram_buffer = CreateBuffer(dram_inout_config);
    std::shared_ptr<tt::tt_metal::Buffer> down_dram_buffer = CreateBuffer(dram_inout_config);
    std::shared_ptr<tt::tt_metal::Buffer> output_dram_buffer = CreateBuffer(dram_inout_config);
    std::shared_ptr<tt::tt_metal::Buffer> scalar_dram_buffer = CreateBuffer(dram_scalar_config);

    // ! In this case you should declare the starting address and the size of the region in the shared cache
    // ! Then Is necessary a method to avoid contention, but for stencil could be avoided by the nature of the problem
    

    // ---------------------------------------------------------
    // SRAM BUFFER CREATION: TOTAL SHARED BETWEEN CORES 108MB HIGH-SPEED REGISTERS 
    // ---------------------------------------------------------

    cout << "Creating SRAM buffers..." << endl;
    
    constexpr uint32_t cb_indices[] = {
        CBIndex::c_0, // input
        CBIndex::c_1, // up
        CBIndex::c_2, // left
        CBIndex::c_3, // right
        CBIndex::c_4, // down
        CBIndex::c_5, // scalar
        CBIndex::c_6, // auxiliary
        CBIndex::c_7, // output
    };

    // Circular Buffers (CB) have to be created rising order!
    for(int i = 0; i < 8; i++) {

        CircularBufferConfig cb_config(TILE_SIZE * SRAM_TILES, 
                                            {{cb_indices[i], data_format}}
        );
        cb_config.set_page_size(cb_indices[i], TILE_SIZE);   
        tt_metal::CreateCircularBuffer(program, all_cores, cb_config);
    }

    // ---------------------------------------------------------
    // KERNELS CREATION: We need a reader, writer and then a compute
    // ---------------------------------------------------------

    cout << "Creating kernels..." << endl;


    std::vector<uint32_t> reader_compile_time_args;
    TensorAccessorArgs(*scalar_dram_buffer).append_to(reader_compile_time_args); 
    TensorAccessorArgs(*up_dram_buffer).append_to(reader_compile_time_args);
    TensorAccessorArgs(*left_dram_buffer).append_to(reader_compile_time_args); 
    TensorAccessorArgs(*right_dram_buffer).append_to(reader_compile_time_args);
    TensorAccessorArgs(*down_dram_buffer).append_to(reader_compile_time_args); 

    auto reader_kernel_id = tt_metal::CreateKernel( program, "/home/lpiarulli_tt/stencil-wormhole/tt-metal_axpy_64cores/src/kernels/dataflow/reader_input.cpp",
        all_cores, tt_metal::DataMovementConfig{ .processor = DataMovementProcessor::RISCV_1, 
                                            .noc = NOC::RISCV_1_default,
                                            .compile_args = reader_compile_time_args}
    );

    std::vector<uint32_t> writer_compile_time_args;
    TensorAccessorArgs(*output_dram_buffer).append_to(writer_compile_time_args); 
    
    auto writer_kernel_id = tt_metal::CreateKernel( program, "/home/lpiarulli_tt/stencil-wormhole/tt-metal_axpy_64cores/src/kernels/dataflow/writer_output.cpp",
        all_cores, tt_metal::DataMovementConfig{ .processor = DataMovementProcessor::RISCV_0,
                                            .noc = NOC::RISCV_0_default,
                                            .compile_args = writer_compile_time_args}
    );
    

    std::vector<uint32_t> compute_args = {};
    KernelHandle stencil_kernel_id = tt_metal::CreateKernel( 
        program, 
        "/home/lpiarulli_tt/stencil-wormhole/tt-metal_axpy_64cores/src/kernels/compute/stencil.cpp",
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
                up_dram_buffer->address(),
                left_dram_buffer->address(),
                right_dram_buffer->address(),
                down_dram_buffer->address(),
                scalar_dram_buffer->address(), 
                start_tile_idx,
                core.x*8 + core.y,
                work_per_core1
            });

            tt_metal::SetRuntimeArgs(program, writer_kernel_id, core, {
                output_dram_buffer->address(), start_tile_idx, work_per_core1
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
                up_dram_buffer->address(),
                left_dram_buffer->address(),
                right_dram_buffer->address(),
                down_dram_buffer->address(),
                scalar_dram_buffer->address(), 
                start_tile_idx,
                core.x*8 + core.y,
                work_per_core2
            });

            tt_metal::SetRuntimeArgs(program, writer_kernel_id, core, {
                output_dram_buffer->address(), start_tile_idx, work_per_core2
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

    vector<bfloat16> output_pad((rows+2) * (cols+2), 0.0f);
   
    EnqueueWriteBuffer(cq, up_dram_buffer, up.data(), true);   
    EnqueueWriteBuffer(cq, left_dram_buffer, left.data(), true);   
    EnqueueWriteBuffer(cq, right_dram_buffer, right.data(), true);   
    EnqueueWriteBuffer(cq, down_dram_buffer, down.data(), true);   
    EnqueueWriteBuffer(cq, scalar_dram_buffer, scalar.data(), true);       
    
    double elapsed_cpu = 0.0;
    double elapsed_memcpy = 0.0;
    double elapsed_wormhole = 0.0;
    std::chrono::_V2::system_clock::time_point start_total, end_total, start_wormhole, end_wormhole, start_memcpy, end_memcpy, start_cpu, end_cpu;
    std::chrono::duration<double, std::milli> elapsed;
    
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
            output[(rows/2)*cols + cols/2] = 100.0f;
            //pad_with_zeros(output.data(), output_pad.data(), rows, cols, 1);
            extract_5p_memcpy_singleloop(output.data(), 
                up.data(),
                left.data(),
                right.data(),
                down.data(),
                rows, 
                cols
            );
            end_cpu = std::chrono::high_resolution_clock::now();
            elapsed = end_cpu - start_cpu;
            elapsed_cpu += elapsed.count();

            start_memcpy = std::chrono::high_resolution_clock::now();
            EnqueueWriteBuffer(cq, up_dram_buffer, up.data(), true);   
            EnqueueWriteBuffer(cq, left_dram_buffer, left.data(), true);   
            EnqueueWriteBuffer(cq, right_dram_buffer, right.data(), true);   
            EnqueueWriteBuffer(cq, down_dram_buffer, down.data(), true);    
            end_memcpy = std::chrono::high_resolution_clock::now();
            elapsed = end_memcpy - start_memcpy;
            elapsed_memcpy += elapsed.count();
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
    //! direct from the input

    uint32_t num_tiles, stencil_tiles, dram_buffer_size, diff_dram;

    int ret, i, j;

    if (buffer_size < TILE_SIZE){
        cerr << "Error: problem size must be at least " << TILE_SIZE << " elements." << endl;
        return -1;
    }

    //* ----------
    //* INPUT ALLOCATION
    //* ----------

    vector<bfloat16> input_vec(rows * cols);
    for(i = 0; i<rows * cols; i++){
        input_vec[i] = bfloat16(0.0f);
    }
    input_vec[(rows/2)*cols + cols/2] = 100.0f;

    //* ----------
    //* PADDING
    //* ----------

    // Pad the input, and the output but it's not necessary
    vector<bfloat16> input_vec_pad(rows_pad * cols_pad, 0.0f);
    vector<bfloat16> output_vec_pad(rows_pad * cols_pad, 0.0f);
    pad_with_zeros(input_vec.data(), input_vec_pad.data(), rows, cols, 1);

    golden_stencil(input_vec_pad, output_vec_pad, rows_pad, cols_pad, iterations);

    //* ----------
    //* ALIGNEMENT
    //* ----------

    dram_buffer_size = rows * cols * sizeof(bfloat16);
    // dram_buffer_size = align_vector_size(input_vec_i2r, i2r_buffer_size, TILE_SIZE);
    // diff_dram = (dram_buffer_size - i2r_buffer_size) / sizeof(bfloat16);

    num_tiles = dram_buffer_size / TILE_SIZE;

    cout << "Problem shape: " << rows << "x" << cols << endl;
    cout << "Stencil order: " << stencil_order << endl;
    cout << "Padded shape: " << rows_pad << "x" << cols_pad << endl;
    cout << "DRAM buffer size (bytes): " << buffer_size << endl;
    cout << "Number of tiles: " << num_tiles << endl;

    //! KERNEL AREA
    int device_id = 0;
    IDevice* device = CreateDevice(device_id);
    const auto core_grid = device->compute_with_storage_grid_size();
    auto [num_cores, all_cores, core_group_1, core_group_2, work_per_core1, work_per_core2] = split_work_to_cores(core_grid, num_tiles);

    vector<bfloat16> up_vec(rows*cols);
    vector<bfloat16> left_vec(rows*cols);
    vector<bfloat16> right_vec(rows*cols);
    vector<bfloat16> down_vec(rows*cols);
    vector<bfloat16> scalar_vec(TILE_WIDTH*TILE_HEIGHT*num_cores, 0.25f);
    vector<bfloat16> output_vec(dram_buffer_size/sizeof(bfloat16), 0.0f);

    extract_submats_5p(input_vec_pad.data(), 
        up_vec.data(),
        left_vec.data(),
        right_vec.data(),
        down_vec.data(),
        rows, 
        cols,
        cols_pad
    );

    axpy_ttker(input_vec,
        up_vec, 
        left_vec, 
        right_vec, 
        down_vec, 
        scalar_vec, 
        output_vec, 
        num_tiles, 
        rows,
        cols,
        iterations, 
        device
    );

    CloseDevice(device);
    //! KERNEL AREA

    //cout << "Output: " << endl;
    //output_vec[(rows/2)*cols + cols/2] = 100.0f;
    //printMat(output_vec, rows, cols);

    return 0;
}
