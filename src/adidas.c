#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "process.h"


#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 





int main(int argc, char** argv) {
    uint8_t input = 0x41;
    uint8_t lsb , msb;
    
    lsb = input  & 0x0F;
    msb = ((input >> 4) & 0x0F);
    
    
    printf("\n RESULT : "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(check_parity(lsb)));
    printf("\n RESULT : "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(check_parity(msb)));
    

    return 0;
}

