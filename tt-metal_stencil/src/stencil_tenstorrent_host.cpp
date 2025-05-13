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
#include <tuple>

using namespace tt;
using namespace tt::tt_metal;
using namespace std;

#define TILE_WIDTH 32
#define TILE_HEIGHT 32
#define TILE_VALUES_BFP16 512
#define TILE_VALUES_UINT32 256

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

std::pair<bfloat16, bfloat16> extract_bfloat16_pair(uint32_t packed) {
    uint16_t low = static_cast<uint16_t>(packed & 0xFFFF);           // lower 16 bits
    uint16_t high = static_cast<uint16_t>((packed >> 16) & 0xFFFF);  // upper 16 bits

    return { bfloat16(high), bfloat16(low) };
}

// PRINT WELL ONLY FOR ONE BLOCK
void TT_printMatrix(vector<vector<uint32_t>> matrix, size_t rows, size_t cols) {
    for(int i = 0; i<rows; i++){
        for(int j = 0; j<cols; j++){
            if (j % 16 == 0) {std::cout << "\n";}

            std::pair<bfloat16, bfloat16> two_bfp16 = extract_bfloat16_pair(matrix[i][j]);
            std::cout << two_bfp16.first.to_float() << " ";
            std::cout << two_bfp16.second.to_float() << " ";
        }
        std::cout << "\n";
    }
    std::cout << std::flush;
}


void printBlockedMatrix(const std::vector<vector<uint32_t>> data, int matrixRows, int matrixCols, int blockRows, int blockCols) {
    int blocksPerRow = matrixCols / blockCols;
    int blocksPerCol = matrixRows / blockRows;

    for (int i = 0; i < matrixRows; ++i) {
        int blockRow = i / blockRows;
        int innerRow = i % blockRows;

        for (int j = 0; j < matrixCols; ++j) {
            for (int j = 0; j < matrixCols; ++j) {
            int blockCol = j / blockCols;
            int innerCol = j % blockCols;

            int blockIndex = blockRow * blocksPerRow + blockCol;
            int indexInBlock = innerRow * blockCols + innerCol;

            std::cout << data[blockIndex][indexInBlock] << " ";
        }
        }
        std::cout << std::endl;
    }
}


void fromAbstractionToBuffer(vector<vector<uint32_t>>& matrix, uint32_t* buffer) {
    for(int i =0; i<matrix.size(); i++){
        for(int j =0; j<matrix[i].size(); j++){
            buffer[i*matrix[i].size() + j] = matrix[i][j];
        }
    }
}

void fromBufferToAbstraction(uint32_t* buffer, vector<vector<uint32_t>>& matrix, int rows, int cols) {
    for(int i = 0; i<rows; i++){
        for(int j = 0; j<cols; j++){
            matrix[i][j] =  buffer[i*cols + j];
        }
    }
}



int main(int argc, char** argv) {

    // Create the device and the program
    int device_id = 0;
    IDevice* device = CreateDevice(device_id);
    CommandQueue& cq = device->command_queue(); // Take the command_queue from the device created
    Program program = CreateProgram();

    // Blocks and tiles now are the same thing! 
    constexpr uint32_t single_tile_size = TILE_WIDTH*TILE_HEIGHT; 
    const auto compute_with_storage_grid_size = device->compute_with_storage_grid_size();

    constexpr uint32_t row_blocks = 1;  //compute_with_storage_grid_size.x;
    constexpr uint32_t col_blocks = 1;  //compute_with_storage_grid_size.y;
    const uint32_t num_cores_x = row_blocks; //compute_with_storage_grid_size.x;
    const uint32_t num_cores_y = col_blocks; 
    const uint32_t num_cores_total = num_cores_x*num_cores_y;

    const uint16_t num_sram_tiles = num_cores_total;
    const uint16_t num_tiles = num_cores_total;


    auto all_device_cores = CoreRange({0, 0}, {num_cores_x - 1, num_cores_y - 1});

    //cout << "CORES X: " << compute_with_storage_grid_size.x << " CORES Y: " << compute_with_storage_grid_size.x <<endl;
    //cout << "PADDED ROWS: " << padded_rows << " PADDED COLS: " << padded_cols << endl;

    const uint32_t dram_buffer_size = row_blocks * col_blocks * single_tile_size; 

    // NECESSARY ONLY FOR INTERNAL ITERATIONS!

    // ORA STO INIZILIZANDO LE MATRICI A VALORI NON UTILI, QUINDI UTILIZZO TILE_VALUES_UINT32

    vector<vector<uint32_t>> h_input_mat(row_blocks*col_blocks, create_constant_vector_of_bfloat16(single_tile_size, 1.0f)); //h_submat_up(row_blocks*col_blocks, vector<uint32_t>(TILE_VALUES_UINT32, 1)
    vector<vector<uint32_t>> h_output_mat(row_blocks*col_blocks, create_constant_vector_of_bfloat16(single_tile_size, 0.0f));
    vector<vector<uint32_t>> h_scalar_mat(row_blocks*col_blocks, create_constant_vector_of_bfloat16(single_tile_size, 0.25f));

    //vector<uint32_t> h_padding_up(col_blocks * std::sqrt(TILE_VALUES_COUNT/2), 1);  // (TILE_VALUES_COUNT/2) is the number of uint32_t, that corresponds to 2 * bfp16
    //vector<uint32_t> h_padding_down(col_blocks * std::sqrt(TILE_VALUES_COUNT/2), 1); 
    //vector<uint32_t> h_padding_left(row_blocks * std::sqrt(TILE_VALUES_COUNT/2), 1); 
    //vector<uint32_t> h_padding_right(row_blocks * std::sqrt(TILE_VALUES_COUNT/2), 1); 

    const uint32_t dram_input_buffer_size = single_tile_size * row_blocks * col_blocks;
    const uint32_t dram_padding_v_buffer_size = row_blocks * TILE_HEIGHT; 
    const uint32_t dram_padding_h_buffer_size = col_blocks * TILE_WIDTH; 

    vector<vector<uint32_t>> h_submat_up(row_blocks*col_blocks, create_constant_vector_of_bfloat16(single_tile_size, 1.0f));
    vector<vector<uint32_t>> h_submat_down(row_blocks*col_blocks, create_constant_vector_of_bfloat16(single_tile_size, 1.0f));
    vector<vector<uint32_t>> h_submat_left(row_blocks*col_blocks, create_constant_vector_of_bfloat16(single_tile_size, 1.0f)); 
    vector<vector<uint32_t>> h_submat_right(row_blocks*col_blocks, create_constant_vector_of_bfloat16(single_tile_size, 1.0f));


    std::cout << "Input CENTER: " << std::endl;
    TT_printMatrix(h_input_mat, h_input_mat.size(), h_input_mat[0].size());
    std::cout << "Input UP: " << std::endl;
    TT_printMatrix(h_submat_up, h_submat_up.size(), h_submat_up[0].size());
    std::cout << "Input DOWN: " << std::endl;
    TT_printMatrix(h_submat_down, h_submat_up.size(), h_submat_up[0].size());
    std::cout << "Input LEFT: " << std::endl;
    TT_printMatrix(h_submat_left, h_submat_left.size(), h_submat_left[0].size());
    std::cout << "Input RIGHT: " << std::endl;
    TT_printMatrix(h_submat_right, h_submat_right.size(), h_submat_right[0].size());

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
    tt_metal::InterleavedBufferConfig d_dram_input_config{
        .device = device, 
        .size = dram_input_buffer_size, 
        .page_size = single_tile_size,  
        .buffer_type = tt_metal::BufferType::DRAM
    };

    tt_metal::InterleavedBufferConfig d_dram_padding_v_config{
        .device = device, 
        .size = dram_padding_v_buffer_size, 
        .page_size = single_tile_size,  
        .buffer_type = tt_metal::BufferType::DRAM
    };

    tt_metal::InterleavedBufferConfig d_dram_padding_h_config{
        .device = device, 
        .size = dram_padding_h_buffer_size, 
        .page_size = single_tile_size,  
        .buffer_type = tt_metal::BufferType::DRAM
    };

    std::shared_ptr<tt::tt_metal::Buffer> d_input_buffer = CreateBuffer(d_dram_input_config);
    std::shared_ptr<tt::tt_metal::Buffer> d_submat_up_buffer = CreateBuffer(d_dram_input_config);
    std::shared_ptr<tt::tt_metal::Buffer> d_submat_down_buffer = CreateBuffer(d_dram_input_config);
    std::shared_ptr<tt::tt_metal::Buffer> d_submat_left_buffer = CreateBuffer(d_dram_input_config);
    std::shared_ptr<tt::tt_metal::Buffer> d_submat_right_buffer = CreateBuffer(d_dram_input_config);

    std::shared_ptr<tt::tt_metal::Buffer> d_scalar_buffer = CreateBuffer(d_dram_input_config);
    std::shared_ptr<tt::tt_metal::Buffer> d_output_buffer = CreateBuffer(d_dram_input_config);
    

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


    // ---------------------------------------------------------
    // KERNELS CREATION: We need a reader, writer and then a compute
    // ---------------------------------------------------------

    
    cout << "Creating kernels..." << endl;

    bool input_is_dram = d_input_buffer->buffer_type() == tt_metal::BufferType::DRAM ? 1 : 0;
    std::vector<uint32_t> reader_compile_time_args = {(uint32_t)input_is_dram};

    bool output_is_dram = d_output_buffer->buffer_type() == tt_metal::BufferType::DRAM ? 1 : 0;
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


    uint32_t* h_input_buffer = (uint32_t*)malloc(dram_input_buffer_size);
    uint32_t* h_output_buffer = (uint32_t*)malloc(dram_input_buffer_size);
    uint32_t* h_scalar_buffer = (uint32_t*)malloc(dram_input_buffer_size);
    uint32_t* h_submat_up_buffer = (uint32_t*)malloc(dram_input_buffer_size);
    uint32_t* h_submat_down_buffer = (uint32_t*)malloc(dram_input_buffer_size);
    uint32_t* h_submat_left_buffer = (uint32_t*)malloc(dram_input_buffer_size);
    uint32_t* h_submat_right_buffer = (uint32_t*)malloc(dram_input_buffer_size);
    fromAbstractionToBuffer(h_input_mat, h_input_buffer);
    fromAbstractionToBuffer(h_output_mat, h_output_buffer);
    fromAbstractionToBuffer(h_scalar_mat, h_scalar_buffer);
    fromAbstractionToBuffer(h_submat_up, h_submat_up_buffer);
    fromAbstractionToBuffer(h_submat_down, h_submat_down_buffer);
    fromAbstractionToBuffer(h_submat_left, h_submat_left_buffer);
    fromAbstractionToBuffer(h_submat_right, h_submat_right_buffer);

    EnqueueWriteBuffer(cq, d_input_buffer, h_input_mat, false);   
    EnqueueWriteBuffer(cq, d_output_buffer, h_output_mat, false); 
    EnqueueWriteBuffer(cq, d_scalar_buffer, h_scalar_mat, false);  
    EnqueueWriteBuffer(cq, d_submat_up_buffer, h_submat_up, false); 
    EnqueueWriteBuffer(cq, d_submat_down_buffer, h_submat_down, false);  
    EnqueueWriteBuffer(cq, d_submat_left_buffer, h_submat_left, false);  
    EnqueueWriteBuffer(cq, d_submat_right_buffer, h_submat_right, false);   
    
    // ---------------------------------------------------------
    // SETUP RUNTIME ARGS: Set the runtime arguments for the kernels
    // ---------------------------------------------------------

    cout << "Setting up runtime arguments..." << endl;

    bool row_major = false;

    for(uint32_t i = 0; i < num_cores_total; i++) {

        CoreCoord core = {i / num_cores_y, i % num_cores_y};
        uint32_t my_tile_index = i;

        tt_metal::SetRuntimeArgs( program, reader_kernel_id, core, {
            d_input_buffer->address(), 
            d_submat_up_buffer->address(), 
            d_submat_down_buffer->address(), 
            d_submat_left_buffer->address(), 
            d_submat_right_buffer->address(), 
            d_scalar_buffer->address(), 
            my_tile_index,
        });
    
        tt_metal::SetRuntimeArgs(program, writer_kernel_id, core, {
            d_output_buffer->address(), 
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
    EnqueueReadBuffer(cq, d_output_buffer, h_output_buffer, true); // Read the result from the device
    Finish(cq);
    auto end = std::chrono::high_resolution_clock::now();
    CloseDevice(device);

    // ---------------------------------------------------------
    // ---------------------------------------------------------
    // SAVE RESULTS AND CLEANING
    // ---------------------------------------------------------
    // ---------------------------------------------------------

    std::cout << "Output: " << std::endl;
    fromBufferToAbstraction(h_output_buffer, h_output_mat, row_blocks*col_blocks, TILE_VALUES_BFP16);
    TT_printMatrix(h_output_mat, h_output_mat.size(), h_output_mat[0].size());

    free(h_submat_up_buffer);
    free(h_submat_down_buffer);
    free(h_submat_left_buffer);
    free(h_submat_right_buffer);
    free(h_input_buffer);
    free(h_output_buffer);
    free(h_scalar_buffer);

    // std::chrono::duration<double> duration = end - start;
    // std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;

    return EXIT_SUCCESS;
}
