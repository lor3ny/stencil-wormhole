// SPDX-FileCopyrightText: © 2024 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <stdint.h>
#include "dataflow_api.h"

#include "debug/dprint.h"

void kernel_main() {

    DPRINT << "READER GO" << ENDL();

    // I HAVE ONLY ONE INPUT BUFFER, that goes to CB c_0 
    uint32_t src_addr = get_arg_val<uint32_t>(0);
    uint32_t num_tiles = get_arg_val<uint32_t>(1);
    uint32_t src_size = get_arg_val<uint32_t>(2);

    uint32_t stencil_addr = get_arg_val<uint32_t>(3);
    uint32_t stencil_tiles = get_arg_val<uint32_t>(4);
    uint32_t stencil_size = get_arg_val<uint32_t>(5);

    constexpr uint32_t cb_id_in0 = 0;
    constexpr uint32_t cb_id_in1 = 1;

    //! Same for stencil, input and output
    // const DataFormat data_format_cb0 = get_dataformat(cb_id_in0);
    // const uint32_t tile_bytes_cb0 = get_tile_size(cb_id_in0);
    // const DataFormat data_format_cb1 = get_dataformat(cb_id_in1);
    // const uint32_t tile_bytes_cb1 = get_tile_size(cb_id_in1);

    // // is true because src0 is uploaded in DRAM
    // const InterleavedAddrGenFast<true> src_dram_loc = {
    //     .bank_base_address = src_addr, .page_size = tile_bytes_cb0, .data_format = data_format_cb0};

    // const InterleavedAddrGenFast<true> stencil_dram_loc = {
    //     .bank_base_address = stencil_addr, .page_size = tile_bytes_cb1, .data_format = data_format_cb1};

    constexpr auto s_args_0 = TensorAccessorArgs<0>();
    const auto src_dram_loc = TensorAccessor(s_args_0, src_addr, get_tile_size(cb_id_in0));

    constexpr auto s_args_1 = TensorAccessorArgs<s_args_0.next_compile_time_args_offset()>();
    const auto stencil_dram_loc = TensorAccessor(s_args_1, stencil_addr, get_tile_size(cb_id_in1));

    //* INPUT READING, pipelined with computation and writing

    for (uint32_t tile_i = 0; tile_i < num_tiles; tile_i++) {
        {
            cb_reserve_back(cb_id_in0, 1); // that call is blocking, it goes only if there's space
            uint32_t l1_write_addr_in0 = get_write_ptr(cb_id_in0);
            noc_async_read_tile(tile_i, src_dram_loc, l1_write_addr_in0);
            noc_async_read_barrier();
            cb_push_back(cb_id_in0, 1);
        }

        {
            cb_reserve_back(cb_id_in1, 1); // that call is blocking, it goes only if there's space
            uint32_t l1_write_addr_in1 = get_write_ptr(cb_id_in1);
            noc_async_read_tile(tile_i, stencil_dram_loc, l1_write_addr_in1);
            noc_async_read_barrier();
            cb_push_back(cb_id_in1, 1);
        }
    } 
    
    DPRINT << "READER STOP" << ENDL();

}
