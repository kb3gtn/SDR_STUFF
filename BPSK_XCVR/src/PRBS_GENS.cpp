#include "PRBS_GENS.hpp"

PRBS_11_GEN::PRBS_11_GEN()
{
    reg = 0x7FF; // set bottom 11 bits to '1' -- initialization state
}

void PRBS_11_GEN::generate( std::vector<uint8_t> &output ) {
    // data is stored msb first in ptr
    uint8_t *ptr;
    ptr = output.data();
    int size = output.size();
    int idx=0;
    for (idx=0; idx < size; idx++) {
        uint8_t tap1 = (0x1FE & reg) >> 1;
        uint8_t tap2 = (0x7F8 & reg) >> 3;
        *ptr = tap2;
        ptr++;
        uint8_t feedback = tap1 ^ tap2;
        reg = (reg << 8) | feedback;
    }
}

