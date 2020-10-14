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
    printf("\n=== RESULT: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte));
    return byte;
}

void read_byte_to_encode(int argc, char** argv)
{
    uint8_t lsb , msb;
    uint8_t byte;
    uint8_t resp;
    FILE *fp;
    fp = fopen(argv[2], "r");
    if(fp == NULL){ printf("Error opening file : %s", argv[2]); return 0; }
    resp = fread(&byte, 1, 1, fp);
    while(resp != 0)
    {
        printf("\n--- READING: %d", byte);
        lsb = byte  & 0x0F;
        msb = ((byte >> 4) & 0x0F);
        lsb = check_parity(lsb); msb = check_parity(msb);
        write_byte(argv, msb);
        write_byte(argv, lsb); 
        resp = fread(&byte, 1, 1, fp);
    }
}


void write_byte(char** argv, uint8_t byte)
{
    FILE *fp;
    fp = fopen(argv[3], "a");
    if(fp == NULL)
    {
        printf("Error opening file : %s", argv[2]);
        return 0; 
    }
    fwrite(&byte, 1, 1, fp);
    printf("\n+++ WRITING: %d", byte);    
    
}

void read_byte_to_decode(int argc, char** argv)
{
    uint8_t lsb , msb;
    uint8_t byte;
    uint8_t resp;
    FILE *fp;
    fp = fopen(argv[2], "r");
    if(fp == NULL){ printf("Error opening file : %s", argv[2]); return 0; }
    resp = fread(&msb, 1, 1, fp);
    printf("\n--- READING TO DECODE---"); 
    while(resp != 0)
    {
        resp = fread(&lsb, 1, 1, fp);
        if (resp != 0) { decode(msb, lsb, argv);}
        resp = fread(&msb, 1, 1, fp);
    }
}

void decode(uint8_t lsb, uint8_t msb, char** argv)
{   
    uint8_t full_byte[2] = {msb, lsb};
    printf("\nlsb %d", lsb);
    printf("\nmsb %d", lsb);
    /* Decode and concatinate and write */
    int i = 0;
    for (i = 0; i <= 1; i++)
    {   
        int p0_check = 0;  /* true partiy  */
        int p1_check = 0;
        int p2_check = 0;
        uint8_t d0 = (full_byte[i] >> 3)  & 0x01;
        uint8_t d1 = (full_byte[i] >> 4)  & 0x01;
        uint8_t d2 = (full_byte[i] >> 5)  & 0x01;
        uint8_t d3 = (full_byte[i] >> 6)  & 0x01;
        if ((full_byte[i] & 0x01) != (d0 ^ d1 ^ d2)) { p0_check = 1;}  /* false parity */
        if (((full_byte[i] >> 1)  & 0x01) != (d0 ^ d1 ^ d3)) { p1_check = 1;}
        if (((full_byte[i] >> 2)  & 0x01) != (d1 ^ d2 ^ d3)) { p2_check = 1;}

        printf("\nbyte %d", full_byte[i]);
        printf("\np0 %d", (full_byte[i] & 0x01));
        printf("\np1 %d", ((full_byte[i] >> 1)  & 0x01));
        printf("\np2 %d", ((full_byte[i] >> 2)  & 0x01));
        printf("\n ***********");
        
    }
}

