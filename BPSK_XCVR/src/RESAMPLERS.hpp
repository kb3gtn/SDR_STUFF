#pragma once

#include <vector>
#include <cstdint>
#include "sdr_types.h"

class IntUpSampleSymbols {
    int upsample_amount;
public:
    IntUpSampleSymbols( int upsample_amount ) : upsample_amount(upsample_amount) {}
    void operator()( std::vector<fc32_t> syms_in, std::vector<fc32_t> &syms_out );
};

//class IntUpSampleData {
//    int upsample_amount;
//public:
//    IntUpSampleData( int upsample_amount );
//    void operator()( std::vector<uint8_t> data_in, std::vector<uint8_t> data_out );
//};
//
//class FacUpSampleSymbols {
//    float upsample_word;
//public:
//    FacUpSampleSymbols( float upsample_ratio );
//    void operator()( std::vector<fc32_t> syms_in, std::vector<fc32_t> syms_out );
//};
//
//class FacUpSampleData {
//    float upsample_word;
//public:
//    FacUpSampleSymbols( float upsample_ratio );
//    void operator()( std::vector<uint8_t> data_in, std::vector<uint8_t> data_out );
//};


