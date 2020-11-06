#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "process.h"








int main(int argc, char** argv) {
     if (argc >= 3){
        if(strcmp(argv[1],"-e") == 0)
        {
          printf("\n**** Encoding *****\n");
          read_byte_to_encode(argc, argv);
        } 
        if(strcmp(argv[1],"-d") == 0)
        {
          printf("\n****   Decoding   ****\n");
          read_byte_to_decode(argc, argv);
        } 
        if(strcmp(argv[1],"-c") == 0)
        {
          printf("\n**** Channel *****\n");
          read_byte_to_channel(argc,argv );
        } 
    }
   /*  else{ 
      printf("\n sufficient arguments, Check README.md");
      uint8_t in = 0x04;
      uint8_t parity = check_parity(in);
      printf("\n before %d", in);
      printf("\n after %d", parity);
      printf("\n noise %d", parity ^ 0x01);
      decode(parity, parity,argv);
     }  */  

    
    return 0;
}

