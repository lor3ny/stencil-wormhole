// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <tt-metalium/host_api.hpp>
#include <tt-metalium/device.hpp>
#include <tt-metalium/command_queue.hpp>
#include <tt-metalium/constants.hpp>
#include <tt-metalium/util.hpp>
#include <tt-metalium/bfloat16.hpp>
#include <tt-metalium/test_tiles.hpp>
#include <tt-metalium/command_queue.hpp>
#include <tt-metalium/tilize_untilize.hpp>
#include <tt-metalium/work_split.hpp>
#include <vector>
#include <chrono>

using namespace tt;
using namespace tt::tt_metal;
using namespace std;

#define TILE_WIDTH 32
#define TILE_HEIGHT 32
#define TILE_VALUES_COUNT (TILE_WIDTH * TILE_HEIGHT) / sizeof(bfloat16)

//export TT_METAL_DPRINT_CORES="(0,0)-(7,7)"

inline void saveGridCSV(const std::string& filename, double* grid, int dim_x, int dim_y) {
    std::ofstream file(filename);
    if (!file) {
        cerr << "Errore nell'apertura del file" << endl;
        return;
    }
    file << dim_x << " " << dim_y << endl;
    for (size_t i=0; i<dim_x; ++i) {
        for (size_t j = 0; j < dim_y; ++j) {
            file << grid[i*dim_x + j] << (j + 1 == dim_y ? "\n" : " ");
        }
    }
    file.close();
}


void TT_printMatrix(uint32_t* matrix, size_t rows, size_t cols) {
    bfloat16* a_bf16 = reinterpret_cast<bfloat16*>(matrix);

    for(int i =0; i<rows; i++){
        for(int j =0; j<cols; j++){
            std::cout << a_bf16[i*cols + j].to_float() << " ";
        }
        std::cout << "\n";
    }
    std::cout << std::flush;
}


int main(int argc, char** argv) {

    // Create the device and the program
    int device_id = 0;
    IDevice* device = CreateDevice(device_id);
    CommandQueue& cq = device->command_queue(); // Take the command_queue from the device created
    Program program = CreateProgram();
    
    constexpr uint32_t single_tile_size = TILE_WIDTH*TILE_HEIGHT; 
    const auto compute_with_storage_grid_size = device->compute_with_storage_grid_size();
    const uint32_t num_cores_x = compute_with_storage_grid_size.x;
    const uint32_t num_cores_y = compute_with_storage_grid_size.y;
    const uint32_t num_cores_total = num_cores_x*num_cores_y;
    const uint16_t num_sram_tiles = num_cores_total; // Non capisco perchè non si possa mettere ad 1
    const uint16_t num_tiles = num_cores_total;

    const uint16_t rows = num_cores_total;  
    constexpr uint16_t cols = TILE_VALUES_COUNT;
    constexpr uint16_t stencil_neighbours = 1; // This implementation works only for a 5-stencil like Laplace

    const uint16_t padding = stencil_neighbours * 2;  //corrisponde a num_tile
    const uint16_t padded_cols = cols + padding; //32
    const uint16_t padded_rows = rows + padding; //16

    auto all_device_cores = CoreRange({0, 0}, {num_cores_x - 1, num_cores_y - 1});

    cout << "CORES X: " << compute_with_storage_grid_size.x << " CORES Y: " << compute_with_storage_grid_size.x <<endl;

    const uint32_t dram_buffer_size = single_tile_size * num_tiles; // Real input size
    const uint32_t input_buffer_size = padded_rows*padded_cols*sizeof(bfloat16); // Padded input size


    if(rows*cols*2 != dram_buffer_size){
        cerr << "Error: DRAM buffer must be equal to ROWS x COLS x SIZE OF BFP16" << endl;
        return EXIT_FAILURE;
    }

    uint32_t* input_mat = (uint32_t*) malloc(input_buffer_size); // It is a BFP16
    uint32_t* submat_center = (uint32_t*) malloc(dram_buffer_size); 
    uint32_t* submat_up = (uint32_t*) malloc(dram_buffer_size); 
    uint32_t* submat_left = (uint32_t*) malloc(dram_buffer_size); 
    uint32_t* submat_right = (uint32_t*) malloc(dram_buffer_size); 
    uint32_t* submat_down = (uint32_t*) malloc(dram_buffer_size); 
    uint32_t* scalar = (uint32_t*) malloc(dram_buffer_size); 
    uint32_t* output_mat = (uint32_t*) malloc(dram_buffer_size);

    bfloat16* center_bf16 = reinterpret_cast<bfloat16*>(submat_center);
    bfloat16* up_bf16 = reinterpret_cast<bfloat16*>(submat_up);
    bfloat16* left_bf16 = reinterpret_cast<bfloat16*>(submat_left);
    bfloat16* right_bf16 = reinterpret_cast<bfloat16*>(submat_right);
    bfloat16* down_bf16 = reinterpret_cast<bfloat16*>(submat_down);
    bfloat16* input_bf16 = reinterpret_cast<bfloat16*>(input_mat);
    bfloat16* scalar_bf16 = reinterpret_cast<bfloat16*>(scalar);

    for(int i = 0; i<padded_rows; i++){
        for(int j = 0; j<padded_cols; j++){
            bfloat16 val = 0.0f;
            if(i==0)
                val = 1.0f;
            else if(j==0)
                val = 1.0f;
            else if (j==padded_cols-1)
                val = 1.0f;
            else if (i==padded_rows-1)
                val = 1.0f;

            input_bf16[i*padded_cols + j] = val;
        }
    }
    input_bf16[(padded_rows/2)*padded_cols + (padded_cols/2)] = 100.0f; //CENTRAL

    for(int i = 0; i<rows; i++){
        for(int j = 0; j<cols; j++){
            center_bf16[i*cols + j] = input_bf16[(i+1)*padded_cols + (j+1)]; //CENTRAL
            up_bf16[i*cols + j] = input_bf16[i*padded_cols + (j+1)]; //UP
            left_bf16[i*cols + j] = input_bf16[(i+1)*padded_cols + j]; //LEFT
            right_bf16[i*cols + j] = input_bf16[(i+1)*padded_cols + (j+2)]; //RIGHT
            down_bf16[i*cols + j] = input_bf16[(i+2)*padded_cols + (j+1)]; //DOWN

            scalar_bf16[i*cols + j] = 0.25f; //SCALAR
        }
    }

    //std::cout << "Input: " << std::endl;
    //TT_printMatrix(input_mat, padded_rows, padded_cols);
    //std::cout << "Input UP: " << std::endl;
    //TT_printMatrix(submat_up, rows, cols);
    //std::cout << "Input DOWN: " << std::endl;
    //TT_printMatrix(submat_down, rows, cols);
    //std::cout << "Input RIGHT: " << std::endl;
    //TT_printMatrix(submat_right, rows, cols);
    //std::cout << "Input LEFT: " << std::endl;
    //TT_printMatrix(submat_left, rows, cols);
    //std::cout << "Input CENTER: " << std::endl;
    //TT_printMatrix(submat_center, rows, cols);

    // ---------------------------------------------------------
    // ---------------------------------------------------------
    // HOST INITIALIZATION
    // ---------------------------------------------------------
    // ---------------------------------------------------------

    //constexpr CoreCoord core = {0, 0};  
    DataFormat data_format = DataFormat::Float16_b; 
    MathFidelity math_fidelity = MathFidelity::HiFi4;

    // ---------------------------------------------------------
    // DRAM BUFFER CREATION: OFFCHIP GDDR6 MEMORY 12GB
    // ---------------------------------------------------------
    
    cout << "Creating DRAM buffers..." << endl;

    // Both input and output have the same configuration, in this case I have chosen Interleaved instead of Shreaded

    // TT suggests to set page_size to your single_tile_size size
    tt_metal::InterleavedBufferConfig dram_submat_config{
        .device = device, 
        .size = dram_buffer_size, 
        .page_size = single_tile_size,  
        .buffer_type = tt_metal::BufferType::DRAM
    };
    std::shared_ptr<tt::tt_metal::Buffer> input_dram_buffer_CENTER = CreateBuffer(dram_submat_config);
    std::shared_ptr<tt::tt_metal::Buffer> input_dram_buffer_UP = CreateBuffer(dram_submat_config);
    std::shared_ptr<tt::tt_metal::Buffer> input_dram_buffer_LEFT = CreateBuffer(dram_submat_config);
    std::shared_ptr<tt::tt_metal::Buffer> input_dram_buffer_RIGHT = CreateBuffer(dram_submat_config);
    std::shared_ptr<tt::tt_metal::Buffer> input_dram_buffer_DOWN = CreateBuffer(dram_submat_config);
    std::shared_ptr<tt::tt_metal::Buffer> scalar_dram_buffer = CreateBuffer(dram_submat_config);
    std::shared_ptr<tt::tt_metal::Buffer> output_dram_buffer = CreateBuffer(dram_submat_config);
    

    // ---------------------------------------------------------
    // SRAM BUFFER CREATION: TOTAL SHARED BETWEEN CORES 108MB HIGH-SPEED REGISTERS 
    // ---------------------------------------------------------

    cout << "Creating SRAM buffers..." << endl;

    constexpr uint32_t cb_indices[] = {
        CBIndex::c_0,
        CBIndex::c_1,
        CBIndex::c_2,
        CBIndex::c_3,
        CBIndex::c_4,
        CBIndex::c_5,
        CBIndex::c_6,
        CBIndex::c_7,
    };

    // Circular Buffers (CB) have to be created rising order!
    for(int i = 0; i < 8; i++) {
        CircularBufferConfig cb_config( single_tile_size * num_sram_tiles, 
                                            {{cb_indices[i], data_format}}
        );
        cb_config.set_page_size(cb_indices[i], single_tile_size);   
        tt_metal::CreateCircularBuffer(program, all_device_cores, cb_config);
    }

    /*
    uint32_t cb_0_index = CBIndex::c_0;  // 0
    CircularBufferConfig cb_0_config( single_tile_size * num_sram_tiles, 
                                          {{cb_0_index, data_format}}
    );
    cb_0_config.set_page_size(cb_0_index, dram_buffer_size);

    uint32_t cb_1_index = CBIndex::c_1;  // 1
    CircularBufferConfig cb_1_config( single_tile_size * num_sram_tiles, 
                                          {{cb_1_index, data_format}}
    );
    cb_1_config.set_page_size(cb_1_index, dram_buffer_size);

    uint32_t cb_2_index = CBIndex::c_2;  // 2
    CircularBufferConfig cb_2_config( single_tile_size * num_sram_tiles, 
                                          {{cb_2_index, data_format}}
    );
    cb_2_config.set_page_size(cb_2_index, dram_buffer_size);

    uint32_t cb_3_index = CBIndex::c_3;  // 3
    CircularBufferConfig cb_3_config( single_tile_size * num_sram_tiles, 
                                          {{cb_3_index, data_format}}
    );
    cb_3_config.set_page_size(cb_3_index, dram_buffer_size);

    uint32_t cb_4_index = CBIndex::c_4;  // 4
    CircularBufferConfig cb_4_config(single_tile_size * num_sram_tiles, 
                                          {{cb_4_index, data_format}}
    );
    cb_4_config.set_page_size(cb_4_index, dram_buffer_size);

    uint32_t cb_5_index = CBIndex::c_5;  // 5
    CircularBufferConfig cb_5_config(single_tile_size * num_sram_tiles, 
                                           {{cb_5_index, data_format}}
    );
    cb_5_config.set_page_size(cb_5_index, dram_buffer_size);

    uint32_t cb_output_index = CBIndex::c_6;  // 6
    CircularBufferConfig cb_output_config( single_tile_size * num_sram_tiles, 
                                           {{cb_output_index, data_format}}
    );
    cb_output_config.set_page_size(cb_output_index, dram_buffer_size);

    uint32_t cb_scalar_index = CBIndex::c_7;  // 6
    CircularBufferConfig cb_scalar_config(single_tile_size * num_sram_tiles, 
                                           {{cb_scalar_index, data_format}}
    );
    cb_scalar_config.set_page_size(cb_scalar_index, dram_buffer_size);

    CBHandle cb_0 = tt_metal::CreateCircularBuffer(program, core, cb_0_config);
    CBHandle cb_1 = tt_metal::CreateCircularBuffer(program, core, cb_1_config); 
    CBHandle cb_2 = tt_metal::CreateCircularBuffer(program, core, cb_2_config);
    CBHandle cb_3 = tt_metal::CreateCircularBuffer(program, core, cb_3_config);
    CBHandle cb_4 = tt_metal::CreateCircularBuffer(program, core, cb_4_config);
    CBHandle cb_5 = tt_metal::CreateCircularBuffer(program, core, cb_5_config);
    CBHandle cb_output = tt_metal::CreateCircularBuffer(program, core, cb_output_config);
    CBHandle cb_scalar = tt_metal::CreateCircularBuffer(program, core, cb_scalar_config);
    */

    // ---------------------------------------------------------
    // KERNELS CREATION: We need a reader, writer and then a compute
    // ---------------------------------------------------------

    
    cout << "Creating kernels..." << endl;

    bool input_is_dram = input_dram_buffer_UP->buffer_type() == tt_metal::BufferType::DRAM ? 1 : 0;
    std::vector<uint32_t> reader_compile_time_args = {(uint32_t)input_is_dram};

    bool output_is_dram = output_dram_buffer->buffer_type() == tt_metal::BufferType::DRAM ? 1 : 0;
    std::vector<uint32_t> writer_compile_time_args = {(uint32_t)output_is_dram};

    auto reader_kernel_id = tt_metal::CreateKernel( 
        program, 
        "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/src/kernels/dataflow/reader_input.cpp",
        all_device_cores, 
        tt_metal::DataMovementConfig{ 
            .processor = DataMovementProcessor::RISCV_1,      
            .noc = NOC::RISCV_1_default,
            .compile_args = reader_compile_time_args
        }
    );

    
    auto writer_kernel_id = tt_metal::CreateKernel( 
        program, 
        "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/src/kernels/dataflow/writer_output.cpp",
        all_device_cores, 
        tt_metal::DataMovementConfig{ 
            .processor = DataMovementProcessor::RISCV_0,
            .noc = NOC::RISCV_0_default,
            .compile_args = writer_compile_time_args
        }
    );
    

    std::vector<uint32_t> compute_args = {};
    KernelHandle stencil_kernel_id = tt_metal::CreateKernel( 
        program, 
        "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/src/kernels/compute/stencil.cpp",
        all_device_cores, 
        tt_metal::ComputeConfig{ 
            .math_fidelity = math_fidelity,
            .fp32_dest_acc_en = false, 
            .math_approx_mode = false,
            .compile_args = compute_args,
        }
    );


    // ---------------------------------------------------------
    // ENQUEUE WRITE BUFFERS: Write data on the allocated buffers
    // ---------------------------------------------------------
    
    cout << "Enqueueing write buffers..." << endl;

    EnqueueWriteBuffer(cq, input_dram_buffer_CENTER, submat_center, false);  // E' UNA MEMCPY, NULLA DI PIÙ
    EnqueueWriteBuffer(cq, input_dram_buffer_UP, submat_up, false);  // E' UNA MEMCPY, NULLA DI PIÙ
    EnqueueWriteBuffer(cq, input_dram_buffer_LEFT, submat_left, false);  // E' UNA MEMCPY, NULLA DI PIÙ
    EnqueueWriteBuffer(cq, input_dram_buffer_RIGHT, submat_right, false);  // E' UNA MEMCPY, NULLA DI PIÙ
    EnqueueWriteBuffer(cq, input_dram_buffer_DOWN, submat_down, false);  // E' UNA MEMCPY, NULLA DI PIÙ
    EnqueueWriteBuffer(cq, output_dram_buffer, output_mat, false);  // E' UNA MEMCPY, NULLA DI PIÙ      
    EnqueueWriteBuffer(cq, scalar_dram_buffer, scalar, false);    
    
    // ---------------------------------------------------------
    // SETUP RUNTIME ARGS: Set the runtime arguments for the kernels
    // ---------------------------------------------------------

    cout << "Setting up runtime arguments..." << endl;

    bool row_major = false;

    for(uint32_t i = 0; i < num_cores_total; i++) {

        CoreCoord core = {i / num_cores_y, i % num_cores_y};
        uint32_t my_tile_index = i;

        tt_metal::SetRuntimeArgs( program, reader_kernel_id, core, {
            input_dram_buffer_CENTER->address(), 
            input_dram_buffer_UP->address(), 
            input_dram_buffer_LEFT->address(), 
            input_dram_buffer_RIGHT->address(), 
            input_dram_buffer_DOWN->address(), 
            scalar_dram_buffer->address(), 
            my_tile_index,
        });
    
        tt_metal::SetRuntimeArgs(program, writer_kernel_id, core, {
            output_dram_buffer->address(), 
            num_tiles, 
            my_tile_index,
        });
    
        tt_metal::SetRuntimeArgs(program, stencil_kernel_id, core, {});
    }

    // ---------------------------------------------------------
    // LAUNCH AND WAIT TERMINATION: Set the runtime arguments for the kernels
    // ---------------------------------------------------------

    cout << "Enqueueing kernels..." << endl;	
        
    auto start = std::chrono::high_resolution_clock::now();
    EnqueueProgram(cq, program, false);
    EnqueueReadBuffer(cq, output_dram_buffer, output_mat, true); // Read the result from the device
    Finish(cq);
    auto end = std::chrono::high_resolution_clock::now();
    CloseDevice(device);

    // ---------------------------------------------------------
    // ---------------------------------------------------------
    // SAVE RESULTS AND CLEANING
    // ---------------------------------------------------------
    // ---------------------------------------------------------

    std::cout << "Output: " << std::endl;
    TT_printMatrix(output_mat, rows, cols);

    free(submat_center);
    free(submat_up);
    free(submat_left);
    free(submat_right);
    free(submat_down);
    free(input_mat);
    free(output_mat);

    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;

    return EXIT_SUCCESS;
}
