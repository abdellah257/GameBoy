#include <stdint.h>
#include <string.h>

#include <cpu.h>
#include <memoryUnit.h>

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

void resetCPU()
{
    Z80->R->A = 0; Z80->R->BC[0] = 0; Z80->R->BC[1] = 0; Z80->R->DE[0] = 0; 
    Z80->R->DE[1] = 0; Z80->R->F = 0; Z80->R->HL[0] = 0; Z80->R->HL[1] = 0; 

    Z80->pc = 0; Z80->sp = 0;
    Z80->m = 0; Z80->t = 0;
}

/* Instructions */

// Block 0
void NOP_()
{
    Z80->m = 1; Z80->t = 4;
}



void LD_A()
{
    uint16_t addr = rw(Z80->pc);
    Z80->pc += 2;
    Z80->R->A = rb(addr);
    Z80->m = 4; Z80->t = 16;
}

void ADD_A(unsigned char Y)
{
    int temp = Z80->R->A + Y;

    Z80->R->A += Y;
    Z80->R->F = 0x00;

    if(!Z80->R->A) Z80->R->F |= Z;
    if( temp > 255) Z80->R->F |= C;

    Z80->m = 1; Z80->t = 4;
}

void CP_A(unsigned char Y)
{
    char temp = Z80->R->A;
    temp -= Y;
    Z80->R->F |= 0x40;

    if(!(temp & 255)) Z80->R->F |= 0x80;
    if( temp < 0 ) Z80->R->F |= 0x10;

    Z80->m = 1; Z80->t = 4;
}

void PUSH_(char* X, char* Y)
{
    Z80->sp--;
    wb(Z80->sp, *X);
    Z80->sp--;
    wb(Z80->sp, *Y);
    Z80->m = 3; Z80->t = 12;
}

void POP_(char* X, char* Y)
{
    *X = rb(Z80->sp);
    Z80->sp ++;
    *Y = rb(Z80->sp);
    Z80->sp ++;

    Z80->m = 3; Z80->t = 12;
}
