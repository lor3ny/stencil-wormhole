//
// SPDX-License-Identifier: Apache-2.0

#include "debug/dprint.h"  // required in all kernels using DPRINT
#include "compute_kernel_api.h"

namespace NAMESPACE {

void MAIN {
    // Nothing to compute. Print respond message.
    // Make sure to export TT_METAL_DPRINT_CORES=0,0 before runtime.

    DPRINT_MATH(DPRINT << "Hello, Master, I am running a void compute kernel." << ENDL());

    // BISONARE PUSHARE TEMP, TEMP_NEW, DIM, CFL

    //void UpdateTemperature(double *temp, double *temp_new, int dim, double CFL) {
    // for(int i = 1; i < dim - 1; i++){
    //     for(int j = 1; j < dim - 1; j++){
    //         temp_new[i*dim+j] = temp[i*dim+j] + CFL* (temp[(i+1)*dim+j] + temp[(i-1)*dim+j] + 
    //                                                     temp[i*dim+(j+1)] + temp[i*dim+(j-1)] - 
    //                                                     4 * temp[i*dim+j]);
    //         //cout << "CFL: " << CFL << <<" values: " << -4*temp[i*dim+j] << " " << temp[(i+1)*dim+j] << " " << temp[(i-1)*dim+j] << " " << temp[i*dim+(j+1)] << " " << temp[i*dim+(j-1)] << " " << temp_new[i*dim+j] << endl;
    //     }
    // }
}

}  // namespace NAMESPACE
