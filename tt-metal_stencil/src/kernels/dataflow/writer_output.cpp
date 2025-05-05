// SPDX-FileCopyrightText: © 2024 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include "dataflow_api.h"
#include "debug/dprint.h"

void kernel_main() {

    DPRINT << "WRITER GO" << ENDL();
    
    uint32_t dst_addr = get_arg_val<uint32_t>(0);
    uint32_t tiles_count = get_arg_val<uint32_t>(1);
    uint32_t dst_bank_id = get_arg_val<uint32_t>(2);
    uint32_t dst_size = get_arg_val<uint32_t>(3);

    constexpr uint32_t cb_id_out = tt::CBIndex::c_4;

    const uint32_t dst_tile_bytes = get_tile_size(cb_id_out);
    const DataFormat dst_data_format = get_dataformat(cb_id_out);
        
    const InterleavedAddrGenFast<true> dst_noc_addr = {
        .bank_base_address = dst_addr, .page_size = dst_tile_bytes, .data_format = dst_data_format};    

    // SINGLE-TILE VERSION

    uint32_t bytes_per_float = 4;
    uint32_t batch_offset = 0;
    uint32_t BATCH_X = 32;
    uint32_t BATCH_Y = 32;
    uint32_t MAT_X = BATCH_X - 2*bytes_per_float; // 15 FLOATS
    uint32_t MAT_Y = BATCH_Y - 2*bytes_per_float; // 15 FLOATS

    uint32_t l1_addr_out = get_write_ptr(cb_id_out);
    uint64_t noc_addr_offset = dst_noc_addr.bank_base_address + batch_offset;

    cb_wait_front(cb_id_out, 1);
    noc_async_write(l1_addr_out, noc_addr_offset, MAT_X*MAT_Y);
    noc_async_write_barrier();
    cb_pop_front(cb_id_out, 1);
    
    DPRINT << "WRITER STOP" << ENDL();
}
