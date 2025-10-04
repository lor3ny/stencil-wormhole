
//#include "debug/dprint.h"  // required in all kernels using DPRINT
#include <stdint.h>

#include "compute_kernel_api.h"
#include "compute_kernel_api/common.h"
#include "compute_kernel_api/tile_move_copy.h"
#include "compute_kernel_api/matmul.h"
#include "tools/profiler/kernel_profiler.hpp"

namespace NAMESPACE {
void MAIN {

    DeviceZoneScopedN("Compute Kernels");
    
    uint32_t num_tiles = get_arg_val<uint32_t>(0);

    constexpr uint32_t cb_in0 = 0;
    constexpr uint32_t cb_in1 = 1;
    constexpr uint32_t cb_out16 = 16;
    constexpr uint32_t dst_reg_index = 0;

    mm_init(cb_in0, cb_in1, cb_out16);

    // OPERATIONS ARE ASYNCH SO EVERYTHING IS PIPELINED
    //! Tile index is always 0, destination index is always
    //! If you process more tile at once, then you need more indices

    for(uint32_t i = 0; i < num_tiles; i++) {

        tile_regs_acquire();

        cb_wait_front(cb_in0, 1);
        cb_wait_front(cb_in1, 1);
    
        matmul_tiles(cb_in0, cb_in1, 0, 0, 0, false);
        
        cb_pop_front(cb_in0, 1);
        //cb_pop_front(cb_in1, 1);

        tile_regs_commit();
        tile_regs_wait();

        cb_reserve_back(cb_out16, 1);
        pack_tile(0, cb_out16);
        cb_push_back(cb_out16, 1);

        tile_regs_release();
    }

}

}
