#include "RESAMPLERS.hpp"

void IntUpSampleSymbols::operator()(std::vector<fc32_t> syms_in, std::vector<fc32_t> &syms_out ) {
    // check that syms_out is N time bigger than syms_in
    if ( (syms_in.size())*upsample_amount > syms_out.size() ) {
        // resize of output required..
        syms_out.resize(syms_in.size()*upsample_amount);
    }
    int src_idx=0;
    int dst_idx=0;
    for (src_idx=0; src_idx < syms_in.size(); src_idx++) {
        syms_out[dst_idx] = syms_in[src_idx];
        dst_idx++;
        for ( int d=0; d < upsample_amount-1; d++ ) {
            // zero insert for upsample.
            syms_out[dst_idx] = (fc32_t){ 0,0 };
            dst_idx++;
        }
    }
}





