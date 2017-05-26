#pragma once

#include <vector>
#include <cstdint>
#include "sdr_types.h"

class fir_filter {
private:
    std::vector<fc32_t> coef;
    std::vector<fc32_t> taps;
public:
    fir_filter( std::vector<fc32_t> _coef );
    void operator()( std::vector<fc32_t> input, std::vector<fc32_t> &output );
};

