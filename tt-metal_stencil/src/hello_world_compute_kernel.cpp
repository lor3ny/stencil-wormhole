// SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <tt-metalium/host_api.hpp>
#include <tt-metalium/device.hpp>

using namespace tt;
using namespace tt::tt_metal;


inline void saveGridCSV(const std::string& filename, double* grid, int dim_x, int dim_y) {
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

int main(int argc, char** argv) {

    // ---------------------------------------------------------
    // TENSTORRENT START SETUP
    // ---------------------------------------------------------

    // Initialize Program and Device

    constexpr CoreCoord core = {0, 0};
    int device_id = 0;
    IDevice* device = CreateDevice(device_id);
    CommandQueue& cq = device->command_queue();
    Program program = CreateProgram();

    // Configure and Create Void Kernel

    std::vector<uint32_t> compute_kernel_args = {};
    KernelHandle void_compute_kernel_id = CreateKernel(
        program,
        "../src/kernels/compute/void_compute_kernel.cpp",
        core,
        ComputeConfig{
            .math_fidelity = MathFidelity::HiFi4,
            .fp32_dest_acc_en = false,
            .math_approx_mode = false,
            .compile_args = compute_kernel_args});


    // ---------------------------------------------------------
    // STENCIL START SETUP
    // ---------------------------------------------------------

    int dim = 1000; // Grid Size
    double lx = 10.0, ly = 10.0;   // Domain Size
    float max_time = 1;

    double dx = lx / (double) (dim - 1);
    double dy = ly / (double) (dim - 1);
    double dt = 0.0001;  // Time step
    double alpha = 0.1;  // Coefficient of diffusion

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
    if(CFL > 0.25){
        cerr << "CFL: " << CFL << " Instabilità numerica: ridurre dt o aumentare dx." << endl;
        return 1;
    }

    // ---------------------------------------------------------
    // KERNEL LAUNCH AND WAITING
    // ---------------------------------------------------------

    double start_time = 0.0;
    double time_elapsed = start_time;
    double *tmp_temp;
    int steps = 0;
    
    // Configure Program and Start Program Execution on Device

    SetRuntimeArgs(program, void_compute_kernel_id, core, {});
    EnqueueProgram(cq, program, false);
    printf("Hello, Core {0, 0} on Device 0, I am sending you a compute kernel. Standby awaiting communication.\n");

    // Wait Until Program Finishes, Print "Hello World!", and Close Device

    Finish(cq);
    printf("Thank you, Core {0, 0} on Device 0, for the completed task.\n");
    CloseDevice(device);


    // ---------------------------------------------------------
    // SAVE RESULTS AND CLEANING
    // ---------------------------------------------------------


    saveGridCSV("result.csv", res_temp, dim, dim);

    free(res_temp);
    free(temp);

    return 0;
}
