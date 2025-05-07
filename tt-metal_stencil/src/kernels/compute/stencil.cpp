
#include "debug/dprint.h"  // required in all kernels using DPRINT
#include "compute_kernel_api.h"
#include "compute_kernel_api/common.h"
#include "compute_kernel_api/eltwise_binary.h"
#include "compute_kernel_api/eltwise_unary/eltwise_unary.h"
#include "compute_kernel_api/tile_move_copy.h"

namespace NAMESPACE {
void MAIN {    
    
    constexpr auto cb_inCENTER = tt::CBIndex::c_0;
    constexpr auto cb_inUP = tt::CBIndex::c_1;
    constexpr auto cb_inLEFT = tt::CBIndex::c_2;
    constexpr auto cb_inRIGHT = tt::CBIndex::c_3;
    constexpr auto cb_inDOWN = tt::CBIndex::c_4;

    constexpr auto cb_SCALAR = tt::CBIndex::c_7;
    constexpr auto cb_MID = tt::CBIndex::c_5; // AUXILIARY
    constexpr auto cb_OUTPUT = tt::CBIndex::c_6; // OUTPUT

    // CB_SCALAR LO PRENDEREI DA COMPILER ARGS
    const auto scalar_stencil = 2.0f;

    constexpr uint32_t dst0 = 0;

    // ADDING UP AND DOWN

    DPRINT << "Start compute" << ENDL();

    
    binary_op_init_common(cb_inUP, cb_inDOWN, cb_MID);
    tile_regs_acquire();
    add_tiles_init(cb_inUP, cb_inDOWN);
    cb_wait_front(cb_inUP, 1);
    cb_wait_front(cb_inDOWN, 1);
    add_tiles(cb_inUP, cb_inDOWN, 0, 0, dst0);
    cb_pop_front(cb_inUP, 1);
    cb_pop_front(cb_inDOWN, 1);

    tile_regs_commit();
    tile_regs_wait();

    
    cb_reserve_back(cb_MID, 1);
    pack_tile(dst0, cb_MID);
    cb_push_back(cb_MID, 1);
    tile_regs_release();
    
    tile_regs_acquire();
    add_tiles_init(cb_MID, cb_inLEFT);
    cb_wait_front(cb_MID, 1);
    cb_wait_front(cb_inLEFT, 1);
    add_tiles(cb_inLEFT, cb_MID, 0, 0, dst0);
    cb_pop_front(cb_MID, 1);
    cb_pop_front(cb_inLEFT, 1);

    tile_regs_commit();
    tile_regs_wait();
    
    cb_reserve_back(cb_MID, 1);
    pack_tile(dst0, cb_MID);
    cb_push_back(cb_MID, 1);
    tile_regs_release();

    // ADDING RIGHT

    tile_regs_acquire();
    add_tiles_init(cb_inRIGHT, cb_MID);
    cb_wait_front(cb_inRIGHT, 1);
    cb_wait_front(cb_MID, 1);
    add_tiles(cb_inRIGHT, cb_MID, 0, 0, dst0);
    cb_pop_front(cb_MID, 1);
    cb_pop_front(cb_inRIGHT, 1);

    tile_regs_commit();
    tile_regs_wait();
    
    cb_reserve_back(cb_MID, 1);
    pack_tile(dst0, cb_MID);
    cb_push_back(cb_MID, 1);
    tile_regs_release();
    

    //SCALING
    
    tile_regs_acquire();
    add_tiles_init(cb_SCALAR, cb_MID);
    cb_wait_front(cb_MID, 1);
    cb_wait_front(cb_SCALAR, 1);
    add_tiles(cb_SCALAR, cb_MID, 0, 0, dst0);
    cb_pop_front(cb_MID, 1);
    cb_pop_front(cb_SCALAR, 1);

    tile_regs_commit();
    tile_regs_wait();
    
    cb_reserve_back(cb_OUTPUT, 1);
    pack_tile(dst0, cb_OUTPUT);
    cb_push_back(cb_OUTPUT, 1);

    tile_regs_release();
    
    
    DPRINT << "End compute" << ENDL();
    
}

}
