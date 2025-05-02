
#include "debug/dprint.h"  // required in all kernels using DPRINT
#include "compute_kernel_api.h"
#include "compute_kernel_api/common.h"
#include "compute_kernel_api/eltwise_unary/eltwise_unary.h"
#include "compute_kernel_api/tile_move_copy.h"

namespace NAMESPACE {
void MAIN {

    DPRINT << "Inizio calcolo" << ENDL();
    
    constexpr auto cb_in0 = tt::CBIndex::c_0;
    constexpr auto cb_out16 = tt::CBIndex::c_16;

    
    //binary_op_init_common(cb_in0, cb_out16, cb_out16);
    //add_tiles_init(cb_in0, cb_out16);
    
    // wait for a block of tiles in each of input CBs

    tile_regs_acquire();

    cb_wait_front(cb_in0, 1);

    copy_tile(cb_in0, 0, 0);

    //add_binary_tile(0, 1);
    //cb_reserve_back(cb_out16, 1);

    tile_regs_commit();
    tile_regs_wait();

    pack_tile(0, cb_out16);
    tile_regs_release();

    cb_push_back(cb_out16, 1);
    cb_pop_front(cb_in0, 1);

    // unary_op_init_common(cb_in0, cb_out16);
    // copy_tile_init(cb_in0);

    // acquire_dst();

    // cb_wait_front(cb_in0, 1);
    // cb_reserve_back(cb_out16, 1);

    // copy_tile(cb_in0, 0, 0);

    // pack_tile(0, cb_out16);
    // cb_push_back(cb_out16, 1);
    // cb_pop_front(cb_in0, 1);
    // release_dst();    

    // DPRINT << "Fine calcolo" << ENDL();
}

}
