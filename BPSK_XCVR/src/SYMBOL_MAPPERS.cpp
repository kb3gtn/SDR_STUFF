#include "SYMBOL_MAPPERS.hpp"

// for debug
#include <cstdio>

BPSK_SYMBOL_MAPPER::BPSK_SYMBOL_MAPPER(BPSK_SYMBOL_MAPPER::MAPPING_t m, BPSK_SYMBOL_MAPPER::PCM_CODE_t p )
{
    map = m;
    pcm = p;
    encode_state_last = 0;
    decode_state_last = 0;
}

// Map data bytes into modulated Baseband symbols.
void BPSK_SYMBOL_MAPPER::map2symbols( std::vector<uint8_t> data_in, std::vector<fc32_t> &syms_out )
{
    // resize syms if needed to fit data into it.
    // 1 data_in element will produce 8 output symbols elements.
    int size_src = data_in.size();
    int size_dst = syms_out.size();

    if ( size_dst != size_src*8 ) {
        syms_out.resize(size_src*8);
    }

    int idx = 0;
    int bitidx = 8;
    fc32_t *sym = syms_out.data();
    for ( idx; idx < size_src; idx++ ) {
        for (bitidx=8; bitidx > 0; bitidx-- ) {
            // mask bit..
            int working_bit = ( data_in[idx] & ( 1 << (bitidx-1) ) );
            if (working_bit != 0) {
                // input is a '1'
                if ( pcm == PCM_CODE_t::NRZ_L ) {
                    // NRZL a sym = bit.
                    *sym = { 1, 0 };
                }
                if ( pcm == PCM_CODE_t::NRZ_M ) {
                    // NRZM a sym is based on the current bit and the last encode state.
                    // NRZ-M '1' -- Toggle output, '0' -- keep same level.
                    if (encode_state_last == 0) {
                        encode_state_last = 1;
                        *sym = { 1, 0 };
                    } else {
                        encode_state_last = 0;
                        *sym = { -1, 0 };
                    }
                }                   
            } else {
                // input is a '0'
                if ( pcm == PCM_CODE_t::NRZ_L ) {
                    *sym = { -1, 0 };
                }
                if ( pcm == PCM_CODE_t::NRZ_M ) {
                    if ( encode_state_last == 0 ) {
                        *sym = {-1,0};
                    } else {
                        *sym = {1,0};
                    }
                }
            }
            //printf("Wrote %f,%f to sym.\n", sym->i, sym->q );
            sym++; // point to next sym in buffer.
        } // end bitidx loop
    } // end data byte loop
}


// Take received band band symbols
// Assumes syms_in is a multiple of 8. (8 syms per byte)
void BPSK_SYMBOL_MAPPER::map2data( std::vector<fc32_t> syms_in, std::vector<uint8_t> &data_out )
{
    // resize syms if needed to fit data into it.
    // 1 data_in element will produce 8 output symbols elements.
    int size_src = syms_in.size();
    int size_dst = data_out.size();

    // Check that syms_in is a multiple of 8 elements
    if ( size_src % 8 != 0 ) {
        data_out.clear();  // output buffer will be zero lengthed..
        return;
    }

    // resize output buffer so it is 1/8 the number of symbol buffer
    // 8 symbols per byte.
    if ( size_dst != size_src/8 ) {
        data_out.resize(size_src/8);
    }

    int sym_idx = 0;
    uint8_t wb = 0; // working byte
    int bit_idx = 8; // working bit.

    for ( sym_idx=0; sym_idx < size_src; sym_idx++ ) {
        wb = 0;
        for ( bit_idx=8; bit_idx > 0; bit_idx--) {
            if ( syms_in[sym_idx].i > 0 ) {
                // have a +1 received.
                if ( pcm == PCM_CODE_t::NRZ_L ) {
                    wb |= (1 << (bit_idx-1)); // set the bit in the byte..
                }
                if (pcm == PCM_CODE_t::NRZ_M) {
                    // input symbol toggle?
                    if ( decode_state_last == 0 ) {
                        decode_state_last = 1; // toggle..
                        wb |= (1 << (bit_idx-1) ); // output a '1' 
                    } else {
                        decode_state_last = 0; // toggle state.
                        // outputs a zero.
                    }
                }
            } else {
                // have a -1 received (0).
                // if pcm == PCM_CODE_t::NRZ_L -- leave as '0' bit..
                if ( pcm == PCM_CODE_t::NRZ_M ) {
                    if ( decode_state_last == 1 ) {
                        // output a '1' 
                        wb |= (1 << (bit_idx-1)); // output a '1'
                    } 
                }
            }              
        }
    }
}


