#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>
#include <stdlib.h>

/* CPU Registers*/
typedef struct Registers
{
    /* data */
    char A; 
    char BC[2]; 
    char DE[2];
    char HL[2];
    
    unsigned char F;
} Registers;

/* Initialize registers */
Registers* initRegs();


#endif