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

void RLCA(){

    unsigned char temp = Z80->R->A >> 7;
    Z80->R->A = (Z80->R->A << 1) | (Z80->R->A >> 7);
    if(! temp) Z80->R->F |= C;

    Z80->m = 1; Z80->t = 4;
}

void RRCA(){

    unsigned char temp = Z80->R->A << 7;
    Z80->R->A = (Z80->R->A >> 1) | (Z80->R->A << 7);
    if(! (temp >> 7)) Z80->R->F |= C;

    Z80->m = 1; Z80->t = 4;
}

void RLA(){

    if(Z80->R->F & C == C){
        Z80->R->A = (Z80->R->A << 1) | 0x01 ;
    }
    else{
        Z80->R->A = (Z80->R->A << 1);
    }
    Z80->m = 1; Z80->t = 4;
}

void RRA(){

    if(Z80->R->F & C == C){
        Z80->R->A = (Z80->R->A >> 1) | 0x80 ;
    }
    else{
        Z80->R->A = (Z80->R->A >> 1);
    }
    Z80->m = 1; Z80->t = 4;
}

void DAA(){

}

void SCF(){
    Z80->R->F |= C;
    Z80->R->F &= ~S;
    Z80->R->F &= ~H;

    Z80->m = 1; Z80->t = 4;
}

void CCF(){
    Z80->R->F = ~(Z80->R->F & C);
    Z80->R->F &= ~S;
    Z80->R->F &= ~H;

    Z80->m = 1; Z80->t = 4;
}

void CPL(){
    Z80->R->A = ~Z80->R->A;
    Z80->R->F |= H;
    Z80->R->F |= S;

    Z80->m = 1; Z80->t = 4;
}

void JR(uint16_t addr){

    uint16_t offset = Z80->pc - addr;

    if ( offset < 128 | offset > -127){
        Z80->pc += offset;
    }

    Z80->m = 3; Z80->t = 8;
}

void JR_C(char cond, uint16_t addr){

    switch(cond){

        case Z:
            if(Z80->R->F & Z == Z){
                JR(addr);
            }
            else{
                Z80->m = 2;
            }
            break;
        case ~Z:
            if(Z80->R->F & Z == N){
                JR(addr);
            }
            else{
                Z80->m = 2;
            }
            break;
        case C:
            if(Z80->R->F & Z == Z){
                JR(addr);
            }
            else{
                Z80->m = 2;
            }
            break;
        case ~C:
            if(Z80->R->F & Z == Z){
                JR(addr);
            }
            else{
                Z80->m = 2;
            }
            break;
        default:
            Z80->m = 2;
            break;
    }
}

// Block 1

void LD_(unsigned char* X, unsigned char* Y){
    /* LD X Y  load Y(copy) on X */
    unsigned char temp = *Y;
    *X = temp;
    Z80->m = 1;
}

void HALT(){
    // More complicated
    // Leave for later 
}

// Block 2

void ADD_A_r8(unsigned char Y)
{
    int temp = Z80->R->A + Y;

    Z80->R->A += Y;

    if(!Z80->R->A) Z80->R->F |= Z;
    if( temp > 255) Z80->R->F |= C;

    Z80->m = 1; Z80->t = 4;
}

void ADC_A_r8(unsigned char r8){
    int temp = Z80->R->A + r8;

    Z80->R->A += r8;

    if(Z80->R->F & C == C){
        Z80->R->A += 1;
    }

    if(!Z80->R->A) Z80->R->F |= Z;
    if( temp > 255) Z80->R->F |= C;

    Z80->m = 1; Z80->t = 4;
}

void SUB_A_r8(unsigned char r8){

    if(r8 > Z80->R->A){
        Z80->R->F |= C;
    }
    Z80->R->F |= S;

    int temp = Z80->R->A - r8;
    if(!temp) Z80->R->F |= Z;

    Z80->R->A = (unsigned char)(temp & 0xFF);
    Z80->m = 1;
}

void SBC_A_r8(unsigned char r8){

    unsigned char carry = (Z80->R->F & C == C)? 1:0;
    if(r8 + carry > Z80->R->A ){
        Z80->R->F |= C;
    }

    int temp = Z80->R->A - r8 - carry;
    if(!temp) Z80->R->F |= Z;
    Z80->R->F |= S;

    Z80->R->A = temp & 0xFF;
    Z80->m = 1;
}

void AND_A_r8(unsigned char r8){

    Z80->R->A &= r8;

    if(!Z80->R->A) Z80->R->F |= Z;
    Z80->R->F |= H;

    Z80->m = 1;
}

void XOR_A_r8(unsigned char r8){

    Z80->R->A ^= r8;
    if(!Z80->R->A) Z80->R->F |= Z;

    Z80->m = 1;
}

void OR_A_r8(unsigned char r8){
    Z80->R->A |= r8;
    if(!Z80->R->A) Z80->R->F |= Z;

    Z80->m = 1;
}

void CP_A_r8(unsigned char Y)
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
