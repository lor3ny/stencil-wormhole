#include <iostream>
#include <cmath>
#include <chrono>


using namespace std;

// - The matrix have to be diagonal dominant and squared
//- Solver for strictly diagonally dominant system of linear equations with n=3

int main(){

	// n is the number of coefficients 
	// iterations_count is the number of iterations
	// a[][] are the coefficents row-wise
	// b[] are the constants column-wise
	// xo[] are the initial values
	// e is the absolute error	

    cout << "STARTING JACOBI" << endl;

    // Fare il parsing da file e testare con sistemi molto più grandi

    int N = 3;
    int iterations_count = 1000000;

    float *b = (float*) malloc(sizeof(float) * N);
    float *a = (float*) malloc(sizeof(float) * N * N);
    float *c = (float*) malloc(sizeof(float) * N);
    float *err = (float*) malloc(sizeof(float) * N);
    float *xo = (float*) malloc(sizeof(float) * N);
    float *xn = (float*) malloc(sizeof(float) * N);


    a[0 * N + 0] = 7; a[0 * N + 1] = -3; a[0 * N + 2] = -4;
    a[1 * N + 0] = -3; a[1 * N + 1] = 6; a[1 * N + 2] = -2;
    a[2 * N + 0] = -4; a[2 * N + 1] = -2; a[2 * N + 2] = 11;

    b[0] = -11; b[1] = 3; b[2] = 25;

    xo[0] = 0; xo[1] = 0; xo[2] = 0;

    auto start = chrono::high_resolution_clock::now();

	// compute the new costants based on x0 and coefficients
	int m = 0;
    while(m<iterations_count){
        for(int i = 0; i<N; i++){
            c[i] = b[i];
            for(int j = 0; j<N; j++){
                if( i != j ){
                    c[i] = c[i] - a[i*N+j] * xo[j];
                }
            }
        }


        // compute the new starting values
        for(int i = 0; i<N; i++){
            xo[i] = c[i] / a[i*N+i];
        }

        // Store old values to the new array, and restart the iteration
        m++;
        if(m < iterations_count){
            for(int i = 0; i<N; i++){
                xn[i] = xo[i];
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();

	// Compute errors, but the error is computed considering the value of the next iteration not the exact value
	for(int i = 0; i<N; i++){
        err[i] = abs(xn[i] - xo[i]);
    }


    cout << "x1:" << xn[0] << " x2:" << xn[1] << " x2:" << xn[2] << endl;
    long long duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "Execution time: " << duration << " microseconds" << endl;



    free(a);
    free(xo);
    free(xn);
    free(c);
    free(b);
    free(err);
}