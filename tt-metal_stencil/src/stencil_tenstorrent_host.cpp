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

inline void PrintGrid(double *grid, int dim){
    int i, j;
    for(i = 0; i < dim; ++i){
        for(j = 0; j < dim; ++j){
            cout << " " << grid[i*dim + j] << " ";
        }
        cout << endl;
    }
}


    // ---------------------------------------------------------
    // ---------------------------------------------------------
    // HEAT PROPAGATION STENCIL SETUP
    // ---------------------------------------------------------
    // ---------------------------------------------------------

    cout << "Setting up heat propagation stencil..." << endl;

    int dim = 1000; // Grid Size
    double lx = 10.0, ly = 10.0;   // Domain Size
    float max_time = 1;

    double dx = lx / (double) (dim - 1);
    double dy = ly / (double) (dim - 1);
    double dt = 0.0001;  // Time step
    double alpha = 0.1;  // Coefficient of diffusion

    double* temp = (double*) malloc(dim * dim * sizeof(double));
    double* res_temp = (double*) malloc(dim * dim * sizeof(double));
    if (!temp || !res_temp) {
        cerr << "Errore: allocazione di memoria fallita!" << endl;
        free(temp);
        free(res_temp);
        return 1;
    }
    memset(temp, 0, dim * dim * sizeof(double));
    memset(res_temp, 0, dim * dim * sizeof(double));

    temp[(dim/2)*dim + (dim/2)] = 100.0;
    res_temp[(dim/2)*dim + (dim/2)] = 100.0;

    // CFL Stability Condition (Courant-Friedrichs-Lewy)
    double CFL = (double) alpha * (double) dt / (double) powf(dx, 2);
    if(CFL > 0.25){
        cerr << "CFL: " << CFL << " Instabilità numerica: ridurre dt o aumentare dx." << endl;
        return 1;
    }

int main(int argc, char** argv) {

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
    constexpr uint32_t single_tile_size = 4; // 1KiB for every tile
    constexpr uint32_t num_tiles = 4; // Number of tiles
    constexpr uint32_t dram_buffer_size = single_tile_size * num_tiles; // Total size of the DRAM buffer: 64 KiB 
    DataFormat data_format = DataFormat::Float32;
    MathFidelity math_fidelity = MathFidelity::HiFi4;

    // ---------------------------------------------------------
    // DRAM BUFFER CREATION: OFFCHIP GDDR6 MEMORY 12GB
    // ---------------------------------------------------------
    
    cout << "Creating DRAM buffers..." << endl;

    // Both input and output have the same configuration, in this case I have chosen Interleaved instead of Shreaded

    // deivce, size, page_size, buffer_type
    tt_metal::InterleavedBufferConfig dram_config{.device = device, 
                                                  .size = dram_buffer_size, 
                                                  .page_size = dram_buffer_size,  
                                                  .buffer_type = tt_metal::BufferType::DRAM};
    std::shared_ptr<tt::tt_metal::Buffer> input_dram_buffer = CreateBuffer(dram_config);
    std::shared_ptr<tt::tt_metal::Buffer> output_dram_buffer = CreateBuffer(dram_config);
    

    // ---------------------------------------------------------
    // SRAM BUFFER CREATION: TOTAL SHARED BETWEEN CORES 108MB HIGH-SPEED REGISTERS 
    // ---------------------------------------------------------

    cout << "Creating SRAM buffers..." << endl;

    uint32_t num_sram_tiles = 4;
    uint32_t cb_input_index = CBIndex::c_0;  // 0
    // size, page_size
    CircularBufferConfig cb_input_config( single_tile_size * num_sram_tiles, 
                                          {{cb_input_index, data_format}}
    );
    cb_input_config.set_page_size(cb_input_index, dram_buffer_size);

    uint32_t cb_output_index = CBIndex::c_16;  // 16
    // size, page_size
    CircularBufferConfig cb_output_config( single_tile_size * num_sram_tiles, 
                                           {{cb_output_index, data_format}}
    );
    cb_output_config.set_page_size(cb_output_index, dram_buffer_size);

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
            .compile_args = compute_args
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

    cout << dram_buffer_size << endl;
    cout << dram_buffer_size/4 << endl;

    vector<uint32_t> input_vec(dram_buffer_size/4);
    vector<uint32_t> output_vec(dram_buffer_size/4);
    memset(input_vec.data(), 1, dram_buffer_size);
    memset(output_vec.data(), 0, dram_buffer_size);

    cout << "Input buffer before: ";
    for(uint32_t i = 0; i < dram_buffer_size/4; ++i){
        cout << input_vec[i] << " ";
    }
    cout << endl;
    
    cout << "Output buffer before: ";
    for(uint32_t i = 0; i < dram_buffer_size/4; ++i){
        cout << output_vec[i] << " ";
    }
    cout << endl;

    EnqueueWriteBuffer(cq, input_dram_buffer, input_vec.data(), false);  // E' UNA MEMCPY, NULLA DI PIÙ
    EnqueueWriteBuffer(cq, output_dram_buffer, output_vec.data(), false);  // E' UNA MEMCPY, NULLA DI PIÙ          
    
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

    
    tt_metal::SetRuntimeArgs(program, stencil_kernel_id, core, {});
    


    // ---------------------------------------------------------
    // LAUNCH AND WAIT TERMINATION: Set the runtime arguments for the kernels
    // ---------------------------------------------------------

    cout << "Enqueueing kernels..." << endl;	
        
    EnqueueProgram(cq, program, false);
    // Wait Until Program Finishes
    EnqueueReadBuffer(cq, output_dram_buffer, output_vec.data(), true); // Read the result from the device
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

    cout << "Input buffer before: ";
    for(uint32_t i = 0; i < dram_buffer_size/4; ++i){
        cout << input_vec[i] << " ";
    }
    cout << endl;
    
    cout << "Output buffer before: ";
    for(uint32_t i = 0; i < dram_buffer_size/4; ++i){
        cout << output_vec[i] << " ";
    }
    cout << endl;

    free(res_temp);
    free(temp);

    return 0;
}
