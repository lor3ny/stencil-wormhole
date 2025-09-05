#pragma once

#include <iostream>
#include <tt-metalium/bfloat16.hpp>


/*--------------------------------

    GENERIC UTILITIES

----------------------------------*/



//template<typename T>
inline void printMat(std::vector<uint32_t> matrix, int rows, int cols) {
    bfloat16* a_bf16 = reinterpret_cast<bfloat16*>(matrix.data());
    for(int i =0; i<rows; i++){
        for(int j =0; j<cols; j++){
            std::cout << a_bf16[i*cols + j].to_float() << " ";
        }
        std::cout << "\n";
    }
    std::cout << std::flush;
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
            std::cout << " " << grid[i*dim + j] << " ";
        }
        std::cout << std::endl;
    }
}

//! Not it isn't used, will be checked if needed
// inline void saveGridCSV(const std::string& filename, double* grid, int dim_x, int dim_y) {
//     std::ofstream file(filename);
//     if (!file) {
//         std::cerr << "Errore nell'apertura del file" << std::endl;
//         return;
//     }
//     file << dim_x << " " << dim_y << endl;
//     for (size_t i=0; i<dim_x; ++i) {
//         for (size_t j = 0; j < dim_y; ++j) {
//             file << grid[i*dim_x + j] << (j + 1 == dim_y ? "\n" : " ");
//         }
//     }
//     file.close();
// }

/*--------------------------------

    TENSTORRENT UTILITIES

----------------------------------*/


// IMPORTANT
// Tenstorrent works with BFP16, TT-Metalium requires uint32_t buffers, and packs on it two BFP16.
// So it packs on every uint32_t cel two bfloat16 values. Checks printf("Result = %x\n", result_vec[0]);
// You will see the two copies of the same HEX. 

// void tt_printMatrix(uint32_t* matrix, size_t rows, size_t cols) {
//     bfloat16* a_bf16 = reinterpret_cast<bfloat16*>(matrix);

//     for(int i =0; i<rows; i++){
//         for(int j =0; j<cols; j++){
//             std::cout << a_bf16[i*cols + j].to_float() << " ";
//         }
//         std::cout << "\n";
//     }
//     std::cout << std::flush;
// }