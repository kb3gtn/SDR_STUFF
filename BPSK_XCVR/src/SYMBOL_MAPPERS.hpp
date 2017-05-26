#pragma once

#include <vector>
#include <cstdint>
#include "sdr_types.h"

class BPSK_SYMBOL_MAPPER {
public:
    // bits to 
    enum MAPPING_t {
        normal=0,
        inverted=1
    };

    enum PCM_CODE_t { 
        NRZ_L=0,
        NRZ_M=1
    };

    BPSK_SYMBOL_MAPPER( BPSK_SYMBOL_MAPPER::MAPPING_t m, BPSK_SYMBOL_MAPPER::PCM_CODE_t p );
    
    void map2symbols( std::vector<uint8_t> data_in, std::vector<fc32_t> &syms_out);
    void map2data( std::vector<fc32_t> syms_in, std::vector<uint8_t> &data_out );

private:
    MAPPING_t map;
    PCM_CODE_t pcm;
    // pcm state tracking between map calls.
    int encode_state_last;
    int decode_state_last;
};

