#pragma once

#include <memory>
#include <vector>
#include <cstdint>

class PRBS_11_GEN {
        uint32_t reg;
    public:
        PRBS_11_GEN();
        void generate( std::vector<uint8_t> &output );
};

