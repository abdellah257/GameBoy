#ifndef CPU_H
#define CPU_H

#include <registers.h>
#include <stdint.h>

/* Z80 CPU Structure */
typedef struct CPU {
    /* data */
    struct Registers* R;

    uint8_t pc; uint8_t sp;
    uint8_t m; uint8_t t;

} CPU;

CPU *Z80;
/* Initialize the CPU */
CPU* initCPU()
{
    CPU* p = malloc(sizeof(CPU));
    p->R = initRegs();
    p->m = 0;
    p->t = 0;
    p->pc = 0;
    p->sp = 0;
    return p;
}

/* Add Register Y to Register X (ADD X Y) */
void ADD_(uint8_t* X, uint8_t* Y){
    *X += *Y;
    Z80->R->F = 0;
    if(!(*X & 255)) Z80->R->F |= 0x80;
    if(!(*X > 255)) Z80->R->F |= 0x40;
    *X &= 255;
    Z80->m = 1; Z80->t = 4;
}

#endif
