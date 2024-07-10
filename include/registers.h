#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>
#include <stdlib.h>

#define FN 0x00 // NULL
#define FZ 0x80 // Zero Flag
#define FS 0x40 // Substraction Flag
#define FH 0x20 // Half Carry Flag
#define FC 0x10 // Carry Flag

/* CPU Registers*/
typedef struct Registers
{
    /* data */
    uint8_t A;
    /* 8 bits registers */
    uint8_t B; uint8_t C;
    uint8_t D; uint8_t E;
    uint8_t H; uint8_t L;
    uint8_t F;

    /* 16 bits registers */
    uint16_t AF; uint16_t BC;
    uint16_t DE; uint16_t HL;

    uint16_t SP;
    uint16_t PC;

} Registers;

/* Initialize registers */
Registers *initRegs();

void resetRegs(Registers *p);

#endif