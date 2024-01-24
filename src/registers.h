#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>
#include <stdlib.h>

#define Z 0x80  // Zero Flag
#define S 0x40  // Substraction Flag
#define H 0x20  // Half Carry Flag
#define C 0x10  // Carry Flag

/* CPU Registers*/
typedef struct Registers
{
    /* data */
    unsigned char A; 
    unsigned char BC[2]; 
    unsigned char DE[2];
    unsigned char HL[2];
    
    unsigned char F;
} Registers;

/* Initialize registers */
Registers* initRegs();


#endif