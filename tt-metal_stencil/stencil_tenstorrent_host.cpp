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
#include <vector>

using namespace tt;
using namespace tt::tt_metal;
using namespace std;


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


inline void PrintGrid(bfloat16 *grid, int rows, int cols){
    cout << "Start Grid" << endl;
    int i, j;
    for(i = 0; i < rows; ++i){
        for(j = 0; j < cols; ++j){
            cout << " " << grid[i*cols + j] << " ";
        }
        cout << endl << endl;
    }
    cout << "End Grid" << endl;
}

int main(int argc, char** argv) {

    // ---------------------------------------------------------
    // ---------------------------------------------------------
    // HEAT PROPAGATION STENCIL SETUP
    // ---------------------------------------------------------
    // ---------------------------------------------------------

    cout << "Setting up heat propagation stencil..." << endl;

    int mat_cols = 514;
    int mat_rows = 3;

    int submat_cols = 512;  //corrisponde a single_tile_size in values
    int submat_rows = 1;  //corrisponde a num_tiles

    constexpr uint32_t single_tile_size = 32*32; 
    constexpr uint32_t num_tiles = 1; 
    constexpr uint32_t dram_buffer_size = single_tile_size * num_tiles; // SUB MATRIX SIZE

    bfloat16* submat_center = (bfloat16*) malloc(dram_buffer_size); 
    bfloat16* submat_up = (bfloat16*) malloc(dram_buffer_size); 
    bfloat16* submat_left = (bfloat16*) malloc(dram_buffer_size); 
    bfloat16* submat_right = (bfloat16*) malloc(dram_buffer_size); 
    bfloat16* submat_down = (bfloat16*) malloc(dram_buffer_size); 


    bfloat16* input_matrix = (bfloat16*) malloc(mat_rows*mat_cols*2); // It is a BFP16
    bfloat16* output_matrix = (bfloat16*) malloc(dram_buffer_size);

    memset(input_matrix, 0, dram_buffer_size);
    memset(output_matrix, 0, dram_buffer_size);

    for(int i = 0; i<mat_rows; i++){
        for(int j = 0; j<mat_cols; j++){
            int val = 0;
            if(i==0)
                val = 2;
            else if(j==0)
                val = 2;
            else if (j==mat_cols-1)
                val = 2;
            else if (i==mat_rows-1)
                val = 2;

            input_matrix[i*mat_cols + j] = val;
        }
    }
    //input_matrix[(mat_cols/2)*mat_cols + (mat_rows/2)] = 5;

    for(int i = 0; i<submat_rows; i++){
        for(int j = 0; j<submat_cols; j++){
            submat_center[i*submat_cols + j] = input_matrix[(i+1)*mat_cols + (j+1)]; //CENTRAL
            submat_up[i*submat_cols + j] = input_matrix[i*submat_cols + (j+1)]; //UP
            submat_left[i*submat_cols + j] = input_matrix[(i+1)*mat_cols + j]; //LEFT
            submat_right[i*submat_cols + j] = input_matrix[(i+1)*mat_cols + (j+2)]; //RIGHT
            submat_down[i*submat_cols + j] = input_matrix[(i+2)*mat_cols + (j+1)]; //DOWN
        }
    }


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
    DataFormat data_format = DataFormat::Float32;  // Convert it to 32
    MathFidelity math_fidelity = MathFidelity::HiFi4;

    // ---------------------------------------------------------
    // DRAM BUFFER CREATION: OFFCHIP GDDR6 MEMORY 12GB
    // ---------------------------------------------------------
    
    cout << "Creating DRAM buffers..." << endl;

    // Both input and output have the same configuration, in this case I have chosen Interleaved instead of Shreaded

    // deivce, size, page_size, buffer_type
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
    std::shared_ptr<tt::tt_metal::Buffer> output_dram_buffer = CreateBuffer(dram_submat_config);
    

    // ---------------------------------------------------------
    // SRAM BUFFER CREATION: TOTAL SHARED BETWEEN CORES 108MB HIGH-SPEED REGISTERS 
    // ---------------------------------------------------------

    cout << "Creating SRAM buffers..." << endl;

    uint32_t num_sram_tiles = num_tiles;

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

    // MAYBE I CAN USE ONLY ONE CONFIG!

    CBHandle cb_0 = tt_metal::CreateCircularBuffer(program, core, cb_0_config);
    CBHandle cb_1 = tt_metal::CreateCircularBuffer(program, core, cb_1_config); 
    CBHandle cb_2 = tt_metal::CreateCircularBuffer(program, core, cb_2_config);
    CBHandle cb_3 = tt_metal::CreateCircularBuffer(program, core, cb_3_config);
    CBHandle cb_4 = tt_metal::CreateCircularBuffer(program, core, cb_4_config);
    CBHandle cb_5 = tt_metal::CreateCircularBuffer(program, core, cb_5_config);
    CBHandle cb_output = tt_metal::CreateCircularBuffer(program, core, cb_output_config);

    // ---------------------------------------------------------
    // KERNELS CREATION: We need a reader, writer and then a compute
    // ---------------------------------------------------------

    
    cout << "Creating kernels..." << endl;

    bool input_is_dram = input_dram_buffer_UP->buffer_type() == tt_metal::BufferType::DRAM ? 1 : 0;
    std::vector<uint32_t> reader_compile_time_args = {(uint32_t)input_is_dram};

    bool output_is_dram = output_dram_buffer->buffer_type() == tt_metal::BufferType::DRAM ? 1 : 0;
    std::vector<uint32_t> writer_compile_time_args = {(uint32_t)output_is_dram};

    auto reader_kernel_id = tt_metal::CreateKernel( program, "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/src/kernels/dataflow/reader_input.cpp",
        core, tt_metal::DataMovementConfig{ .processor = DataMovementProcessor::RISCV_1, 
                                            .noc = NOC::RISCV_1_default,
                                            .compile_args = reader_compile_time_args}
    );

    
    auto writer_kernel_id = tt_metal::CreateKernel( program, "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/src/kernels/dataflow/writer_output.cpp",
        core, tt_metal::DataMovementConfig{ .processor = DataMovementProcessor::RISCV_0,
                                            .noc = NOC::RISCV_0_default,
                                            .compile_args = writer_compile_time_args}
    );
    

    std::vector<uint32_t> compute_args = {};
    KernelHandle stencil_kernel_id = tt_metal::CreateKernel( 
        program, 
        "/home/lpiarulli_tt/stencil_wormhole/tt-metal_stencil/src/kernels/compute/stencil.cpp",
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
    cout << endl;

    EnqueueWriteBuffer(cq, input_dram_buffer_CENTER, submat_center, false);  // E' UNA MEMCPY, NULLA DI PIÙ
    EnqueueWriteBuffer(cq, input_dram_buffer_UP, submat_up, false);  // E' UNA MEMCPY, NULLA DI PIÙ
    EnqueueWriteBuffer(cq, input_dram_buffer_LEFT, submat_left, false);  // E' UNA MEMCPY, NULLA DI PIÙ
    EnqueueWriteBuffer(cq, input_dram_buffer_RIGHT, submat_right, false);  // E' UNA MEMCPY, NULLA DI PIÙ
    EnqueueWriteBuffer(cq, input_dram_buffer_DOWN, submat_down, false);  // E' UNA MEMCPY, NULLA DI PIÙ
    EnqueueWriteBuffer(cq, output_dram_buffer, output_matrix, false);  // E' UNA MEMCPY, NULLA DI PIÙ          
    
    // ---------------------------------------------------------
    // SETUP RUNTIME ARGS: Set the runtime arguments for the kernels
    // ---------------------------------------------------------

    cout << "Setting up runtime arguments..." << endl;

    const uint32_t input_bank_id = 0;
    const uint32_t output_bank_id = 0;
    const uint32_t stencil_scalar = 1; // SETTING IT TO uint32_t means to have an integer than? maybe

    // QUI SETTO RUNTIME ARGS PER READER
    tt_metal::SetRuntimeArgs( program, reader_kernel_id, core, {
        input_dram_buffer_CENTER->address(), 
        input_dram_buffer_UP->address(), 
        input_dram_buffer_LEFT->address(), 
        input_dram_buffer_RIGHT->address(), 
        input_dram_buffer_DOWN->address(), 
        num_tiles, 
        input_bank_id
    });

    // QUI SETTO RUNTIME ARGS PER WRITER
    tt_metal::SetRuntimeArgs(program, writer_kernel_id, core, 
            {output_dram_buffer->address(), num_tiles, output_bank_id, output_dram_buffer->size()});

    tt_metal::SetRuntimeArgs(program, stencil_kernel_id, core, {});

    // ---------------------------------------------------------
    // LAUNCH AND WAIT TERMINATION: Set the runtime arguments for the kernels
    // ---------------------------------------------------------

    PrintGrid(submat_up, submat_rows, submat_cols);
    PrintGrid(submat_down, submat_rows, submat_cols);
    PrintGrid(submat_center, submat_rows, submat_cols);

    cout << "Enqueueing kernels..." << endl;	
        
    EnqueueProgram(cq, program, false);
    EnqueueReadBuffer(cq, output_dram_buffer, submat_center, true); // Read the result from the device
    Finish(cq);
    printf("Core {0, 0} on Device 0 completed the task.\n");
    CloseDevice(device);

    // ---------------------------------------------------------
    // ---------------------------------------------------------
    // SAVE RESULTS AND CLEANING
    // ---------------------------------------------------------
    // ---------------------------------------------------------

    PrintGrid(submat_center, submat_rows, submat_cols);

    free(submat_center);
    free(submat_up);
    free(submat_left);
    free(submat_right);
    free(submat_down);
    free(input_matrix);
    free(output_matrix);

    return 0;
}
