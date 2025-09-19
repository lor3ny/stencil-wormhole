// SPDX-FileCopyrightText: © 2024 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <stdint.h>
#include "dataflow_api.h"
#include "debug/dprint.h"

using namespace tt;

void kernel_main() {

    DPRINT << "READER GO" << ENDL();

    uint32_t src_addr_CENTER = get_arg_val<uint32_t>(0);
    uint32_t src_addr_UP = get_arg_val<uint32_t>(1);
    uint32_t src_addr_LEFT= get_arg_val<uint32_t>(2);
    uint32_t src_addr_RIGHT = get_arg_val<uint32_t>(3);
    uint32_t src_addr_DOWN = get_arg_val<uint32_t>(4);
    uint32_t src_addr_SCALAR = get_arg_val<uint32_t>(5);
    uint32_t start_tile_index = get_arg_val<uint32_t>(6);
    uint32_t num_tiles = get_arg_val<uint32_t>(7);

    constexpr uint32_t cb_id_0 = 0; // CENTER
    constexpr uint32_t cb_id_1 = 1; // UP
    constexpr uint32_t cb_id_2 = 2; // LEFT
    constexpr uint32_t cb_id_3 = 3; // RIGHT
    constexpr uint32_t cb_id_4 = 4; // DOWN
    constexpr uint32_t cb_id_5 = 5; // SCALAR
    
    uint32_t i, idx, l1_write_addr_in;

    const DataFormat src_data_format = get_dataformat(cb_id_0);
    const uint32_t src_tile_bytes = get_tile_size(cb_id_0);
    
    const InterleavedAddrGenFast<true> src_CENTER = {
        .bank_base_address = src_addr_CENTER,
        .page_size = src_tile_bytes, 
        .data_format = src_data_format
    };
    
    const InterleavedAddrGenFast<true> src_UP = {
        .bank_base_address = src_addr_UP, 
        .page_size = src_tile_bytes,
        .data_format = src_data_format
    };

    const InterleavedAddrGenFast<true> src_DOWN = {
        .bank_base_address = src_addr_DOWN,
        .page_size = src_tile_bytes,
        .data_format = src_data_format
    };
    const InterleavedAddrGenFast<true> src_LEFT = {
        .bank_base_address = src_addr_LEFT,
        .page_size = src_tile_bytes,
        .data_format = src_data_format
    };
    const InterleavedAddrGenFast<true> src_RIGHT = {
        .bank_base_address = src_addr_RIGHT,
        .page_size = src_tile_bytes,
        .data_format = src_data_format
    };

    const InterleavedAddrGenFast<true> src_SCALAR = {
        .bank_base_address = src_addr_SCALAR,
        .page_size = src_tile_bytes,
        .data_format = src_data_format
    };
    

    for(i = 0; i<num_tiles; i++){

        DPRINT << i << ENDL();

        uint32_t idx = i+start_tile_index;
        uint32_t l1_write_addr_in;

        {
            cb_reserve_back(cb_id_1, 1);
            DPRINT << "a" << ENDL();
            l1_write_addr_in = get_write_ptr(cb_id_1);
            noc_async_read_tile(idx, src_UP, l1_write_addr_in);
            noc_async_read_barrier();
            cb_push_back(cb_id_1, 1);
        }   

        {
            cb_reserve_back(cb_id_4, 1);
            DPRINT << "a" << ENDL();
            l1_write_addr_in = get_write_ptr(cb_id_4);
            noc_async_read_tile(idx, src_UP, l1_write_addr_in);
            noc_async_read_barrier();
            cb_push_back(cb_id_4, 1);
        }  

        {
            cb_reserve_back(cb_id_2, 1);
            DPRINT << "a" << ENDL();
            l1_write_addr_in = get_write_ptr(cb_id_2);
            noc_async_read_tile(idx, src_UP, l1_write_addr_in);
            noc_async_read_barrier();
            cb_push_back(cb_id_2, 1);
        }  

        // {
        //     cb_reserve_back(cb_id_3, 1);
        //     DPRINT << "a" << ENDL();
        //     l1_write_addr_in = get_write_ptr(cb_id_3);
        //     noc_async_read_tile(idx, src_UP, l1_write_addr_in);
        //     noc_async_read_barrier();
        //     cb_push_back(cb_id_3, 1);
        // }   

        {
            cb_reserve_back(cb_id_0, 1);
            DPRINT << "a" << ENDL();
            l1_write_addr_in = get_write_ptr(cb_id_0);
            noc_async_read_tile(idx, src_UP, l1_write_addr_in);
            noc_async_read_barrier();
            cb_push_back(cb_id_0, 1);
        }  

        {
            cb_reserve_back(cb_id_5, 1);
            DPRINT << "a" << ENDL();
            l1_write_addr_in = get_write_ptr(cb_id_5);
            noc_async_read_tile(idx, src_UP, l1_write_addr_in);
            noc_async_read_barrier();
            cb_push_back(cb_id_5, 1);
        } 


    }

    DPRINT << "READER STOP" << ENDL();
    
}
