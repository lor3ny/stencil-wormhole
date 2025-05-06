// SPDX-FileCopyrightText: © 2024 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <stdint.h>
#include "dataflow_api.h"

#include "debug/dprint.h"

void kernel_main() {

    DPRINT << "READER GO" << ENDL();

    // I HAVE ONLY ONE INPUT BUFFER, that goes to CB c_0 
    uint32_t src_addr_CENTER = get_arg_val<uint32_t>(0);
    uint32_t src_addr_UP = get_arg_val<uint32_t>(1);
    uint32_t src_addr_LEFT= get_arg_val<uint32_t>(2);
    uint32_t src_addr_RIGHT = get_arg_val<uint32_t>(3);
    uint32_t src_addr_DOWN = get_arg_val<uint32_t>(4);

    uint32_t num_tiles = get_arg_val<uint32_t>(1);
    uint32_t src_bank_id = get_arg_val<uint32_t>(2);
    //uint32_t src_size = get_arg_val<uint32_t>(3);


    constexpr uint32_t cb_id_0 = tt::CBIndex::c_0;
    constexpr uint32_t cb_id_1 = tt::CBIndex::c_1;
    constexpr uint32_t cb_id_2 = tt::CBIndex::c_2;
    constexpr uint32_t cb_id_3 = tt::CBIndex::c_3;
    constexpr uint32_t cb_id_4 = tt::CBIndex::c_4;
    const DataFormat src_data_format = get_dataformat(cb_id_0);
    const uint32_t src_tile_bytes = get_tile_size(cb_id_0);

    // is true because src0 is uploaded in DRAM
    const InterleavedAddrGenFast<true> src_CENTER = {
        .bank_base_address = src_addr_CENTER, .page_size = src_tile_bytes, .data_format = src_data_format};
    
    
    const InterleavedAddrGenFast<true> src_UP = {
        .bank_base_address = src_addr_UP, .page_size = src_tile_bytes, .data_format = src_data_format};
    /*
    const InterleavedAddrGenFast<true> src_LEFT = {
        .bank_base_address = src_addr_LEFT, .page_size = src_tile_bytes, .data_format = src_data_format};
    const InterleavedAddrGenFast<true> src_RIGHT = {
        .bank_base_address = src_addr_RIGHT, .page_size = src_tile_bytes, .data_format = src_data_format};
    const InterleavedAddrGenFast<true> src_DOWN = {
        .bank_base_address = src_addr_DOWN, .page_size = src_tile_bytes, .data_format = src_data_format};
    */
    
    /*
    cb_reserve_back(cb_id_0, 1);
    uint32_t l1_write_addr_in0 = get_write_ptr(cb_id_0);
    noc_async_read_tile(0, src_CENTER, l1_write_addr_in0);
    noc_async_read_barrier();
    cb_push_back(cb_id_0, 1);
    */

    cb_reserve_back(cb_id_1, 1);
    uint32_t l1_write_addr_in1 = get_write_ptr(cb_id_1);
    noc_async_read_tile(0, src_UP, l1_write_addr_in1);
    noc_async_read_barrier();
    cb_push_back(cb_id_1, 1);

    /*
    cb_reserve_back(cb_id_2, 1);
    uint32_t l1_write_addr_in2 = get_write_ptr(cb_id_2);
    noc_async_read_tile(0, src_LEFT, l1_write_addr_in2);
    noc_async_read_barrier();
    cb_push_back(cb_id_2, 1);

    cb_reserve_back(cb_id_3, 1);
    uint32_t l1_write_addr_in3 = get_write_ptr(cb_id_3);
    noc_async_read_tile(0, src_RIGHT, l1_write_addr_in3);
    noc_async_read_barrier();
    cb_push_back(cb_id_3, 1);

    cb_reserve_back(cb_id_4, 1);
    uint32_t l1_write_addr_in4 = get_write_ptr(cb_id_4);
    noc_async_read_tile(0, src_DOWN, l1_write_addr_in4);
    noc_async_read_barrier();
    cb_push_back(cb_id_4, 1);
    */ 
    DPRINT << "READER STOP" << ENDL();

    /*
    for(uint32_t i = 0; i<BATCH_Y; i++){ // SOSTITUIRE POI CON MAT_Y

        //uint64_t noc_addr_offset = s0.bank_base_address + batch_offset + (i*BATCH_X) + 1 * bytes_per_float;

        uint64_t noc_addr_offset = batch_offset + (i*BATCH_X); //+ 1 * bytes_per_float;

        std::uint32_t offset = noc_addr_offset % 32;
        
        std::uint32_t offset_start = noc_addr_offset-offset;
        
        std::uint32_t page_id=offset_start / 1024;

        std::uint32_t page_offset=offset_start % 1024;

        //uint32_t page_id = noc_addr_offset / src_tile_bytes;
        //uint32_t page_offset = noc_addr_offset % src_tile_bytes;

        uint64_t domain_data_buffer_noc_addr = s0.get_noc_addr(page_id, page_offset);

        uint32_t cb_addr_4 = get_write_ptr(cb_id_4);
        noc_async_read(domain_data_buffer_noc_addr, cb_addr_4 + i * BATCH_X, BATCH_X); //SOSTITUIRE POI CON MAT_X
        noc_async_read_barrier();
        cb_push_back(cb_id_4, 1); 
    } 
    */
    
    //std::uint32_t addr_offset=((batch_x_start+(i*total_y_size)+batch_y_start)*2) + (ALIGNMENT-2);

    //uint32_t buffer_addr_offset=i*buffer_bytes_per_line;

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
