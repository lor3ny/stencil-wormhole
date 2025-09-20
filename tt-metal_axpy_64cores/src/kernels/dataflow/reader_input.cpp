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
    
    uint32_t i, idx;

    // const DataFormat src_data_format = get_dataformat(cb_id_0);
    // const uint32_t src_tile_bytes = get_tile_size(cb_id_0);
    
    // const InterleavedAddrGenFast<true> src_CENTER = {
    //     .bank_base_address = src_addr_CENTER,
    //     .page_size = src_tile_bytes, 
    //     .data_format = src_data_format
    // };
    
    // const InterleavedAddrGenFast<true> src_UP = {
    //     .bank_base_address = src_addr_UP, 
    //     .page_size = src_tile_bytes,
    //     .data_format = src_data_format
    // };

    // const InterleavedAddrGenFast<true> src_DOWN = {
    //     .bank_base_address = src_addr_DOWN,
    //     .page_size = src_tile_bytes,
    //     .data_format = src_data_format
    // };
    // const InterleavedAddrGenFast<true> src_LEFT = {
    //     .bank_base_address = src_addr_LEFT,
    //     .page_size = src_tile_bytes,
    //     .data_format = src_data_format
    // };
    // const InterleavedAddrGenFast<true> src_RIGHT = {
    //     .bank_base_address = src_addr_RIGHT,
    //     .page_size = src_tile_bytes,
    //     .data_format = src_data_format
    // };

    // const InterleavedAddrGenFast<true> src_SCALAR = {
    //     .bank_base_address = src_addr_SCALAR,
    //     .page_size = src_tile_bytes,
    //     .data_format = src_data_format
    // };

    constexpr auto s_args_CENTER = TensorAccessorArgs<0>();
    const auto src_CENTER = TensorAccessor(s_args_CENTER, src_addr_CENTER, get_tile_size(cb_id_0));

    constexpr auto s_args_UP = TensorAccessorArgs<s_args_CENTER.next_compile_time_args_offset()>();
    const auto src_UP = TensorAccessor(s_args_UP, src_addr_UP, get_tile_size(cb_id_1));

    constexpr auto s_args_LEFT = TensorAccessorArgs<s_args_UP.next_compile_time_args_offset()>();
    const auto src_LEFT = TensorAccessor(s_args_LEFT, src_addr_LEFT, get_tile_size(cb_id_2));

    constexpr auto s_args_RIGHT = TensorAccessorArgs<s_args_LEFT.next_compile_time_args_offset()>();
    const auto src_RIGHT = TensorAccessor(s_args_RIGHT, src_addr_RIGHT, get_tile_size(cb_id_3));
    
    constexpr auto s_args_DOWN = TensorAccessorArgs<s_args_RIGHT.next_compile_time_args_offset()>();
    const auto src_DOWN = TensorAccessor(s_args_DOWN, src_addr_DOWN, get_tile_size(cb_id_4));
    
    constexpr auto s_args_SCALAR = TensorAccessorArgs<s_args_DOWN.next_compile_time_args_offset()>();
    const auto src_SCALAR = TensorAccessor(s_args_SCALAR, src_addr_SCALAR, get_tile_size(cb_id_5));

    

    for(i = 0; i<num_tiles; i++){

        uint32_t idx = i+start_tile_index;
        uint32_t l1_write_addr_in;

        {
            cb_reserve_back(cb_id_1, 1);
            uint32_t l1_write_addr_in1 = get_write_ptr(cb_id_1);
            noc_async_read_tile(idx, src_UP, l1_write_addr_in1);
            noc_async_read_barrier();
            cb_push_back(cb_id_1, 1);
        }   

        {
            cb_reserve_back(cb_id_4, 1);
            uint32_t l1_write_addr_in4 = get_write_ptr(cb_id_4);
            noc_async_read_tile(idx, src_DOWN, l1_write_addr_in4);
            noc_async_read_barrier();
            cb_push_back(cb_id_4, 1);
        }  

        {
            cb_reserve_back(cb_id_2, 1);
            uint32_t l1_write_addr_in2 = get_write_ptr(cb_id_2);
            noc_async_read_tile(idx, src_LEFT, l1_write_addr_in2);
            noc_async_read_barrier();
            cb_push_back(cb_id_2, 1);
        }  

        {
            cb_reserve_back(cb_id_3, 1);
            uint32_t l1_write_addr_in3 = get_write_ptr(cb_id_3);
            noc_async_read_tile(idx, src_RIGHT, l1_write_addr_in3);
            noc_async_read_barrier();
            cb_push_back(cb_id_3, 1);
        }   

        // {
        //     cb_reserve_back(cb_id_0, 1);
        //     DPRINT << "a" << ENDL();
        //     uint32_t l1_write_addr_in0 = get_write_ptr(cb_id_0);
        //     noc_async_read_tile(idx, src_CENTER, l1_write_addr_in0);
        //     noc_async_read_barrier();
        //     cb_push_back(cb_id_0, 1);
        // }  

        {
            cb_reserve_back(cb_id_5, 1);
            uint32_t l1_write_addr_in5 = get_write_ptr(cb_id_5);
            noc_async_read_tile(idx, src_SCALAR, l1_write_addr_in5);
            noc_async_read_barrier();
            cb_push_back(cb_id_5, 1);
        }   

    }

    DPRINT << "READER STOP" << ENDL();
    
}
