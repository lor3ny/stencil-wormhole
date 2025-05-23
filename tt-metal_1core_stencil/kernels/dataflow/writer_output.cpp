// SPDX-FileCopyrightText: © 2024 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include "dataflow_api.h"
#include "debug/dprint.h"

using namespace tt;

void kernel_main() {

    DPRINT << "WRITER GO" << ENDL();
    
    uint32_t dst_addr = get_arg_val<uint32_t>(0);

    constexpr uint32_t cb_id_out = tt::CBIndex::c_6;

    const uint32_t dst_tile_bytes = get_tile_size(cb_id_out);
    const DataFormat dst_data_format = get_dataformat(cb_id_out);
        
    const InterleavedAddrGenFast<true> dst_noc_addr = {
        .bank_base_address = dst_addr, .page_size = dst_tile_bytes, .data_format = dst_data_format};    


    cb_wait_front(cb_id_out, 1);
    uint32_t l1_addr_out = get_write_ptr(cb_id_out);
    noc_async_write_tile(0, dst_noc_addr, l1_addr_out);
    noc_async_write_barrier();
    cb_pop_front(cb_id_out, 1);
    
    DPRINT << "WRITER STOP" << ENDL();
}
