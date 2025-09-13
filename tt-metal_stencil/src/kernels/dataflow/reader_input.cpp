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
    uint32_t src_bank_id = get_arg_val<uint32_t>(2);
    uint32_t src_size = get_arg_val<uint32_t>(3);

    uint32_t stencil_addr = get_arg_val<uint32_t>(4);
    uint32_t stencil_tiles = get_arg_val<uint32_t>(5);
    uint32_t stencil_bank_id = get_arg_val<uint32_t>(6);
    uint32_t stencil_size = get_arg_val<uint32_t>(7);


    constexpr uint32_t cb_id_in0 = tt::CBIndex::c_0;
    constexpr uint32_t cb_id_in1 = tt::CBIndex::c_1;

    //! Same for stencil, input and output
    const DataFormat data_format = get_dataformat(cb_id_in0);
    const uint32_t tile_bytes = get_tile_size(cb_id_in0);

    // is true because src0 is uploaded in DRAM
    const InterleavedAddrGenFast<true> src_dram_loc = {
        .bank_base_address = src_addr, .page_size = tile_bytes, .data_format = data_format};

    const InterleavedAddrGenFast<true> stencil_dram_loc = {
        .bank_base_address = stencil_addr, .page_size = tile_bytes, .data_format = data_format};


    // SINGLE-TILE VERSION

    //uint32_t l1_write_addr_in0 = get_write_ptr(cb_id_in0);
    //std::uint64_t dram_buffer_src_noc_addr = get_noc_addr_from_bank_id<true>(src_bank_id, src_addr);
    //cb_reserve_back(cb_id_in0, 1);
    //noc_async_read(dram_buffer_src_noc_addr, l1_write_addr_in0, src_size);
    //noc_async_read_barrier();
    //cb_push_back(cb_id_in0, 1);


    /*
    Qui leggiamo un tile alla volta da DRAM (s0), vorremmo fare una lettura scaglionata

    */

    //* STENCIL READING
    cb_reserve_back(cb_id_in1, 1);

    //! The tile is written in index 0 of CB_0
    uint32_t l1_write_addr_in1 = get_write_ptr(cb_id_in1);
    noc_async_read_tile(0, src_dram_loc, l1_write_addr_in1);

    noc_async_read_barrier();
    cb_push_back(cb_id_in1, 1);

    //* INPUT READING, pipelined with computation and writing

    for (uint32_t tile_i = 0; tile_i < num_tiles; tile_i++) {

        cb_reserve_back(cb_id_in0, 1);

        //! The tile is written in index 0 of CB_0
        uint32_t l1_write_addr_in0 = get_write_ptr(cb_id_in0);
        noc_async_read_tile(tile_i, src_dram_loc, l1_write_addr_in0);

        noc_async_read_barrier();
        cb_push_back(cb_id_in0, 1);
    } 
    
    DPRINT << "READER STOP" << ENDL();

}
