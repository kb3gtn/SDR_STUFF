#include "FILTERS.hpp"

#include <algorithm>
#include <cstdio>

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
    //printf("STARTUP TAPS = [ ");
    //for (int n=0; n < taps.size(); n++) {
    //    printf(" {%f, %f} ", taps[n].i, taps[n].q );
    // }
    //printf(" ]\n");
    //printf("Filter has %d taps\n", (int)taps.size() );
 
    int idx;
    // loop over all input samples
    for (idx=0; idx < input.size(); idx++) {
        //printf("IDX = %d\n", idx );
        //  shift samples in taps
        for (int t=coef.size()-1; t > 0; t-- ) {
            //printf("t = %d\n", t );
            taps[t] = taps[t-1];
        }
        // add in working sample..
        taps[0] = input[idx];
        //printf("TAPS = [ ");
        //for (int n=0; n < taps.size(); n++) {
        //    printf(" {%f, %f} ", taps[n].i, taps[n].q );
        //}
        //printf(" ]\n");
 
        output[idx] = {0,0};
        // compute output sample, based on values in taps 
        for (int c=0; c < coef.size(); c++ ) {
            output[idx].i += (coef[c].i*taps[c].i);
            output[idx].q += (coef[c].q*taps[c].q);
        }
    }
};

