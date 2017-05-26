#include "PRBS_GENS.hpp"
#include <cstdio>
#include <cstdint>
#include <cctype>

// print memory buffer to screen in nice hex display
void hexdump(void *mem, unsigned int len);


int main() {
    PRBS_11_GEN mygen;
    std::vector<uint8_t> buffer;
    buffer.resize(2048);
    mygen.generate( buffer );
    hexdump( buffer.data(), buffer.size() );
    return 0;
}



//////////////////////////////
// Utitlity functions
//////////////////////////////

// print memory buffer to screen in nice hex display
void hexdump(void *mem, unsigned int len)
{
    unsigned int i, j;
    for(i = 0; i < len + ((len % 32) ? (32 - len % 32) : 0); i++)
    {
        /* print offset */
        if(i % 32 == 0)
        {
            printf("0x%06x: ", i);
        }
        /* print hex data */
        if(i < len)
        {
            printf("%02x ", 0xFF & ((char*)mem)[i]);
        }
        else /* end of block, just aligning for ASCII dump */
        {
            printf("   ");
       }
       /* print ASCII dump */
       if(i % 32 == (32 - 1))
       {
            for(j = i - (31); j <= i; j++)
            {
                if(j >= len) /* end of block, not really printing */
                {
                     putchar(' ');
                }
                else if(isprint(((char*)mem)[j])) /* printable char */
                {
                    putchar(0xFF & ((char*)mem)[j]);        
                }
                else /* other char */
                {
                    putchar('.');
                }
            }
            putchar('\n');
        }
    }
}



