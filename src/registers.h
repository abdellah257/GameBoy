#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>
#include <stdlib.h>

/* CPU Registers*/
typedef struct Registers
{
    /* data */
    char A; char B; char C; char D;
    char E; char F; char H; char L;

} Registers;

/* Initialize registers */
Registers* initRegs();

#endif