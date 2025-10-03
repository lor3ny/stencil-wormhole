#pragma once


#include "utils.hpp"


#define N 10
#define M 10
#define K 3

using namespace std;


//! This function is specialized for star 5-point stencil
// in is 10x10 and out is 64x32,
void extract_submats_5p(
    vector<bfloat16>& in,
    vector<bfloat16>& up,
    vector<bfloat16>& left,
    vector<bfloat16>& right, 
    vector<bfloat16>& down,  
    int rows, 
    int cols, 
    int cols_pad) {
    
    int flat, i, j;

    // #pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        bfloat16* out_up    = &up[i * cols];
        bfloat16* out_left  = &left[i * cols];
        bfloat16* out_right = &right[i * cols];
        bfloat16* out_down  = &down[i * cols];

        bfloat16* in_center = &in[(i + 1) * cols_pad + 1];  // aligned with (j+1)

        for (int j = 0; j < cols; j++) {
            out_up[j]    = in_center[j - cols_pad];  // UP
            out_left[j]  = in_center[j - 1];         // LEFT
            out_right[j] = in_center[j + 1];         // RIGHT
            out_down[j]  = in_center[j + cols_pad];  // DOWN
        }
    }

    // for(i=0; i<rows; i++){
    //     for(j=0; j<cols; j++){
    //         up[i*cols + j] = in[i*cols_pad + (j+1)]; //UP
    //         left[i*cols + j] = in[(i+1)*cols_pad + j]; //LEFT
    //         right[i*cols + j] = in[(i+1)*cols_pad + (j+2)]; //RIGHT
    //         down[i*cols + j] = in[(i+2)*cols_pad + (j+1)]; //DOWN
    //     }
    // }
}


void vec2stencil_5p(vector<bfloat16>& in, vector<bfloat16>& out, int tile_height, int n_tiles){
    int j;
    int picker = 0;
    for(j = 0; j<tile_height*n_tiles; j++){
        out[j] = in[picker];
        picker+=32;
    }
}

// it is implemented considering star stencils, not squared ones
void pad_with_zeros(vector<bfloat16>& in,  vector<bfloat16>& out, int rows, int cols, int pad_size){

    size_t new_rows = rows + pad_size*2;
    size_t new_cols = cols + pad_size*2;


    for (size_t r = 0; r < rows; ++r) {
        // Destination row start (skip first row + padding col)
        bfloat16* dest = out.data() + (r + pad_size) * new_cols + pad_size;
        const bfloat16* src = in.data() + r * cols;

        std::memcpy(dest, src, cols * sizeof(bfloat16));
    }
}


uint32_t align_vector_size(vector<bfloat16>& in, size_t starting_size, size_t single_tile_size){

    uint32_t new_size = starting_size;

    if(new_size % single_tile_size != 0){
        uint32_t align = 1;
        while ((new_size + align) % single_tile_size != 0){
            align += 1;
        }    
        new_size += align;
    }
    uint32_t new_count = new_size / sizeof(bfloat16);
    in.resize(new_count, 0.0f);

    return new_size;
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

