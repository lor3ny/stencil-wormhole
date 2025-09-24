#include <stdint.h>
#include "dataflow_api.h"
//#include "debug/dprint.h"

void kernel_main() {

    DPRINT << "READER GO" << ENDL();

    DeviceZoneScopedN("READER KERNEL");

    // I HAVE ONLY ONE INPUT BUFFER, that goes to CB c_0 
    uint32_t src_addr = get_arg_val<uint32_t>(0);
    uint32_t src_tile_start_idx = get_arg_val<uint32_t>(1);
    uint32_t src_num_tiles = get_arg_val<uint32_t>(2);
    uint32_t src_size = get_arg_val<uint32_t>(3);

    uint32_t stencil_addr = get_arg_val<uint32_t>(4);
    uint32_t stencil_tiles = get_arg_val<uint32_t>(6);
    uint32_t stencil_tile_start_idx = get_arg_val<uint32_t>(5);
    uint32_t stencil_size = get_arg_val<uint32_t>(7);

    constexpr uint32_t cb_id_in0 = 0;
    constexpr uint32_t cb_id_in1 = 1;

    constexpr auto s_args_0 = TensorAccessorArgs<0>();
    const auto src_dram_loc = TensorAccessor(s_args_0, src_addr, get_tile_size(cb_id_in0));

    constexpr auto s_args_1 = TensorAccessorArgs<s_args_0.next_compile_time_args_offset()>();
    const auto stencil_dram_loc = TensorAccessor(s_args_1, stencil_addr, get_tile_size(cb_id_in1));

    //* INPUT READING, pipelined with computation and writing


    cb_reserve_back(cb_id_in1, 1); // that call is blocking, it goes only if there's space
    uint32_t l1_write_addr_in1 = get_write_ptr(cb_id_in1);
    noc_async_read_tile(stencil_tile_start_idx, stencil_dram_loc, l1_write_addr_in1);
    noc_async_read_barrier();
    cb_push_back(cb_id_in1, 1);

    for (uint32_t tile_i = 0; tile_i < src_num_tiles; tile_i++) {
        {
            cb_reserve_back(cb_id_in0, 1); // that call is blocking, it goes only if there's space
            uint32_t l1_write_addr_in0 = get_write_ptr(cb_id_in0);
            noc_async_read_tile(tile_i+src_tile_start_idx, src_dram_loc, l1_write_addr_in0);
            noc_async_read_barrier();
            cb_push_back(cb_id_in0, 1);
        }
    } 
    
    DPRINT << "READER STOP" << ENDL();

}
