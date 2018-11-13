#include "FILTERS.hpp"
#include <stdio.h>

#include <algorithm>

fir_filter::fir_filter( std::vector<fc32_t> _coef ) {
    coef = _coef; // copy
    taps.resize(coef.size());
    // zero out prev samples to start.
    std::fill(taps.begin(), taps.end(), (fc32_t){ 0,0 } );
};

void fir_filter::operator()( std::vector<fc32_t> input, std::vector<fc32_t> &output ) {
    // insure output buffer is correct size.
    if ( output.size() != input.size() ) {
        printf("Fir filter resizing output buffer..\n");
        output.resize( input.size() );
    }
 
    int idx;
    // loop over all input samples
    for (idx=0; idx < input.size(); idx++) {
        //  shift samples in taps
        for (int t=coef.size()-1; t > 0; t-- ) {
            //printf("t = %d\n", t );
            taps[t] = taps[t-1];
        }
        // add in working sample..
        taps[0] = input[idx];
        // init output as 0 (just in case..)
        output[idx] = {0,0};
        // compute output sample, based on values in taps and coef
        for (int c=0; c < coef.size(); c++ ) {
            output[idx].i += (coef[c].i*taps[c].i);
            output[idx].q += (coef[c].q*taps[c].q);
        }
    }
};

