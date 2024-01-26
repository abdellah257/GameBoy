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

void NOP_(){
    Z80->m = 1; Z80->t = 4;
}

void LD_imm(unsigned char* r16, unsigned short imm16){

    unsigned short temp = imm16;

    r16[0] = temp & 0xFF;
    r16[1] = (temp >> 8) & 0xFF;

    Z80->m = 3; Z80->t = 12;
}

void LD_mm_A(unsigned short addr){

    char val = Z80->R->A;

    wb(addr, val);
    
}

void LD_A_mm(unsigned short addr){

    unsigned char val = rb(addr);

    Z80->R->A = val;

    Z80->m = 2; Z80->t = 4;
}

void LD_sp_imm(unsigned short imm16){
    Z80->sp = imm16;

    Z80->m = 3; Z80->t = 12;
}

void LD_mm_sp(unsigned short addr){

    unsigned char temp[2];
    temp[0] = Z80->sp & 0xFF;
    temp[1] = Z80->sp >> 8;

    ww(addr, temp);

    Z80->m = 5; Z80->t = 12;
}

void INC(char* r8){
    *r8 += 1;
    char temp = *r8;

    if(!temp) Z80->R->F |= Z;
    //if((temp & 0x0F) == 0) Z80->R->F |= H;
    Z80->R->F &= ~C;
    Z80->R->F &= ~S;

    Z80->m = 1; Z80->t = 4;
}

void INC_16(unsigned char* r16){

    unsigned short* pt = (unsigned short*)r16;
    *pt += 1;
    Z80->m = 2; Z80->t = 4;
}

void DEC(char* r8){
    *r8 -= 1;
    char temp = *r8;

    Z80->R->F |= S;

    if(!temp) Z80->R->F |= Z;
    //if((temp & 0x0F) == 0) Z80->R->F |= H;
    Z80->R->F &= ~C;

    Z80->m = 1; Z80->t = 4;
}

void DEC_16(unsigned char* r16){

    unsigned short* pt = (unsigned short*)r16;
    *pt -= 1;
    Z80->m = 2; Z80->t = 4;
}

void ADD_hl_r16(unsigned short* r16){
    unsigned short temp = *r16;

    Z80->R->HL[0] = temp & 0xFF;
    Z80->R->HL[1] = (temp >> 8) & 0xFF;

    Z80->m = 2; Z80->t = 4;
}

void LD_r8_mm8(unsigned char* X, unsigned char imm8){
    *X = imm8;

    Z80->m = 2; Z80->t = 8;
}

// Block 2

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
    int temp = Z80->R->A;
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
