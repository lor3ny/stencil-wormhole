#include <iostream>
#include <cmath>

using namespace std;


void UpdateTemperature(double* temp, int dim, double dt, double alpha){
    // Update temperature
    for(int i = 1; i < dim - 1; i++){
        for(int j = 1; j < dim - 1; j++){
            temp[i*dim+j] = temp[i*dim+j] + alpha * dt * (temp[(i+1)*dim+j] + temp[(i-1)*dim+j] + temp[i*dim+(j+1)] + temp[i*dim+(j-1)] - 4 * temp[i*dim+j]);
        }
    }

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

int main(){

    // SIMULATION PARAMETERS
    int dim = 100; //GRID SIZE
    double lx = 1, ly = 1;   //DOMAIN SIZE
    float max_time = 0.2;

    double dx = lx / (double) (dim - 1);
    double dy = ly / (dim - 1);
    double dt = 0.0001;  // Time step
    double alpha = 0.1;  // Coefficient of diffusion

    //TEMPERATURE MATRIX
    double* temp = (double*) malloc(dim * dim * sizeof(double));
    InitializeGrid(temp, dim);

    temp[(dim/2)*dim + (dim/2)] = 100.0;

    // CFL Stability Condition (Courant-Friedrichs-Lewy)
    double CFL = alpha * dt / (double) powf(dx, 2);
    if(CFL > 0.25){
        cerr << "CFL: " << CFL << " Instabilità numerica: ridurre dt o aumentare dx." << endl;
        return 1;
    }

    double start_time = 0.0;
    double time_elapsed = start_time;
    while(time_elapsed < max_time){
        UpdateTemperature(temp, dim, dt, alpha);
        time_elapsed += dt;
    }

    cout << "Simulation completed." << endl;

    PrintGrid(temp, dim);

    free(temp);
    return 0;
}

