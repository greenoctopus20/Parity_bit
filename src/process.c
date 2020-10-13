#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "process.h"





uint8_t check_parity(uint8_t nib)
{   
    uint8_t byte ;
    uint8_t p0 ;
    uint8_t p1 ;
    uint8_t p2 ;

    /* Assigning parity */
    p0 = (((nib & 0x01) + (((nib >> 1)  & 0x01)) + (((nib >> 2)  & 0x01))) % 2 == 0) ? 0x00 : 0x01;
    p1 = (((nib & 0x01) + (((nib >> 1)  & 0x01)) + (((nib >> 3)  & 0x01))) % 2 == 0) ? 0x00 : 0x01;
    p2 = (((((nib >> 1)  & 0x01)) + (((nib >> 2)  & 0x01)) + (((nib >> 3)  & 0x01))) % 2 == 0) ? 0x00 : 0x01;

    byte = (((((nib << 1) | p2 ) << 1) | p1) << 1 ) | p0;
    
    return byte;
}

void read_byte()
{
    
}

void write_byte(uint8_t byte)
{
    
}
