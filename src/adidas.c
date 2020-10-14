#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "process.h"








int main(int argc, char** argv) {
    if (argc < 2){
      if(strcmp(argv[1],"-e") == 0)
      {
        printf("\n**** Decoding ******\n");
        read_byte_to_encode(argc, argv);
      } 
      if(strcmp(argv[1],"-d") == 0)
      {
        printf("\n**** Channeling ******\n");
        read_byte_to_decode(argc, argv);
      } 
    }
    uint8_t lsb = 0x13; 
    uint8_t msb = 0x13; 
    decode(lsb, msb, argv);
    return 0;
}

