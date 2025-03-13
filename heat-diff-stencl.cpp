#include <iostream>
#include <cmath>

using namespace std;


void UpdateTemperature(float* temp, int dim, float dt, float alpha){
    // Update temperature
    for(int i = 1; i < dim - 1; i++){
        for(int j = 1; j < dim - 1; j++){
            temp[i*dim+j] = temp[i*dim+j] + alpha * dt * (temp[(i+1)*dim+j] + temp[(i-1)*dim+j] + temp[i*dim+(j+1)] + temp[i*dim+(j-1)] - 4 * temp[i*dim+j]);
        }
    }

}

inline void InitializeGrid(int *grid, int dim){
    int i, j;
    for(i = 0; i < dim*dim; ++i){
        for(j = 0; j < dim; ++j){
            grid[i*dim +j] = 0.0;
        }
    }
}

int main(){

    // SIMULATION PARAMETERS
    int dim = 100; //GRID SIZE
    int lx = 1, ly = 1;   //DOMAIN SIZE
    float max_time = 0.2;

    float dx = lx / (dim - 1);
    float dy = ly / (dim - 1);
    float dt = 0.0001;  // Time step
    float alpha = 0.1;  // Coefficient of diffusion

    //TEMPERATURE MATRIX
    float* temp = (float*) malloc(dim * dim * sizeof(float));

    // CFL Stability Condition (Courant-Friedrichs-Lewy)
    float CFL = alpha * dt / powf(dx, 2);
    if(CFL > 0.25){
        cerr << "Instabilità numerica: ridurre dt o aumentare dx." << endl;
        return 1;
    }

    double start_time = 0.0;
    double time_elapsed = start_time;
    while(time_elapsed < max_time){
        UpdateTemperature(temp, dim, dt, alpha);
        time_elapsed += dt;
    }

    cout << "Simulation completed." << endl;

    free(temp);
    return 0;
}

