#include "SYMBOL_MAPPERS.hpp"
#include "PRBS_GENS.hpp"
#include "RESAMPLERS.hpp"
#include "FILTERS.hpp"
#include <cstdio>
#include <cstdint>
#include <cctype>
#include "sdr_types.h"

// prototypes for useful utility functions.
void PrintDataBuffer( std::vector<uint8_t> buffer );
void PrintSampleBuffer( std::vector<fc32_t> buffer );


int main() {
    PRBS_11_GEN prbs11_gen;
    BPSK_SYMBOL_MAPPER bpsk_map( 
            BPSK_SYMBOL_MAPPER::MAPPING_t::normal, 
            BPSK_SYMBOL_MAPPER::PCM_CODE_t::NRZ_L );

    IntUpSampleSymbols up_samp_by_4( 4 );
    std::vector<fc32_t> shape_filter_coef = { (fc32_t){0.05,0.05}, (fc32_t){0.2,0.2}, (fc32_t){0.5,0.5}, (fc32_t){0.2,0.2}, (fc32_t){0.05,0.05} };
    fir_filter shape_filter( shape_filter_coef );

    std::vector<uint8_t> data_buffer1;
    std::vector<fc32_t> symbol_buffer1;
    std::vector<fc32_t> symbol_buffer2;
    std::vector<fc32_t> symbol_buffer3;


    // allocate space in buffer and fill..
    data_buffer1.resize(128);
    prbs11_gen.generate( data_buffer1 );

    // print buffer of data generated.
    printf("Data Generated, 128 bytes\n");
    PrintDataBuffer( data_buffer1 );
    printf("\n");
    printf("Mapping data to modulated signals\n");
    bpsk_map.map2symbols( data_buffer1, symbol_buffer1 );
    printf("Upsampling by 4\n");
    up_samp_by_4( symbol_buffer1, symbol_buffer2 );
    
    printf("Symbols generated, %lu symbols\n", symbol_buffer2.size() );
    PrintSampleBuffer( symbol_buffer2 );
    printf("\n");

    printf("Applying shaping filter..\n");
    shape_filter( symbol_buffer2, symbol_buffer3 );

    printf("Symbols generated, %lu symbols\n", symbol_buffer3.size() );
    PrintSampleBuffer( symbol_buffer3 );
    printf("\n");

    printf("First 10 samples in buffer3\n");
    for (int i=0; i < 10; i++ ) {
        printf(" {%f,%f} ", symbol_buffer3[i].i, symbol_buffer3[i].q );
    }


    printf("\nDone..\n");

 
    return 0;
}


////////////////////////////////////
// Utility Functions
///////////////////////////////////

// Print buffer contents as a hex dump display to the screen.
void PrintDataBuffer( std::vector<uint8_t> buffer ) {
    uint8_t *mem= buffer.data();
    int len = buffer.size();
    unsigned int i,j;
    for(i = 0; i < len + ((len % 32) ? (32 - len % 32) : 0); i++) {
        if(i % 32 == 0) {
            printf("0x%06x: ", i);
        }
        if(i < len) {
            printf("%02x ", 0xFF & ((char*)mem)[i]);
        } else {
            printf("   ");
        }
        if(i % 32 == (32 - 1)) {
            for(j = i - (31); j <= i; j++) {
                if(j >= len) {
                     putchar(' ');
                }
                else if(isprint(((char*)mem)[j])) {
                    putchar(0xFF & ((char*)mem)[j]);        
                }
                else {
                    putchar('.');
                }
            }
            putchar('\n');
        }
    }
}


void PrintSampleBuffer( std::vector<fc32_t> buffer ) {
    fc32_t *sample = buffer.data();
    int len = buffer.size();
    char iline[512];
    char qline[512];
    int idx=0;
    int line_idx = 0;
    int lo = 23; // line offset for printing
    for (idx=0; idx < len; idx++) {
        float i=sample->i;
        float q=sample->q;
        // every start of line...
        if ( ( idx % 128 ) == 0 ) {
            if ( idx != 0 ) {
                // if not first one, print line buffers to screen.
                printf("%s\x1b[0m\n", iline );
                printf("%s\x1b[0m\n", qline );
            }
            // print line headers
            sprintf(&iline[0], "\x1b[1;33;40m%6X-I \xe2\x8e\xaf         ", idx );
            sprintf(&qline[0], "\x1b[1;32;40m%6X-Q \xe2\x8e\xaf         ", idx );
            line_idx = 0;
        }
        // print sample unicode representation to buffer.
        if ( i >= 0.75 ) { 
            sprintf(&iline[lo+(line_idx*3)], "\xe2\x8e\xba" ); // high line
        } else if ( ( i < 0.75 ) && ( i >= 0.25 ) ) {
            sprintf(&iline[lo+(line_idx*3)], "\xe2\x8e\xbb" ); // mid high line
        } else if ( ( i < 0.25 ) && ( i > -0.25 ) ) {
            sprintf(&iline[lo+(line_idx*3)], "\xe2\x8e\xbc" ); // mid line
        } else if ( ( i > -0.75 ) && ( i < -0.25 ) ) {
            sprintf(&iline[lo+(line_idx*3)], "\xe2\x8e\xbd" ); // mid low line
        } else {
            sprintf(&iline[lo+(line_idx*3)], "\xe2\x80\x97" ); // low line
        }
        if ( q > 0.75 ) { 
            sprintf(&qline[lo+(line_idx*3)], "\xe2\x8e\xba" ); // high line
        } else if ( ( q < 0.75 ) && ( q >= 0.25 ) ) {
            sprintf(&qline[lo+(line_idx*3)], "\xe2\x8e\xbb" ); // mid high line
        } else if ( ( q < 0.25 ) && ( q > -0.25 ) ) {
            sprintf(&qline[lo+(line_idx*3)], "\xe2\x8e\xbc" ); // mid line
        } else if ( ( q > -0.75 ) && ( q < -0.25 ) ) {
            sprintf(&qline[lo+(line_idx*3)], "\xe2\x8e\xbd" ); // mid low line
        } else {
            sprintf(&qline[lo+(line_idx*3)], "\xe2\x80\x97" ); // low line
        }
         // next location in the line.. 
        line_idx++;
        sample++; // point to next sample.
    } // end idx for loop.
    // flush last line..
    printf("%s\x1b[0m\n", iline );
    printf("%s\x1b[0m\n", qline );
}

