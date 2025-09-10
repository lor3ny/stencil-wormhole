#pragma once

#include <tt-metalium/bfloat16.hpp>
#include <iostream>
#include <vector>
//#include <omp.h>

#include "utils.hpp"


#define N 10
#define M 10
#define K 3

using namespace std;

constexpr float test_input[100] = {
    1,2,3,4,5,6,7,8,9,10,
    11,12,13,14,15,16,17,18,19,20,
    21,22,23,24,25,26,27,28,29,30,
    31,32,33,34,35,36,37,38,39,40,
    41,42,43,44,45,46,47,48,49,50,
    51,52,53,54,55,56,57,58,59,60,
    61,62,63,64,65,66,67,68,69,70,
    71,72,73,74,75,76,77,78,79,80,
    81,82,83,84,85,86,87,88,89,90,
    91,92,93,94,95,96,97,98,99,100,
};
constexpr float est_stencil[9] = {
    0,1,0,
    1,4,1,
    0,1,1
};

//! This function is specialized for star 5-point stencil
void im2row_5p(vector<uint32_t>& in, vector<uint32_t>& out, uint32_t rows, uint32_t cols){
    
    bfloat16* in_bf16 = reinterpret_cast<bfloat16*>(in.data());
    bfloat16* out_bf16 = reinterpret_cast<bfloat16*>(out.data());
    
    // This functin can be parallelized
    int index = 0;
    for(int i = 1; i<rows-1; i++){ //rows
        for(int j = 1; j<cols-1; j++){ //cols

            // This phase ca be SIMDized
            out_bf16[index] = in_bf16[(i-1)*cols + j];
            out_bf16[index+1] = in_bf16[i*cols + (j-1)];
            out_bf16[index+2] = in_bf16[i*cols + j];
            out_bf16[index+3] = in_bf16[i*cols + (j+1)];
            out_bf16[index+4] = in_bf16[(i+1)*cols + j];
            index += 5;
        }
    }
}


uint32_t align_vector_size(vector<uint32_t>& in, size_t starting_size, size_t single_tile_size){

    uint32_t new_size = starting_size;

    if(new_size % single_tile_size != 0){
        uint32_t align = 1;
        while ((new_size + align) % single_tile_size != 0){
            align += 1;
        }    
        new_size += align;
    }
    uint32_t new_count = new_size / sizeof(uint32_t);
    in.resize(new_count, 0.0f);

    return new_size;
}


// it is implemented considering star stencils, not squared ones
vector<uint32_t> pad_with_zeros(vector<uint32_t>& in, int rows, int cols, int stencil_order){

    size_t pad_size = stencil_order * 2;

    size_t new_rows = rows + pad_size;
    size_t new_cols = cols + pad_size;
    std::vector<uint32_t> out(new_rows * new_cols);
    create_constant_vector_of_bfloat16(new_rows * new_cols * sizeof(bfloat16), 0.0f);

    for (size_t r = 0; r < rows; ++r) {
        // Destination row start (skip first row + padding col)
        bfloat16* out_bf16 = reinterpret_cast<bfloat16*>(out.data());
        bfloat16* in_bf16 = reinterpret_cast<bfloat16*>(in.data());

        bfloat16* dest = out_bf16 + (r + stencil_order) * new_cols + stencil_order;
        const bfloat16* src = in_bf16 + r * cols;

        std::memcpy(dest, src, cols * sizeof(bfloat16));
    }

    return out;
}

//! Tester function
void matVecMul(
    const std::vector<float>& matrix,
    const float* vec,
    float* result,
    size_t rows,
    size_t cols
) {
    for (size_t i = 0; i < rows; ++i) {
        result[i] = 0.0f;
        for (size_t j = 0; j < cols; ++j) {
            result[i] += matrix[i * cols + j] * vec[j];
        }
    }
}

