// SPDX-FileCopyrightText: © 2024 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include "dataflow_api.h"
#include "debug/dprint.h"

using namespace tt;

void kernel_main() {

    //DeviceZoneScopedN("Writer Kernels");
    
    uint32_t dst_addr = get_arg_val<uint32_t>(0);
    uint32_t start_tile_index = get_arg_val<uint32_t>(1);
    uint32_t num_tiles = get_arg_val<uint32_t>(2);

    constexpr uint32_t cb_id_out = 7;
    uint32_t i;
  
    constexpr auto s_args_dst = TensorAccessorArgs<0>();
    const auto dst_noc_addr = TensorAccessor(s_args_dst, dst_addr, get_tile_size(cb_id_out));

    for(i = 0; i<num_tiles; i++){    
        cb_wait_front(cb_id_out, 1);
        uint32_t l1_addr_out = get_read_ptr(cb_id_out);
        noc_async_write_tile(i+start_tile_index, dst_noc_addr, l1_addr_out);
        noc_async_write_barrier();
        cb_pop_front(cb_id_out, 1);
    }
    
}
