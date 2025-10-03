// SPDX-FileCopyrightText: © 2024 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <stdint.h>
#include "dataflow_api.h"
#include "debug/dprint.h"

using namespace tt;

void kernel_main() {

    DeviceZoneScopedN("Reader Kernels");

    uint32_t src_addr_UP = get_arg_val<uint32_t>(0);
    uint32_t src_addr_LEFT= get_arg_val<uint32_t>(1);
    uint32_t src_addr_RIGHT = get_arg_val<uint32_t>(2);
    uint32_t src_addr_DOWN = get_arg_val<uint32_t>(3);
    uint32_t src_addr_SCALAR = get_arg_val<uint32_t>(4);
    uint32_t start_tile_index = get_arg_val<uint32_t>(5);
    uint32_t scalar_tile_index = get_arg_val<uint32_t>(6);
    uint32_t num_tiles = get_arg_val<uint32_t>(7);

    constexpr uint32_t cb_id_1 = 1; // UP
    constexpr uint32_t cb_id_2 = 2; // LEFT
    constexpr uint32_t cb_id_3 = 3; // RIGHT
    constexpr uint32_t cb_id_4 = 4; // DOWN
    constexpr uint32_t cb_id_5 = 5; // SCALAR
    
    uint32_t i, idx;

    constexpr auto s_args_UP = TensorAccessorArgs<0>();
    const auto src_UP = TensorAccessor(s_args_UP, src_addr_UP, get_tile_size(cb_id_1));

    constexpr auto s_args_LEFT = TensorAccessorArgs<s_args_UP.next_compile_time_args_offset()>();
    const auto src_LEFT = TensorAccessor(s_args_LEFT, src_addr_LEFT, get_tile_size(cb_id_2));

    constexpr auto s_args_RIGHT = TensorAccessorArgs<s_args_LEFT.next_compile_time_args_offset()>();
    const auto src_RIGHT = TensorAccessor(s_args_RIGHT, src_addr_RIGHT, get_tile_size(cb_id_3));
    
    constexpr auto s_args_DOWN = TensorAccessorArgs<s_args_RIGHT.next_compile_time_args_offset()>();
    const auto src_DOWN = TensorAccessor(s_args_DOWN, src_addr_DOWN, get_tile_size(cb_id_4));
    
    constexpr auto s_args_SCALAR = TensorAccessorArgs<s_args_DOWN.next_compile_time_args_offset()>();
    const auto src_SCALAR = TensorAccessor(s_args_SCALAR, src_addr_SCALAR, get_tile_size(cb_id_5));


    cb_reserve_back(cb_id_5, 1);
    uint32_t l1_write_addr_in5 = get_write_ptr(cb_id_5);
    noc_async_read_tile(scalar_tile_index, src_SCALAR, l1_write_addr_in5);
    noc_async_read_barrier();
    cb_push_back(cb_id_5, 1);

    for(i = 0; i<num_tiles; i++){

        DPRINT << "READER tile: " << i << ENDL();

        {
            cb_reserve_back(cb_id_1, 1);
            uint32_t l1_write_addr_in1 = get_write_ptr(cb_id_1);
            noc_async_read_tile(i+start_tile_index, src_UP, l1_write_addr_in1);
            noc_async_read_barrier();
            cb_push_back(cb_id_1, 1);
        }   

        {
            cb_reserve_back(cb_id_4, 1);
            uint32_t l1_write_addr_in4 = get_write_ptr(cb_id_4);
            noc_async_read_tile(i+start_tile_index, src_DOWN, l1_write_addr_in4);
            noc_async_read_barrier();
            cb_push_back(cb_id_4, 1);
        }  

        {
            cb_reserve_back(cb_id_2, 1);
            uint32_t l1_write_addr_in2 = get_write_ptr(cb_id_2);
            noc_async_read_tile(i+start_tile_index, src_LEFT, l1_write_addr_in2);
            noc_async_read_barrier();
            cb_push_back(cb_id_2, 1);
        }  

        {
            cb_reserve_back(cb_id_3, 1);
            uint32_t l1_write_addr_in3 = get_write_ptr(cb_id_3);
            noc_async_read_tile(i+start_tile_index, src_RIGHT, l1_write_addr_in3);
            noc_async_read_barrier();
            cb_push_back(cb_id_3, 1);
        }   

    }
    
}
