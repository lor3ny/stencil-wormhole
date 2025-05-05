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

inline void InitializeGrid(double *grid, int dim){
    int i, j;
    for(i = 0; i < dim; ++i){
        for(j = 0; j < dim; ++j){
            grid[i*dim +j] = 0.0;
        }
    }
}

inline void PrintGrid(uint32_t *grid, int dim){
    int i, j;
    for(i = 0; i < dim; ++i){
        for(j = 0; j < dim; ++j){
            cout << " " << grid[i*dim + j] << " ";
        }
        cout << endl;
    }
}

int main(int argc, char** argv) {

    // ---------------------------------------------------------
    // ---------------------------------------------------------
    // HEAT PROPAGATION STENCIL SETUP
    // ---------------------------------------------------------
    // ---------------------------------------------------------

    cout << "Setting up heat propagation stencil..." << endl;

    //int dim = 1000; // Grid Size
    //double lx = 10.0, ly = 10.0;   // Domain Size
    //float max_time = 1;

    //double dx = lx / (double) (dim - 1);
    //double dy = ly / (double) (dim - 1);
    //double dt = 0.0001;  // Time step
    //double alpha = 0.1;  // Coefficient of diffusion

    constexpr uint32_t single_tile_size = 32*32; 
    constexpr uint32_t num_tiles = 1; 
    constexpr uint32_t dram_buffer_size = single_tile_size * num_tiles; 
    constexpr uint32_t dim = 8;  

    uint32_t* input_vec = (uint32_t*) malloc(dram_buffer_size); // It is a BFP16
    uint32_t* output_vec= (uint32_t*) malloc(dram_buffer_size);

    memset(input_vec, 0, dram_buffer_size);
    memset(output_vec, 0, dram_buffer_size);


    for(int i = 0; i<dim; i++){
        for(int j = 0; j<dim; j++){
            int val = 0;
            if(i==0)
                val = 1;
            else if(j==0)
                val = 1;
            else if (j==dim-1)
                val = 1;
            else if (i==dim-1)
                val = 1;

            input_vec[i*dim + j] = val;
        }
    }

    input_vec[(dim/2)*dim + (dim/2)] = 100.0;

    // CFL Stability Condition (Courant-Friedrichs-Lewy)
    // double CFL = (double) alpha * (double) dt / (double) powf(dx, 2);
    // if(CFL > 0.25){
    //     cerr << "CFL: " << CFL << " Instabilità numerica: ridurre dt o aumentare dx." << endl;
    //     return 1;
    // }

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

    // ---------------------------------------------------------
    // I want to compute a single tile stencil: 32x32 bytes is 16x16 BFP32, but with a stencil 3x3 I need to consider
    // 1 BFP32 padding, so the real matrix is 15x15 BFP32.
    // ---------------------------------------------------------

    constexpr CoreCoord core = {0, 0};  
    DataFormat data_format = DataFormat::Float16;  // Convert it to 32
    MathFidelity math_fidelity = MathFidelity::HiFi4;

    // ---------------------------------------------------------
    // DRAM BUFFER CREATION: OFFCHIP GDDR6 MEMORY 12GB
    // ---------------------------------------------------------
    
    cout << "Creating DRAM buffers..." << endl;

    // Both input and output have the same configuration, in this case I have chosen Interleaved instead of Shreaded

    // deivce, size, page_size, buffer_type
    tt_metal::InterleavedBufferConfig dram_config{.device = device, 
                                                  .size = dram_buffer_size, 
                                                  .page_size = single_tile_size,  
                                                  .buffer_type = tt_metal::BufferType::DRAM};
    std::shared_ptr<tt::tt_metal::Buffer> input_dram_buffer = CreateBuffer(dram_config);
    std::shared_ptr<tt::tt_metal::Buffer> output_dram_buffer = CreateBuffer(dram_config);
    

    // ---------------------------------------------------------
    // SRAM BUFFER CREATION: TOTAL SHARED BETWEEN CORES 108MB HIGH-SPEED REGISTERS 
    // ---------------------------------------------------------

    cout << "Creating SRAM buffers..." << endl;

    uint32_t num_sram_tiles = num_tiles;

    uint32_t cb_0_index = CBIndex::c_0;  // 0
    // size, page_size
    CircularBufferConfig cb_0_config( single_tile_size * num_sram_tiles, 
                                          {{cb_0_index, data_format}}
    );
    cb_0_config.set_page_size(cb_0_index, dram_buffer_size);

    uint32_t cb_1_index = CBIndex::c_1;  // 1
    // size, page_size
    CircularBufferConfig cb_1_config( single_tile_size * num_sram_tiles, 
                                          {{cb_1_index, data_format}}
    );
    cb_1_config.set_page_size(cb_1_index, dram_buffer_size);

    uint32_t cb_2_index = CBIndex::c_2;  // 2
    // size, page_size
    CircularBufferConfig cb_2_config( single_tile_size * num_sram_tiles, 
                                          {{cb_2_index, data_format}}
    );
    cb_2_config.set_page_size(cb_2_index, dram_buffer_size);

    uint32_t cb_3_index = CBIndex::c_3;  // 3
    // size, page_size
    CircularBufferConfig cb_3_config( single_tile_size * num_sram_tiles, 
                                          {{cb_3_index, data_format}}
    );
    cb_3_config.set_page_size(cb_3_index, dram_buffer_size);

    uint32_t cb_4_index = CBIndex::c_4;  // 4
    // size, page_size
    CircularBufferConfig cb_4_config(single_tile_size * num_sram_tiles, 
                                          {{cb_4_index, data_format}}
    );
    cb_4_config.set_page_size(cb_4_index, dram_buffer_size);


    uint32_t cb_output_index = CBIndex::c_5;  // 5
    // size, page_size
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
    CBHandle cb_output = tt_metal::CreateCircularBuffer(program, core, cb_output_config);

    // ---------------------------------------------------------
    // KERNELS CREATION: We need a reader, writer and then a compute
    // ---------------------------------------------------------

    
    cout << "Creating kernels..." << endl;

    bool input_is_dram = input_dram_buffer->buffer_type() == tt_metal::BufferType::DRAM ? 1 : 0;
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

    PrintGrid(input_vec, dim);
    PrintGrid(output_vec, dim);
    cout << endl;

    EnqueueWriteBuffer(cq, input_dram_buffer, input_vec, false);  // E' UNA MEMCPY, NULLA DI PIÙ
    EnqueueWriteBuffer(cq, output_dram_buffer, output_vec, false);  // E' UNA MEMCPY, NULLA DI PIÙ          
    
    // ---------------------------------------------------------
    // SETUP RUNTIME ARGS: Set the runtime arguments for the kernels
    // ---------------------------------------------------------

    cout << "Setting up runtime arguments..." << endl;

    const uint32_t input_bank_id = 0;
    const uint32_t output_bank_id = 0;
    const float stencil_scalar = 0.25f; // SETTING IT TO uint32_t means to have an integer than? maybe

    // QUI SETTO RUNTIME ARGS PER READER
    tt_metal::SetRuntimeArgs( program, reader_kernel_id, core,
            {input_dram_buffer->address(), num_tiles, input_bank_id, input_dram_buffer->size()});
    // QUI SETTO RUNTIME ARGS PER WRITER
    tt_metal::SetRuntimeArgs(program, writer_kernel_id, core, 
            {output_dram_buffer->address(), num_tiles, output_bank_id, output_dram_buffer->size()});

    tt_metal::SetRuntimeArgs(program, stencil_kernel_id, core, 
            {num_tiles, stencil_scalar});
    


    // ---------------------------------------------------------
    // LAUNCH AND WAIT TERMINATION: Set the runtime arguments for the kernels
    // ---------------------------------------------------------

    cout << "Enqueueing kernels..." << endl;	
        
    EnqueueProgram(cq, program, false);
    // Wait Until Program Finishes
    EnqueueReadBuffer(cq, output_dram_buffer, output_vec, true); // Read the result from the device
    Finish(cq);
    printf("Core {0, 0} on Device 0 completed the task.\n");
    CloseDevice(device);

    // ---------------------------------------------------------
    // ---------------------------------------------------------
    // SAVE RESULTS AND CLEANING
    // ---------------------------------------------------------
    // ---------------------------------------------------------

    cout << "Saving results..." << endl;

    //saveGridCSV("result.csv", res_temp, dim, dim);

    PrintGrid(input_vec, dim);
    PrintGrid(output_vec, dim-2);

    free(input_vec);
    free(output_vec);

    return 0;
}
