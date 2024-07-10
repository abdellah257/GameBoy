#include <stdint.h>
#include <string.h>

#include <cpu.h>
#include <memoryUnit.h>

CPU *Z80;

CPU* initCPU()
{
    CPU* p = malloc(sizeof(CPU));
    p->R = initRegs();
    p->m = 0;
    p->t = 0;
    return p;
}

void resetCPU()
{
    resetRegs(Z80->R);
    Z80->m = 0; Z80->t = 0;
}

/* Instructions */

// Block 0

void NOP_(){
    Z80->m = 1; Z80->t = 4;
}

void LD_r16_imm(uint16_t *r16, uint16_t imm16){
    uint16_t temp = imm16;
    r16[0] = temp & 0xFF;
    r16[1] = (temp >> 8) & 0xFF;
    Z80->m = 3; Z80->t = 12;
}

void LD_mem_A(uint16_t addr){
    char val = Z80->R->A;
    wb(addr, val);    
}

void LD_A_mem(uint16_t addr){
    uint8_t val = rb(addr);
    Z80->R->A = val;
    Z80->m = 2; Z80->t = 4;
}

void LD_mem_sp(uint16_t addr){
    uint8_t temp[2];
    temp[0] = Z80->R->SP & 0xFF;
    temp[1] = Z80->R->SP >> 8;
    ww(addr, temp);
    Z80->m = 5; Z80->t = 12;
}

void LD_sp_imm(uint16_t imm16){
    Z80->R->SP = imm16;
    Z80->m = 3; Z80->t = 12;
}

void INC_8(uint8_t* r8){
    *r8 += 1;
    char temp = *r8;
    if(!temp) Z80->R->F |= FZ;
    //if((temp & 0x0F) == 0) Z80->R->F |= FH;
    Z80->R->F &= ~FC;
    Z80->R->F &= ~FS;
    Z80->m = 1; Z80->t = 4;
}

void INC_16(uint16_t *r16){
    uint16_t* pt = (uint16_t*)r16;
    *pt += 1;
    Z80->m = 2; Z80->t = 4;
}

void DEC_8(uint8_t *r8){
    *r8 -= 1;
    char temp = *r8;
    Z80->R->F |= FS;
    if(!temp) Z80->R->F |= FZ;
    //if((temp & 0x0F) == 0) Z80->R->F |= FH;
    Z80->R->F &= ~FC;
    Z80->m = 1; Z80->t = 4;
}

void DEC_16(uint16_t *r16){
    uint16_t* pt = (uint16_t*)r16;
    *pt -= 1;
    Z80->m = 2; Z80->t = 4;
}

void ADD_HL_r16(uint16_t* r16){
    // TODO 
    Z80->m = 2; Z80->t = 4;
}

void LD_r8_imm(uint8_t* X, uint8_t imm8){
    *X = imm8;
    Z80->m = 2; Z80->t = 8;
}

void RLCA(){
    uint8_t temp = Z80->R->A >> 7;
    Z80->R->A = (Z80->R->A << 1) | (Z80->R->A >> 7);
    if(! temp) Z80->R->F |= FC;
    Z80->m = 1; Z80->t = 4;
}

void RRCA(){
    uint8_t temp = Z80->R->A << 7;
    Z80->R->A = (Z80->R->A >> 1) | (Z80->R->A << 7);
    if(! (temp >> 7)) Z80->R->F |= FC;
    Z80->m = 1; Z80->t = 4;
}

void RLA(){
    if((Z80->R->F & FC) == FC){
        Z80->R->A = (Z80->R->A << 1) | 0x01 ;
    }else{
        Z80->R->A = (Z80->R->A << 1);
    }
    Z80->m = 1; Z80->t = 4;
}

void RRA(){
    if((Z80->R->F & FC) == FC){
        Z80->R->A = (Z80->R->A >> 1) | 0x80 ;
    }else{
        Z80->R->A = (Z80->R->A >> 1);
    }
    Z80->m = 1; Z80->t = 4;
}

void DAA(){

}

void SCF(){
    Z80->R->F |= FC;
    Z80->R->F &= ~FS;
    Z80->R->F &= ~FH;
    Z80->m = 1; Z80->t = 4;
}

void CCF(){
    Z80->R->F = ~(Z80->R->F & FC);
    Z80->R->F &= ~FS;
    Z80->R->F &= ~FH;
    Z80->m = 1; Z80->t = 4;
}

void CPL(){
    Z80->R->A = ~Z80->R->A;
    Z80->R->F |= FH;
    Z80->R->F |= FS;
    Z80->m = 1; Z80->t = 4;
}

void JR(uint16_t addr){
    uint16_t offset = Z80->R->PC - addr;
    if ( (offset < 128) | (offset > -127)){
        Z80->R->PC += offset;
    }
    Z80->m = 3; Z80->t = 8;
}

void JR_C(char cond, uint16_t addr)
{
    if((cond == FZ) && ((Z80->R->F & FZ) == FZ)){
        JR(addr);
    }else if((cond == ~FZ) && ((Z80->R->F & FZ) == FN)){
        JR(addr);
    }else if((cond == FC) && ((Z80->R->F & FC) == FC)){
        JR(addr);
    }else if((cond==~FC) && ((Z80->R->F & FC) == FN)){
        JR(addr);
    }else{
        Z80->m = 2;
    }
}

// Block 1

void LD_r8_r8(uint8_t* X, uint8_t* Y){
    /* LD X Y  load Y(copy) on X */
    uint8_t temp = *Y;
    *X = temp;
    Z80->m = 1;
}

void HALT(){
    // More complicated
    // Leave for later 
}

// Block 2

void ADD_A_r8(uint8_t Y)
{
    int temp = Z80->R->A + Y;
    Z80->R->A += Y;
    if(!Z80->R->A) Z80->R->F |= FZ;
    if( temp > 255) Z80->R->F |= FC;
    Z80->m = 1; Z80->t = 4;
}

void ADC_A_r8(uint8_t r8){
    int temp = Z80->R->A + r8;
    Z80->R->A += r8;
    if((Z80->R->F & FC) == FC){
        Z80->R->A += 1;
    }
    if(!Z80->R->A) Z80->R->F |= FZ;
    if( temp > 255) Z80->R->F |= FC;
    Z80->m = 1; Z80->t = 4;
}

void SUB_A_r8(uint8_t r8){
    if(r8 > Z80->R->A){
        Z80->R->F |= FC;
    }
    Z80->R->F |= FS;
    int temp = Z80->R->A - r8;
    if(!temp) Z80->R->F |= FZ;
    Z80->R->A = (uint8_t)(temp & 0xFF);
    Z80->m = 1;
}

void SBC_A_r8(uint8_t r8){
    uint8_t carry = ((Z80->R->F & FC) == FC)? 1:0;
    if((r8 + carry) > Z80->R->A ){
        Z80->R->F |= FC;
    }
    int temp = Z80->R->A - r8 - carry;
    if(!temp) Z80->R->F |= FZ;
    Z80->R->F |= FS;
    Z80->R->A = temp & 0xFF;
    Z80->m = 1;
}

void AND_A_r8(uint8_t r8){
    Z80->R->A &= r8;
    if(!Z80->R->A) Z80->R->F |= FZ;
    Z80->R->F |= FH;
    Z80->m = 1;
}

void XOR_A_r8(uint8_t r8){
    Z80->R->A ^= r8;
    if(!Z80->R->A) Z80->R->F |= FZ;
    Z80->m = 1;
}

void OR_A_r8(uint8_t r8){
    Z80->R->A |= r8;
    if(!Z80->R->A) Z80->R->F |= FZ;
    Z80->m = 1;
}

void CP_A_r8(uint8_t Y){
    int temp = Z80->R->A;
    temp -= Y;
    Z80->R->F |= 0x40;
    if(!(temp & 255)) Z80->R->F |= 0x80;
    if( temp < 0 ) Z80->R->F |= 0x10;
    Z80->m = 1; Z80->t = 4;
}

// Block 3

void ADD_A_imm(uint8_t Y){
    int temp = Z80->R->A + Y;
    Z80->R->A += Y;
    if(!Z80->R->A) Z80->R->F |= FZ;
    if( temp > 255) Z80->R->F |= FC;
    Z80->m = 2; Z80->t = 4;
}

void ADC_A_imm(uint8_t r8){
    int temp = Z80->R->A + r8;
    Z80->R->A += r8;
    if((Z80->R->F & FC) == FC){
        Z80->R->A += 1;
    }
    if(!Z80->R->A) Z80->R->F |= FZ;
    if( temp > 255) Z80->R->F |= FC;
    Z80->m = 2; Z80->t = 4;
}

void SUB_A_imm(uint8_t r8){
    if(r8 > Z80->R->A){
        Z80->R->F |= FC;
    }
    Z80->R->F |= FS;
    int temp = Z80->R->A - r8;
    if(!temp) Z80->R->F |= FZ;
    Z80->R->A = (uint8_t)(temp & 0xFF);
    Z80->m = 2;
}

void SBC_A_imm(uint8_t r8){
    uint8_t carry = ((Z80->R->F & FC) == FC)? 1:0;
    if((r8 + carry) > Z80->R->A ){
        Z80->R->F |= FC;
    }
    int temp = Z80->R->A - r8 - carry;
    if(!temp) Z80->R->F |= FZ;
    Z80->R->F |= FS;
    Z80->R->A = temp & 0xFF;
    Z80->m = 2;
}

void AND_A_imm(uint8_t r8){
    Z80->R->A &= r8;
    if(!Z80->R->A) Z80->R->F |= FZ;
    Z80->R->F |= FH;
    Z80->m = 2;
}

void XOR_A_imm(uint8_t r8){
    Z80->R->A ^= r8;
    if(!Z80->R->A) Z80->R->F |= FZ;
    Z80->m = 2;
}

void OR_A_imm(uint8_t r8){
    Z80->R->A |= r8;
    if(!Z80->R->A) Z80->R->F |= FZ;
    Z80->m = 2;
}

void CP_A_imm(uint8_t Y){
    int temp = Z80->R->A;
    temp -= Y;
    Z80->R->F |= 0x40;
    if(!(temp & 255)) Z80->R->F |= 0x80;
    if( temp < 0 ) Z80->R->F |= 0x10;
    Z80->m = 2;
}

void RET(){
    POP_(&(Z80->R->PC));
    Z80->m = 4;
}

void RET_C(char cond){
    if ((cond > 0) && ((Z80->R->F & cond) == cond)){
        POP_(&(Z80->R->PC));
        Z80->m = 5;
    } else if ((cond < 0) && (Z80->R->F & cond) == FN){
        POP_(&(Z80->R->PC));
        Z80->m = 5;
    }else{
        Z80->m = 3;
    }
}

void RETI()
{

}

void JP(uint16_t imm16)
{
    Z80->R->PC = imm16;
    Z80->m = 4;
}

void JP_C(char cond, uint16_t imm16){
    if(cond > 0){
        if((Z80->R->F & cond) == cond){
            JP(imm16);
        }else{
            Z80->m = 3;
        }
    }else{
        if((Z80->R->F & cond) == FN){
            JP(imm16);
        }else{
            Z80->m = 3;
        }
    }
}

void JP_HL(){
    uint16_t* temp = (uint16_t*)Z80->R->HL;
    Z80->R->PC = *temp;
    Z80->m = 1;
}

void CALL(uint16_t imm16)
{
    
}

void CALL_C(char cond, uint16_t imm16)
{

}
void RST(char target[3])
{

}

void PUSH_(uint16_t *r16){
    Z80->R->SP=-2;
    ww(Z80->R->SP, *r16);
    Z80->m = 3; Z80->t = 12;
}

void POP_(uint16_t *r16){
    *r16 = rw(Z80->R->SP);
    Z80->R->SP += 2;
    Z80->m = 3; Z80->t = 12;
}

void LDH_C_A(){
    // TODO
}

void LDH_A_C(){
    // TODO
}

void LDH_imm_A(uint8_t imm8){
    // TODO
}

void LD_addr_A(uint16_t addr){
    // TODO
}

void LDH_A_imm(uint8_t imm8){
    // TODO
}

void LD_A_addr(uint16_t addr){
    // TODO
}

void STOP(){
    // TODO
}

void decode8(uint8_t opcode, uint8_t nextByte, uint16_t nextWord){
    switch (opcode)
    {
    case 0x00:
        NOP_();
        break;
    case 0x01:
        LD_r16_imm(&(Z80->R->BC), nextWord);
        break;
    case 0x02:
        LD_addr_A(Z80->R->BC);
        break;
    case 0x03:
        INC_16(&(Z80->R->BC));
        break;
    case 0x04:
        INC_8(&(Z80->R->B));
        break;
    case 0x05:
        DEC_8(&(Z80->R->B));
        break;
    case 0x06:
        LD_r8_imm(&(Z80->R->B), nextByte);
        break;
    case 0x07:
        RLCA();
        break;
    case 0x08:
        LD_addr_A(nextWord);
        break;
    case 0x09:
        ADD_HL_r16(&(Z80->R->BC));
        break;
    case 0x0A:
        LD_A_addr(Z80->R->BC);
        break;
    case 0x0B:
        DEC_16(&(Z80->R->BC));
        break;
    case 0x0C:
        INC_8(&(Z80->R->C));
        break;
    case 0x0D:
        DEC_8(&(Z80->R->C));
        break;
    case 0x0E:
        LD_r8_imm(&(Z80->R->C), nextByte);
        break;
    case 0x0F:
        RRCA();
        break;

    case 0x10:
        STOP();
        break;

    case 0x11:
        LD_r16_imm(&(Z80->R->DE), nextWord);
        break;
    case 0x12:
        LD_addr_A(Z80->R->DE);
        break;
    case 0x13:
        INC_16(&(Z80->R->DE));
        break;
    case 0x14:
        INC_8(&(Z80->R->D));
        break;
    case 0x15:
        DEC_8(&(Z80->R->D));
        break;
    case 0x16:
        LD_r8_imm(&(Z80->R->D), nextByte);
        break;
    case 0x17:
        RLA();
        break;

    case 0x18:
        JR(nextByte);
        break;

    case 0x19:
        ADD_HL_r16(&(Z80->R->DE));
        break;
    case 0x1A:
        LD_A_addr(Z80->R->DE);
        break;
    case 0x1B:
        DEC_16(&(Z80->R->DE));
        break;
    case 0x1C:
        INC_8(&(Z80->R->E));
        break;
    case 0x1D:
        DEC_8(&(Z80->R->E));
        break;
    case 0x1E:
        LD_r8_imm(&(Z80->R->E), nextByte);
        break;
    case 0x1F:
        RRA();
        break;

    case 0x20:
        JR_C(FZ, nextByte);
        break;
    case 0x21:
        LD_r16_imm(&(Z80->R->HL), nextWord);
        break;
    case 0x22:
        LD_mem_A(Z80->R->HL);
        break;
    case 0x23:
        INC_16(&(Z80->R->HL));
        break;
    case 0x24:
        INC_8(&(Z80->R->H));
        break;
    case 0x25:
        DEC_8(&(Z80->R->H));
        break;
    case 0x26:
        LD_r8_imm(&(Z80->R->H), nextByte);
        break;
    case 0x27:
        DAA();
        break;

    case 0x28:
        JR_C(FZ, nextByte);
        break;
    case 0x29:
        ADD_HL_r16(&(Z80->R->HL));
        break;
    case 0x2A:
        LD_A_mem(Z80->R->HL);
        break;
    case 0x2B:
        DEC_16(&(Z80->R->HL));
        break;
    case 0x2C:
        INC_8(&(Z80->R->L));
        break;
    case 0x2D:
        DEC_8(&(Z80->R->L));
        break;
    case 0x2E:
        LD_r8_imm(&(Z80->R->L), nextByte);
        break;
    case 0x2F:
        CPL();
        break;

    case 0x30:
        JR_C(FC, nextByte);
        break;
    case 0x31:
        LD_r16_imm(&(Z80->R->SP), nextWord);
        break;
    case 0x32:
        LD_mem_A(Z80->R->SP);
        break;
    case 0x33:
        INC_16(&(Z80->R->SP));
        break;
    default:
        break;
    }
}