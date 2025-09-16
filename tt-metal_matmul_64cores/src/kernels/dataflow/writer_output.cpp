// SPDX-FileCopyrightText: © 2024 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include "dataflow_api.h"
#include "debug/dprint.h"

void kernel_main() {

    DPRINT << "WRITER GO" << ENDL();
    
    uint32_t dst_addr = get_arg_val<uint32_t>(0);
    uint32_t tiles_count = get_arg_val<uint32_t>(1);
    uint32_t dst_size = get_arg_val<uint32_t>(2);

    constexpr uint32_t cb_id_out16 = 16;

    // const uint32_t dst_tile_bytes = get_tile_size(cb_id_out16);
    // const DataFormat dst_data_format = get_dataformat(cb_id_out16);
        
    // const InterleavedAddrGenFast<true> dst_noc_addr = {
    //     .bank_base_address = dst_addr, .page_size = dst_tile_bytes, .data_format = dst_data_format};    

    constexpr auto s_args = TensorAccessorArgs<0>();
    const auto dst_noc_addr = TensorAccessor(s_args, dst_addr, get_tile_size(cb_id_out16));


    for(uint32_t tile_i=0; tile_i<tiles_count; ++tile_i){
        cb_wait_front(cb_id_out16, 1);
        
        //! The tile is taken from index 0 of CB_16
        uint32_t l1_read_addr = get_read_ptr(cb_id_out16);
        noc_async_write_tile(tile_i, dst_noc_addr, l1_read_addr);

        noc_async_write_barrier();
        cb_pop_front(cb_id_out16, 1);
    }
    
    DPRINT << "WRITER STOP" << ENDL();
}
