#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>
#include <stdlib.h>

/* CPU Registers*/
typedef struct Registers
{
    /* data */
    uint8_t A; uint8_t B; uint8_t C; uint8_t D;
    uint8_t E; uint8_t F; uint8_t H; uint8_t L;

} Registers;

/* Initialize registers */
Registers* initRegs(){

    Registers* p = (Registers*)malloc(sizeof(Registers));

    p->A = 0; p->B = 0; p->C = 0; p->D = 0;
    p->E = 0; p->F = 0; p->L = 0; p->H = 0;

    return p;
}

#endif