
#include "debug/dprint.h"  // required in all kernels using DPRINT
#include "compute_kernel_api.h"
#include "compute_kernel_api/common.h"
#include "compute_kernel_api/eltwise_binary.h"
#include "compute_kernel_api/eltwise_unary/eltwise_unary.h"
#include "compute_kernel_api/tile_move_copy.h"
//#include "tools/profiler/kernel_profiler.hpp"

namespace NAMESPACE {
void MAIN {    

    //DeviceZoneScopedN("Compute Kernels");
    
    uint32_t num_tiles = get_arg_val<uint32_t>(0);

    constexpr auto cb_inUP = 1;
    constexpr auto cb_inLEFT = 2;
    constexpr auto cb_inRIGHT = 3;
    constexpr auto cb_inDOWN = 4;
    constexpr auto cb_SCALAR = 5;

    constexpr auto cb_MID = 6; // AUXILIARY
    constexpr auto cb_OUTPUT = 7; // OUTPUT
    constexpr uint32_t dst0 = 0;

    uint32_t i;

    binary_op_init_common(cb_inUP, cb_inDOWN, cb_MID);

    for(i=0; i<num_tiles; i++){

         // ADDING UP AND DOWN

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

        // ADDING LEFT
        
        tile_regs_acquire();
        add_tiles_init(cb_inLEFT, cb_MID);
        cb_wait_front(cb_inLEFT, 1);
        cb_wait_front(cb_MID, 1);
        add_tiles(cb_inLEFT, cb_MID, 0, 0, dst0);
        cb_pop_front(cb_inLEFT, 1);
        cb_pop_front(cb_MID, 1);

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
        cb_pop_front(cb_inRIGHT, 1);
        cb_pop_front(cb_MID, 1);

        tile_regs_commit();
        tile_regs_wait();
        
        cb_reserve_back(cb_MID, 1);
        pack_tile(dst0, cb_MID);
        cb_push_back(cb_MID, 1);
        tile_regs_release();

        // SCALING
        
        tile_regs_acquire();
        mul_tiles_init(cb_SCALAR, cb_MID);
        cb_wait_front(cb_SCALAR, 1);
        cb_wait_front(cb_MID, 1);
        mul_tiles(cb_SCALAR, cb_MID, 0, 0, dst0);
        cb_pop_front(cb_MID, 1);

        tile_regs_commit();
        tile_regs_wait();
        
        cb_reserve_back(cb_OUTPUT, 1);
        pack_tile(dst0, cb_OUTPUT);
        cb_push_back(cb_OUTPUT, 1);
        tile_regs_release();
    }
}
}
