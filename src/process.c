#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include "process.h"
#define range 7


int count_noise[8] = { 0 } ;

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
        
        lsb = byte  & 0x0F;
        msb = ((byte >> 4) & 0x0F);
        lsb = check_parity(lsb);
        msb = check_parity(msb);
        write_byte(argv, msb);
        write_byte(argv, lsb); 
        resp = fread(&byte, 1, 1, fp);
    }
    printf("\nDecoding is Done  \n");
}


void write_byte(char** argv, uint8_t byte)
{
    FILE *fp;
    fp = fopen(argv[3], "a");
    if(fp == NULL)
    {
        printf("Error opening file : %s", argv[3]);
        return 0; 
    }
    fwrite(&byte, 1, 1, fp);
    fclose(fp);
    
    
}

void read_byte_to_decode(int argc, char** argv)
{
    printf("\n--- READING TO DECODE---"); 
    uint8_t lsb , msb;
    uint8_t byte;
    uint8_t resp;
    FILE *fp;
    fp = fopen(argv[2], "r");
    if(fp == NULL){ printf("Error opening file : %s", argv[2]); return 0; }
    resp = fread(&lsb, 1, 1, fp);
    
    while(resp != 0)
    {
        resp = fread(&msb, 1, 1, fp);
        if (resp != 0) { decode(msb, lsb, argv);}
        resp = fread(&lsb, 1, 1, fp);
    }
    fclose(fp);
}

void decode(uint8_t lsb, uint8_t msb, char** argv)
{   
    uint8_t full_byte[2] = {msb, lsb};
    uint8_t byte_write = 0x00;
    /* Decode and concatinate and write */

    int i = 0;
    for (i = 0; i <= 1; i++)
    {   
         
        int p0_check = 1;  /* true partiy  */
        int p1_check = 1;
        int p2_check = 1;
        uint8_t d0 = (full_byte[i] >> 3)  & 0x01;
        uint8_t d1 = (full_byte[i] >> 4)  & 0x01;
        uint8_t d2 = (full_byte[i] >> 5)  & 0x01;
        uint8_t d3 = (full_byte[i] >> 6)  & 0x01;
        
        /* false parity  = -1 */
        if ((full_byte[i] & 0x01) != (d0 ^ d1 ^ d2)) { p0_check = -1; }  
        if (((full_byte[i] >> 1)  & 0x01) != (d0 ^ d1 ^ d3)) { p1_check = -1;}
        if (((full_byte[i] >> 2)  & 0x01) != (d1 ^ d2 ^ d3)) { p2_check = -1;}


        /* fixing false parity */
        if(p0_check == -1 && p1_check == -1 && p2_check == -1) 
        {
            
            d1 = d1 ^ 0x01;
            
        }
        else
        {
            if(p0_check == -1 && p1_check == -1 ) 
            {
                d0 = d0 ^ 0x01;
            
            }
            else
            {
                if(p0_check == -1 && p2_check == -1 )
                {
                    d2 = d2 ^ 0x01;
            
                }
                else
                {
                    if(p1_check == -1 && p2_check == -1 )
                    {
                         d3 = d3 ^ 0x01;
            
                    }
                }   
            }    
        }
        
        
        byte_write = ((((( ((byte_write << 1) | d3) << 1) | d2) << 1) | d1) << 1) | d0;
        
    }
    
    write_byte(argv, byte_write);
}

void read_byte_to_channel (int argc, char** argv)
{
    
    uint8_t masks [8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40 , 0x60};
    int random;
    uint8_t byte;
    uint8_t resp;
    int read_count = 0;
    FILE *fp;
    fp = fopen(argv[2], "r");
    if(fp == NULL){ printf("Error opening file : %s", argv[2]); return 0; }
    resp = fread(&byte, 1, 1, fp);
    
    srand(time(NULL));
    
    while(resp != 0)

    {    
        read_count++;
        
        if (read_count % 2 == 0)
        {
            random = rand() % (range);
            count_noise[random]++;
            byte = byte ^ masks[random];
        }
        write_byte(argv, byte);
        resp = fread(&byte, 1, 1, fp);
    }
    fclose(fp);
    printf("\n---------------------------");
    printf("\nChannel frequency");
    printf("\n---------------------------");
    printf("\np0\tp1\tp2");
    printf("\n---------------------------");
    printf("\n%d\t%d\t%d", count_noise[0], count_noise[1],count_noise[2]);
    printf("\n---------------------------");
    printf("\nd0\td1\td2\td3");
    printf("\n---------------------------");
    printf("\n%d\t%d\t%d\t%d", count_noise[3], count_noise[4], count_noise[5], count_noise[6]);
    printf("\n---------------------------");
    printf("\n\nChanneling is Done\n");
}