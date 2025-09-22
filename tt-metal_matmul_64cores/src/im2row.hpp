#pragma once


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
// in is 10x10 and out is 64x32,
void stencil2vec_5p(vector<bfloat16>& in, vector<bfloat16>& out, int rows, int cols){

    ZoneScoped;
    
    // This functin can be parallelized
    int index = 0;
    int total_size = (rows-1) * (cols-1);
    int i, j;

    // for(int flat = cols; flat < total_size; flat++){
    //     i = flat / cols;
    //     j = flat % cols;

    //     out[index] = in[(i-1)*cols + j];
    //     out[index+1] = in[i*cols + (j-1)];
    //     out[index+2] = in[i*cols + j];
    //     out[index+3] = in[i*cols + (j+1)];
    //     out[index+4] = in[(i+1)*cols + j];
    //     for(int k = index+5; k<index+31; k++){
    //         out[index+k] = 0; //! padding
    //     }
    //     index += 32;
    // }

    for(i = 1; i<rows-1; i++){ // rows from 1 to 9
        for(j = 1; j<cols-1; j++){ // cols

            // This phase ca be SIMDized
            out[index] = in[(i-1)*cols + j];
            out[index+1] = in[i*cols + (j-1)];
            out[index+2] = in[i*cols + j];
            out[index+3] = in[i*cols + (j+1)];
            out[index+4] = in[(i+1)*cols + j];
            for(int k = index+5; k<index+31; k++){
                out[k] = 0; //! padding
            }
            index += 32;
        }
    }
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

