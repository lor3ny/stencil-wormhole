// SPDX-FileCopyrightText: © 2024 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#include <stdint.h>
#include "dataflow_api.h"

#include "debug/dprint.h"

void kernel_main() {

    // I HAVE ONLY ONE INPUT BUFFER, that goes to CB c_0 

    uint32_t src0_addr = get_arg_val<uint32_t>(0);
    uint32_t num_tiles = get_arg_val<uint32_t>(1);

    //uint64_t src0_noc_addr = get_noc_addr_from_bank_id<true>(src0_bank_id, src0_addr);
    //uint64_t src1_noc_addr = get_noc_addr_from_bank_id<true>(src1_bank_id, src1_addr);

    constexpr uint32_t cb_id_in0 = tt::CBIndex::c_0;
    
    // PERCHÈ PRENDO IL TILE SIZE DALLA CB?
    constexpr uint32_t onetile = 1;
    const uint32_t src0_tile_bytes = get_tile_size(cb_id_in0);
    const tt::DataFormat src0_data_format = get_dataformat(cb_id_in0);

    // is true because src0 is uploaded in DRAM
    const InterleavedAddrGenFast<true> s0 = {
        .bank_base_address = src0_addr, .page_size = src0_tile_bytes, .data_format = src0_data_format};

    // read ublocks from src0/src1 to CB0/CB1, then push ublocks to compute (unpacker)

    // I tiles dovrebbero essere 64, però io ne ho allocati solo due per ogni CB.
    // Probabilmente per la versione single core il numero di tiles in CB deve corrispondere al numero di tiles in dram
    // Poi per la versionee multicore posso fare cose pazze

    for (uint32_t tile = 0; tile < num_tiles; tile++) {
        {  // Read A's tile at (mt, kt)
            cb_reserve_back(cb_id_in0, onetile);
            uint32_t l1_write_addr_in0 = get_write_ptr(cb_id_in0);
            noc_async_read_tile(tile, s0, l1_write_addr_in0);
            noc_async_read_barrier();
            cb_push_back(cb_id_in0, onetile);
        }
        // DPRINT << "Pushed itileA=" << itileA << " itileB=" << itileB << ENDL()
    }  
}
