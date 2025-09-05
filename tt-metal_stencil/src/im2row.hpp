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

template<typename T>
void im2row(vector<T>& in, vector<T>& out, int stencil_order);

vector<uint32_t> pad_with_zeros(vector<uint32_t>& in, int rows, int cols,  int stencil_order);

void matVecMul(
    const std::vector<float>& matrix,
    const float* vec,
    float* result,
    size_t rows,
    size_t cols
);