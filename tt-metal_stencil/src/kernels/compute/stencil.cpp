
#include "debug/dprint.h"  // required in all kernels using DPRINT
#include "compute_kernel_api.h"
#include "compute_kernel_api/common.h"
#include "compute_kernel_api/eltwise_unary/eltwise_unary.h"
#include "compute_kernel_api/tile_move_copy.h"
#include "compute_kernel_api/matmul.h"

namespace NAMESPACE {
void MAIN {

    DPRINT << "Start compute" << ENDL();
    
    uint32_t num_tiles = get_arg_val<uint32_t>(0);

    constexpr auto cb_in0 = tt::CBIndex::c_0;
    constexpr auto cb_in1 = tt::CBIndex::c_1;
    constexpr auto cb_out16 = tt::CBIndex::c_16;
    constexpr uint32_t dst_reg_index = 0;


    // Reading the stencil tile



    //unary_op_init_common(cb_in0, cb_out16);
    //copy_tile_init(cb_in0);

    mm_init(cb_in0, cb_in1, cb_out16);

    // OPERATIONS ARE ASYNCH SO EVERYTHING IS PIPELINED
    //! Tile index is always 0, destination index is always
    //! If you process more tile at once, then you need more indices
    for(uint32_t i = 0; i < num_tiles; i++) {

        tile_regs_acquire();

        cb_reserve_back(cb_out16, 1);

        if (i == 0){
            cb_wait_front(cb_in1, 1);
        }
        cb_wait_front(cb_in0, 1);

        //copy_tile(cb_in0, 0, dst_reg_index);
        matmul_tiles(cb_in0, cb_in1, 0, 0, dst_reg_index, false);

        tile_regs_commit();
        tile_regs_wait();

        pack_tile(dst_reg_index, cb_out16);

        cb_push_back(cb_out16, 1);
        cb_pop_front(cb_in0, 1);

        tile_regs_release();
    }

    DPRINT << "End compute" << ENDL();
}

}
