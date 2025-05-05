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


    constexpr uint32_t cb_id_0 = tt::CBIndex::c_0;
    constexpr uint32_t cb_id_1 = tt::CBIndex::c_1;
    constexpr uint32_t cb_id_2 = tt::CBIndex::c_2;
    constexpr uint32_t cb_id_3 = tt::CBIndex::c_3;
    constexpr uint32_t cb_id_4 = tt::CBIndex::c_4;
    const DataFormat src_data_format = get_dataformat(cb_id_0);
    const uint32_t src_tile_bytes = get_tile_size(cb_id_0);

    // is true because src0 is uploaded in DRAM
    const InterleavedAddrGenFast<true> s0 = {
        .bank_base_address = src_addr, .page_size = src_tile_bytes, .data_format = src_data_format};

    // THE CENTRAL MAT

    uint32_t bytes_per_float = 4;

    uint32_t batch_offset = 0; 

    uint32_t BATCH_X = 32; //32 bytes, 8 floats
    uint32_t BATCH_Y = 32;
    uint32_t MAT_X = BATCH_X - 2*bytes_per_float; // 24 bytes, 6 floats
    uint32_t MAT_Y = BATCH_Y - 2*bytes_per_float; 

    
    for(uint32_t i = 1; i<MAT_Y; i++){

        uint64_t noc_addr_offset = s0.bank_base_address + batch_offset + (i*BATCH_X) + 1 * bytes_per_float;

        std::uint32_t offset=noc_addr_offset % 32;
        std::uint32_t offset_start=noc_addr_offset-offset;
        std::uint32_t page_id=offset_start / 1024;
        std::uint32_t page_offset=offset_start % 1024;

        uint64_t domain_data_buffer_noc_addr = s0.get_noc_addr(page_id, page_offset);

        uint32_t cb_addr_4 = get_write_ptr(cb_id_4);
        noc_async_read(domain_data_buffer_noc_addr, cb_addr_4 + (i-1) * MAT_X, MAT_X);
        noc_async_read_barrier();
        cb_push_back(cb_id_4, 1);
    } 
    

    // --
    /*
    for(uint32_t i = 0; i<MAT_Y; i++){
        uint64_t noc_addr_offset = s0.bank_base_address + batch_offset + (i*BATCH_X) + 1 * bytes_per_float;
        uint32_t cb_addr_0 = get_write_ptr(cb_id_0);
        noc_async_read(noc_addr_offset, cb_addr_0 + i * BATCH_X, MAT_X);
        noc_async_read_barrier();
        cb_push_back(cb_id_0, 1);
    } 

    for(uint32_t i = 1; i<MAT_Y; i++){
        uint64_t noc_addr_offset = s0.bank_base_address + batch_offset + (i*BATCH_X) + 1 * bytes_per_float;
        uint32_t cb_addr_1 = get_write_ptr(cb_id_1);
        noc_async_read(noc_addr_offset, cb_addr_1 + (i-1) * BATCH_X, MAT_X);
        noc_async_read_barrier();
        cb_push_back(cb_id_1, 1);
    } 

    for(uint32_t i = 1; i<MAT_Y; i++){
        uint64_t noc_addr_offset = s0.bank_base_address + batch_offset + (i*BATCH_X) + 2 * bytes_per_float;
        uint32_t cb_addr_2 = get_write_ptr(cb_id_2);
        noc_async_read(noc_addr_offset, cb_addr_2 + (i-1) * BATCH_X, MAT_X);
        noc_async_read_barrier();
        cb_push_back(cb_id_2, 1);
    } 

    for(uint32_t i = 2; i<MAT_Y; i++){
        uint64_t noc_addr_offset = s0.bank_base_address + batch_offset + (i*BATCH_X) + 1 * bytes_per_float;
        uint32_t cb_addr_3 = get_write_ptr(cb_id_3);
        noc_async_read(noc_addr_offset, cb_addr_3 + (i-2) * BATCH_X, MAT_X);
        noc_async_read_barrier();
        cb_push_back(cb_id_3, 1);
    } 
    */
}
