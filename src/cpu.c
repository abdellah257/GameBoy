#include <cpu.h>



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

void ADD_(char* X, char* Y){
    *X += *Y;
    Z80->R->F = 0;
    if(!(*X & 255)) Z80->R->F |= 0x80;
    if(!(*X > 255)) Z80->R->F |= 0x40;
    *X &= 255;
    Z80->m = 1; Z80->t = 4;
}