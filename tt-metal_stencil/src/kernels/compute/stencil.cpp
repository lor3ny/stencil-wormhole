
#include "debug/dprint.h"  // required in all kernels using DPRINT
#include "compute_kernel_api.h"
#include "compute_kernel_api/common.h"
#include "compute_kernel_api/eltwise_unary/eltwise_unary.h"
#include "compute_kernel_api/tile_move_copy.h"

namespace NAMESPACE {
void MAIN {

    DPRINT << "Start compute" << ENDL();
    
    uint32_t num_tiles = get_arg_val<uint32_t>(0);

    constexpr auto cb_in0 = tt::CBIndex::c_0;
    constexpr auto cb_out16 = tt::CBIndex::c_16;

    unary_op_init_common(cb_in0, cb_out16);
    copy_tile_init(cb_in0);

    // OPERATIONS ARE ASYNCH SO EVERYTHING IS PIPELINED

    tile_regs_acquire();

    for(uint32_t i = 0; i < num_tiles; i++) {

        DPRINT << i << ENDL();

        cb_reserve_back(cb_out16, 1);

        cb_wait_front(cb_in0, 1);

        copy_tile(cb_in0, i, i);

        tile_regs_commit();
        tile_regs_wait();

        pack_tile(0, cb_out16);

        cb_push_back(cb_out16, 1);
        cb_pop_front(cb_in0, 1);
    }

    tile_regs_release();


    // SINGLE-TILE VERSION

    // tile_regs_acquire();

    // cb_wait_front(cb_in0, 1);

    // copy_tile(cb_in0, 0, 0);

    // tile_regs_commit();
    // tile_regs_wait();

    // pack_tile(0, cb_out16);
    // tile_regs_release();

    // cb_push_back(cb_out16, 1);
    // cb_pop_front(cb_in0, 1);

    DPRINT << "End compute" << ENDL();
}

}
