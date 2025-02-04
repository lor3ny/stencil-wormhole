#include <iostream>
#include <cmath>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip> // For setprecision


using namespace std;

// - The matrix have to be diagonal dominant and squared
//- Solver for strictly diagonally dominant system of linear equations with n=3

void readCSV(const string& filename, float* buff, int dim) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return; 
    }

    string line;
    int i = 1;
    while (getline(file, line)) {
        stringstream ss(line);
        string value;      

        int j = 1;
        while (getline(ss, value, ' ')) {
            buff[i*dim + j] = stof(value);
            j++;
        }
        i++;
    }

    file.close();
}


void printBuff(float* mat, int dim){
    for(int i = 1; i<dim-1; i++){
        for(int j = 1; j<dim-1; j++){
            cout << fixed << setprecision(2) << mat[i*dim + j] << " ";
        }   
        cout << endl;
    }
}

int main(){


    cout << "STARTING JACOBI" << endl;

    // Fare il parsing da file e testare con sistemi molto più grandi

    int N = 5+2;
    int iterations_count = 100;

    float *b = (float*) malloc(sizeof(float) * N);
    float *a = (float*) malloc(sizeof(float) * N * N);
    float *c = (float*) malloc(sizeof(float) * N * N);
    float *err = (float*) malloc(sizeof(float) * N);

    readCSV("inputs/basic_input.csv", a, N);
    cout << "INPUT:" << endl;
    printBuff(a, N);
    cout << endl;

    auto start = chrono::high_resolution_clock::now();

	int m = 0;
    while(m<iterations_count){

        // Application of the stencil
        for(int i = 1; i<N-1; i++){
            for(int j = 1; j<N-1; j++){
                c[i*N+j] = (-4*(a[i*N+j]) + 1*(a[(i-1)*N+j] + a[(i+1)*N+j] + a[i*N+(j-1)] + a[i*N+(j+1)]));
            }
        }

        // Swap use the new result as new coefficent matrix
        float* temp = a;
        a = c;
        c = temp;

        m++;
    }

    auto end = chrono::high_resolution_clock::now();


    long long duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "OUTPUT:" << endl;
    printBuff(a, N);
    cout << "Execution time: " << fixed << setprecision(9) << duration / (float) 1000000 << " seconds" << endl;

    free(a);
    free(c);
    free(b);
    free(err);
}