#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;


void UpdateTemperature(double *temp, double *temp_new, int dim, double CFL) {
    // Update temperature
    for(int i = 1; i < dim - 1; i++){
        for(int j = 1; j < dim - 1; j++){
            temp_new[i*dim+j] = temp[i*dim+j] + CFL* (temp[(i+1)*dim+j] + temp[(i-1)*dim+j] + temp[i*dim+(j+1)] + temp[i*dim+(j-1)] - 4 * temp[i*dim+j]);
            //cout << "CFL: " << CFL << <<" values: " << -4*temp[i*dim+j] << " " << temp[(i+1)*dim+j] << " " << temp[(i-1)*dim+j] << " " << temp[i*dim+(j+1)] << " " << temp[i*dim+(j-1)] << " " << temp_new[i*dim+j] << endl;
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


void saveGridCSV(const std::string& filename, double* grid, int dim_x, int dim_y) {
    std::ofstream file(filename);
    if (!file) {
        cerr << "Errore nell'apertura del file" << endl;
        return;
    }
    file << dim_x << " " << dim_y << endl;
    for (size_t i=0; i<dim_x; ++i) {
        for (size_t j = 0; j < dim_y; ++j) {
            file << grid[i*dim_x + j] << (j + 1 == dim_y ? "\n" : " ");
        }
    }
    file.close();
}

int main(){

    // SIMULATION PARAMETERS
    int dim = 100; //GRID SIZE
    double lx = 1.0, ly = 1.0;   //DOMAIN SIZE
    float max_time = 0.1;

    double dx = lx / (double) (dim - 1);
    double dy = ly / (double) (dim - 1);
    double dt = 0.0001;  // Time step
    double alpha = 0.1;  // Coefficient of diffusion

    //TEMPERATURE MATRIX
    double* temp = (double*) malloc(dim * dim * sizeof(double));
    double* res_temp = (double*) malloc(dim * dim * sizeof(double));
    if (!temp || !res_temp) {
        cerr << "Errore: allocazione di memoria fallita!" << endl;
        free(temp);
        free(res_temp);
        return 1;
    }
    memset(temp, 0, dim * dim * sizeof(double));
    memset(res_temp, 0, dim * dim * sizeof(double));

    temp[(dim/2)*dim + (dim/2)] = 100.0;
    res_temp[(dim/2)*dim + (dim/2)] = 100.0;

    // CFL Stability Condition (Courant-Friedrichs-Lewy)
    double CFL = (double) alpha * (double) dt / (double) powf(dx, 2);
    cout << powf(dx, 2) << endl;
    if(CFL > 0.25){
        cerr << "CFL: " << CFL << " Instabilità numerica: ridurre dt o aumentare dx." << endl;
        return 1;
    }

    double start_time = 0.0;
    double time_elapsed = start_time;
    double *tmp_temp;
    int steps = 0;
    while(time_elapsed < max_time){
        cout << "Tempo: " << time_elapsed << "s" << endl;

        UpdateTemperature(temp, res_temp, dim, CFL);
        tmp_temp = temp;
        temp = res_temp;
        res_temp = tmp_temp;

        time_elapsed += dt;
        steps++;
    }

    cout << "Simulation completed." << endl;

    PrintGrid(temp, dim);
    saveGridCSV("result.csv", res_temp, dim, dim);

    free(res_temp);
    //free(gb_temp);
    return 0;
}

