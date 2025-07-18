#include <iostream>
#include <vector>
#define N 10
#define M 10

#define K 3

using namespace std;


float input[100] = {
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
float stencil[9] = {
    0,1,0,
    1,4,1,
    0,1,1
};

void im2row(float* in_A, vector<float>& out_A, int stencil_order){
    
    int new_rows = (N-1) * (M-1);
    int new_cols = K * K;

    for(int i = stencil_order; i<N-stencil_order; i++){ //rows
        for(int j = stencil_order; j<M-stencil_order; j++){ //cols
            for(int k = -stencil_order; k<=stencil_order; k++){
                for(int h = -stencil_order; h<=stencil_order; h++){
                    out_A.push_back(input[(i+k)*M + j+h]);
                }
            }
        }
    }
}

void printMat(float* buffer, size_t rows, size_t cols) {
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            std::cout << buffer[i*cols + j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << std::flush;
}

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


int main(){

    float *conv_stencil = (float*) malloc(sizeof(float) * K * K); 
    std::vector<float> conv_input;

    printMat(input, N, M);

    if(K % 2 == 0){
        cerr << "ERROR: Stencil size must be odd!" << endl;
        return EXIT_FAILURE;
    }
    int stencil_order = (K-1)/2;
    
    im2row(input, conv_input, stencil_order);

    int new_cols = K * K;
    int new_rows = (N-stencil_order*2) * (M-stencil_order*2);

    printMat(conv_input.data(), conv_input.size()/(K*K), K*K);



    float *result = (float*) malloc(sizeof(float) * new_cols * new_rows); 
    matVecMul(conv_input, stencil, result, new_rows, new_cols);
    // NOW MATRIX * VECTOR


    // Inserire funzioni di padding
    // Creare una struttura che possia visualizzare tutto meglio? Forse

    printMat(result, N-2, M-2);

    free(result);
    free(conv_stencil);
}