#include <stdint.h>
#include <string.h>

#include <cpu.h>
#include <memoryUnit.h>

CPU *Z80;

CPU* initCPU(){
    CPU* p = malloc(sizeof(CPU));
    p->R = initRegs();
    p->clock_m = 0;
    p->clock_t = 0;
    p->halt = false;
    return p;
}
void resetCPU(){
    resetRegs(Z80->R);
    Z80->R->PC = 0x0100;
    Z80->R->SP = 0xFFFE;
    Z80->R->A = 1;
    Z80->R->C = 0x13;
    Z80->R->HL = 0x014D;
    Z80->R->m = 0; Z80->clock_t = 0;
}

/* Instructions */

// Load Instructions
void LD_r8_imm(uint8_t* X, uint8_t imm8){
    *X = imm8;
    Z80->R->m = 2; Z80->clock_t = 8;
}
void LD_r8_r8(uint8_t* X, uint8_t* Y){
    /* LD X Y  load Y(copy) on X */
    uint8_t temp = *Y;
    *X = temp;
    Z80->R->m = 1;
}
void LD_r16_imm(uint16_t *r16, uint16_t imm16){
    uint16_t temp = imm16;
    r16[0] = temp & 0xFF;
    r16[1] = (temp >> 8) & 0xFF;
    Z80->R->m = 3; Z80->clock_t = 12;
}
void LD_mem_A(uint16_t addr){
    uint8_t val = Z80->R->A;
    wb(addr, val);
    Z80->R->m = 2; Z80->clock_t = 4;
}
void LD_A_mem(uint16_t addr){
    // addr comes from r16 registers
    uint8_t val = rb(addr);
    Z80->R->A = val;
    Z80->R->m = 2; Z80->clock_t = 4;
}
void LDH_C_A(){
    uint8_t val = Z80->R->A;
    wb(0xFF00 + Z80->R->C, val);
    Z80->R->m = 2;
}
void LDH_A_C(){
    uint8_t val = rb(0xFF00 + Z80->R->C);
    Z80->R->A = val;
    Z80->R->m = 2;
}
void LDH_imm_A(uint8_t imm8){}
void LD_addr_A(uint16_t addr){
    // addr is between 0xFF00 and 0xFFFF
    uint8_t val = Z80->R->A;
    wb(addr, val);
    Z80->R->m = 3;
}
void LDH_A_imm(uint8_t imm8){}
void LD_A_addr(uint16_t addr){
    // addr is between 0xFF00 and 0xFFFF
    uint8_t val = rb(addr);
    Z80->R->A = val;
    Z80->R->m = 3;
}

// Bit Shift Instructions
void RLCA(){
    uint8_t temp = Z80->R->A >> 7;
    Z80->R->A = (Z80->R->A << 1) | temp;
    if(! temp) Z80->R->F |= FC;
    Z80->R->m = 1; Z80->clock_t = 4;
}
void RRCA(){
    uint8_t temp = Z80->R->A << 7;
    Z80->R->A = (Z80->R->A >> 1) | temp;
    if(! (temp >> 7)) Z80->R->F |= FC;
    Z80->R->m = 1; Z80->clock_t = 4;
}
void RLA(){
    if((Z80->R->F & FC) == FC){
        Z80->R->A = (Z80->R->A << 1) | 0x01 ;
    }else{
        Z80->R->A = (Z80->R->A << 1);
    }
    Z80->R->m = 1; Z80->clock_t = 4;
}
void RRA(){
    if((Z80->R->F & FC) == FC){
        Z80->R->A = (Z80->R->A >> 1) | 0x80 ;
    }else{
        Z80->R->A = (Z80->R->A >> 1);
    }
    Z80->R->m = 1; Z80->clock_t = 4;
}

// Arithmetic Instructions

void ADD_A_r8(uint8_t Y)
{
    int temp = Z80->R->A + Y;
    Z80->R->A += Y;
    if(!Z80->R->A) Z80->R->F |= FZ;
    if( temp > 255) Z80->R->F |= FC;
    Z80->R->m = 1; Z80->clock_t = 4;
}
void ADD_A_imm(uint8_t Y){
    int temp = Z80->R->A + Y;
    Z80->R->A += Y;
    if(!Z80->R->A) Z80->R->F |= FZ;
    if( temp > 255) Z80->R->F |= FC;
    Z80->R->m = 2; Z80->clock_t = 4;
}
void ADC_A_r8(uint8_t r8){
    int temp = Z80->R->A + r8;
    Z80->R->A += r8;
    if((Z80->R->F & FC) == FC){
        Z80->R->A += 1;
    }
    if(!Z80->R->A) Z80->R->F |= FZ;
    if( temp > 255) Z80->R->F |= FC;
    Z80->R->m = 1; Z80->clock_t = 4;
}
void ADC_A_imm(uint8_t r8){
    int temp = Z80->R->A + r8;
    Z80->R->A += r8;
    if((Z80->R->F & FC) == FC){
        Z80->R->A += 1;
    }
    if(!Z80->R->A) Z80->R->F |= FZ;
    if( temp > 255) Z80->R->F |= FC;
    Z80->R->m = 2; Z80->clock_t = 4;
}
void SUB_A_r8(uint8_t r8){
    if(r8 > Z80->R->A){
        Z80->R->F |= FC;
    }
    Z80->R->F |= FS;
    int temp = Z80->R->A - r8;
    if(!temp) Z80->R->F |= FZ;
    Z80->R->A = (uint8_t)(temp & 0xFF);
    Z80->R->m = 1;
}
void SUB_A_imm(uint8_t r8){
    if(r8 > Z80->R->A){
        Z80->R->F |= FC;
    }
    Z80->R->F |= FS;
    int temp = Z80->R->A - r8;
    if(!temp) Z80->R->F |= FZ;
    Z80->R->A = (uint8_t)(temp & 0xFF);
    Z80->R->m = 2;
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
    Z80->R->m = 1;
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
    Z80->R->m = 2;
}
void AND_A_r8(uint8_t r8){
    Z80->R->A &= r8;
    if(!Z80->R->A) Z80->R->F |= FZ;
    Z80->R->F |= FH;
    Z80->R->m = 1;
}
void AND_A_imm(uint8_t r8){
    Z80->R->A &= r8;
    if(!Z80->R->A) Z80->R->F |= FZ;
    Z80->R->F |= FH;
    Z80->R->m = 2;
}
void XOR_A_r8(uint8_t r8){
    Z80->R->A ^= r8;
    if(!Z80->R->A) Z80->R->F |= FZ;
    Z80->R->m = 1;
}
void XOR_A_imm(uint8_t r8){
    Z80->R->A ^= r8;
    if(!Z80->R->A) Z80->R->F |= FZ;
    Z80->R->m = 2;
}
void OR_A_r8(uint8_t r8){
    Z80->R->A |= r8;
    if(!Z80->R->A) Z80->R->F |= FZ;
    Z80->R->m = 1;
}
void OR_A_imm(uint8_t r8){
    Z80->R->A |= r8;
    if(!Z80->R->A) Z80->R->F |= FZ;
    Z80->R->m = 2;
}
void CP_A_r8(uint8_t Y){
    int temp = Z80->R->A;
    temp -= Y;
    Z80->R->F |= 0x40;
    if(!(temp & 255)) Z80->R->F |= 0x80;
    if( temp < 0 ) Z80->R->F |= 0x10;
    Z80->R->m = 1; Z80->clock_t = 4;
}
void CP_A_imm(uint8_t Y){
    int temp = Z80->R->A;
    temp -= Y;
    Z80->R->F |= 0x40;
    if(!(temp & 255)) Z80->R->F |= 0x80;
    if( temp < 0 ) Z80->R->F |= 0x10;
    Z80->R->m = 2;
}
void INC_8(uint8_t* r8){
    *r8 += 1;
    char temp = *r8;
    if(!temp) Z80->R->F |= FZ;
    Z80->R->F &= ~FC;
    Z80->R->F &= ~FS;
    Z80->R->m = 1; Z80->clock_t = 4;
}
void INC_16(uint16_t *r16){
    uint16_t* pt = (uint16_t*)r16;
    *pt += 1;
    Z80->R->m = 2; Z80->clock_t = 4;
}
void DEC_8(uint8_t *r8){
    *r8 -= 1;
    char temp = *r8;
    Z80->R->F |= FS;
    if(!temp) Z80->R->F |= FZ;
    Z80->R->F &= ~FC;
    Z80->R->m = 1; Z80->clock_t = 4;
}
void DEC_16(uint16_t *r16){
    uint16_t* pt = (uint16_t*)r16;
    *pt -= 1;
    Z80->R->m = 2; Z80->clock_t = 4;
}
void ADD_HL_r16(uint16_t* r16){
    int temp = *r16 + Z80->R->HL;
    if(!Z80->R->A) Z80->R->F |= FZ;
    if( temp > 0xFFFF) Z80->R->F |= FC;
    else if(temp > 0xFFF) Z80->R->F |= FH;
    Z80->R->HL = temp & 0xFFFF;
    Z80->R->m = 2; Z80->clock_t = 4;
}

// Jumps & Subroutine instructions

void JP(uint16_t imm16){
    Z80->R->PC = imm16;
    Z80->R->m = 4;
}
void JP_C(enum cond c, uint16_t imm16){
    switch(c){
        case NZ:
            if((Z80->R->F & FZ) == FN) JP(imm16);
            else Z80->R->m = 3;
            break;
        case Z:
            if((Z80->R->F & FZ) == FZ) JP(imm16);
            else Z80->R->m = 3;
            break;
        case NC:
            if((Z80->R->F & FC) == FN) JP(imm16);
            else Z80->R->m = 3;
            break;
        case C:
            if((Z80->R->F & FC) == FC) JP(imm16);
            else Z80->R->m = 3;
            break;
        default:
            break;
    }
}
void JP_HL(){
    uint16_t* temp = (uint16_t*)Z80->R->HL;
    Z80->R->PC = *temp;
    Z80->R->m = 1;
}
void JR(uint16_t addr){
    uint16_t offset = Z80->R->PC - addr;
    if ( (offset < 128) | (offset > -127)){
        Z80->R->PC += offset;
    }
    Z80->R->m = 3; Z80->clock_t = 8;
}
void JR_C(enum cond c, uint16_t addr)
{
    switch(c){
        case NZ:
            if ((Z80->R->F & FZ == FN)) JR(addr);
            else Z80->R->m = 2;
            break;
        case Z:
            if ((Z80->R->F & FZ == FZ)) JR(addr);
            else Z80->R->m = 2;
            break;
        case NC:
            if ((Z80->R->F & FC == FN)) JR(addr);
            else Z80->R->m = 2;
            break;
        case C:
            if ((Z80->R->F & FC == FC)) JR(addr);
            else Z80->R->m = 2;
            break;
        default:
            break;
    }
}
void CALL(uint16_t imm16){
    Z80->R->SP -= 2;
    ww(Z80->R->SP, imm16);
    Z80->R->PC = imm16;
    Z80->R->m = 5;
}
void CALL_C(enum cond c, uint16_t imm16){
    switch (c){
        case Z:
            if(Z80->R->F & FZ == FN){
                CALL(imm16);
                Z80->R->m = 6;
            }else{ Z80->R->m = 3;}
            break;
        case NZ:
            if(Z80->R->F & FZ == FZ){
                CALL(imm16);
            }else{Z80->R->m = 3;}
            break;
        case C:
            if(Z80->R->F & FC == FN){
                CALL(imm16);
            }else{Z80->R->m = 3;}
            break;
        case NC:
            if(Z80->R->F & FC == FC){
                CALL(imm16);
            }else{Z80->R->m = 3;}
            break;
        default:
            break;
    }
}
void RST(char target[3]){}
void RET(){
    POP_(&(Z80->R->PC));
    Z80->R->m = 4;
}
void RET_C(enum cond c){
    switch(c){
        case NZ:
            if(Z80->R->F & FZ == FN) RET();
            else Z80->R->m = 2;
            break;
        case Z:
            if(Z80->R->F & FZ == FZ) RET();
            else Z80->R->m = 2;
            break;
        case NC:
            if(Z80->R->F & FC == FN) RET();
            else Z80->R->m = 2;
            break;
        case C:
            if(Z80->R->F & FC == FC) RET();
            else Z80->R->m = 2;
            break;
        default:
            break;
    }
}
void RETI(){
    EI(); RET(); Z80->R->m = 4;
}

// Stack instructions

void LD_mem_sp(uint16_t addr){
    uint8_t temp[2];
    temp[0] = Z80->R->SP & 0xFF;
    temp[1] = Z80->R->SP >> 8;
    ww(addr, temp);
    Z80->R->m = 5; Z80->clock_t = 12;
}
void LD_sp_imm(uint16_t imm16){
    Z80->R->SP = imm16;
    Z80->R->m = 3; Z80->clock_t = 12;
}
void PUSH_(uint16_t *r16){
    Z80->R->SP=-2;
    ww(Z80->R->SP, *r16);
    Z80->R->m = 3; Z80->clock_t = 12;
}
void POP_(uint16_t *r16){
    *r16 = rw(Z80->R->SP);
    Z80->R->SP += 2;
    Z80->R->m = 3; Z80->clock_t = 12;
}
void ADD_sp_imm(int8_t imm8){
    Z80->R->SP += imm8;
    Z80->R->m = 4;
}
void LD_HL_sp_imm(uint8_t imm8){
    uint16_t temp = Z80->R->SP + imm8;
    Z80->R->HL = temp;
    Z80->R->m = 3;
}
void LD_sp_HL(){
    Z80->R->SP = Z80->R->HL;
    Z80->R->m = 2;
}

// Miscalaneous Instructions

void NOP_(){
    Z80->R->m = 1; Z80->clock_t = 4;
}
void DAA(){
    uint8_t a = Z80->R->A;
    if(Z80->R->F & FH || a&0x0F > 9){
        Z80->R->A += 0x06;
        Z80->R->F &= 0xEF;
    }else if( Z80->R->F & FH || a > 0x99){
        Z80->R->A += 0x60;
        Z80->R->F |= FC;
    }
    Z80->R->m = 1;
}
void SCF(){
    Z80->R->F |= FC;
    Z80->R->F &= ~FS;
    Z80->R->F &= ~FH;
    Z80->R->m = 1; Z80->clock_t = 4;
}
void CCF(){
    Z80->R->F = ~(Z80->R->F & FC);
    Z80->R->F &= ~FS;
    Z80->R->F &= ~FH;
    Z80->R->m = 1; Z80->clock_t = 4;
}
void CPL(){
    Z80->R->A = ~Z80->R->A;
    Z80->R->F |= FH;
    Z80->R->F |= FS;
    Z80->R->m = 1; Z80->clock_t = 4;
}
void HALT(){}
void DI(){}
void EI(){}
void STOP(){}

// CB block instructions
void RLC(uint8_t* r8){
    uint8_t temp = *r8;
    *r8 = (temp<<1) | (temp>>7);
    if(*r8 == 0) Z80->R->F |= FZ;
    if(temp & 0x80) Z80->R->F |= FC;
    else Z80->R->F &= ~FC;
    Z80->R->m = 2;
}
void RRC(uint8_t* r8){
    uint8_t temp = *r8;
    *r8 = (temp>>1) | (temp<<7);
    if(*r8 == 0) Z80->R->F |= FZ;
    if(temp & 0x01) Z80->R->F |= FC;
    Z80->R->m = 2;
}
void RL(uint8_t* r8){
    uint8_t temp = *r8;
    *r8 = (temp<<1) | ((Z80->R->F & FC == FC)?1:0);
    if(*r8 == 0) Z80->R->F |= FZ;
    if(temp & 0x80) Z80->R->F |= FC;
    else Z80->R->F &= ~FC;
    Z80->R->m = 2;
}
void RR(uint8_t* r8){
    uint8_t temp = *r8;
    *r8 = (temp>>1) | ((Z80->R->F & FC == FC)?0x80:0);
    if(*r8 == 0) Z80->R->F |= FZ;
    if(temp & 0x01) Z80->R->F |= FC;
    else Z80->R->F &= ~FC;
    Z80->R->m = 2;
}
void SLA(uint8_t* r8){
    uint8_t temp = *r8;
    *r8 = (temp<<1);
    if(*r8 == 0) Z80->R->F |= FZ;
    if(temp & 0x80) Z80->R->F |= FC;
    else Z80->R->F &= ~FC;
    Z80->R->m = 2;
}
void SRA(uint8_t* r8){
    uint8_t temp = *r8;
    *r8 = (temp>>1) | ((temp & 0x80) << 7);
    if(*r8 == 0) Z80->R->F |= FZ;
    if(temp & 0x01) Z80->R->F |= FC;
    else Z80->R->F &= ~FC;
    Z80->R->m = 2;
}
void SWAP(uint8_t* r8){
    uint8_t temp = *r8;
    *r8 = (temp<<4) | (temp>>4);
    if(*r8 == 0) Z80->R->F |= FZ;
    Z80->R->m = 2;
}
void SRL(uint8_t* r8){
    uint8_t temp = *r8;
    *r8 = (temp>>1);
    if(*r8 == 0) Z80->R->F |= FZ;
    if(temp & 0x01) Z80->R->F |= FC;
    else Z80->R->F &= ~FC;
    Z80->R->m = 2;
}

void decode8(uint8_t opcode, uint8_t nextByte, uint16_t nextWord){
    switch (opcode)
    {
    case 0x00: NOP_(); break;
    case 0x01: LD_r16_imm(&(Z80->R->BC), nextWord); break;
    case 0x02: LD_addr_A(Z80->R->BC); break;
    case 0x03: INC_16(&(Z80->R->BC)); break;
    case 0x04: INC_8(&(Z80->R->B)); break;
    case 0x05: DEC_8(&(Z80->R->B)); break;
    case 0x06: LD_r8_imm(&(Z80->R->B), nextByte); break;
    case 0x07: RLCA(); break;
    case 0x08: LD_addr_A(nextWord); break;
    case 0x09: ADD_HL_r16(&(Z80->R->BC)); break;
    case 0x0A: LD_A_addr(Z80->R->BC); break;
    case 0x0B: DEC_16(&(Z80->R->BC)); break;
    case 0x0C: INC_8(&(Z80->R->C)); break;
    case 0x0D: DEC_8(&(Z80->R->C)); break;
    case 0x0E: LD_r8_imm(&(Z80->R->C), nextByte); break;
    case 0x0F: RRCA(); break;

    case 0x10: STOP(); break;
    case 0x11: LD_r16_imm(&(Z80->R->DE), nextWord); break;
    case 0x12: LD_addr_A(Z80->R->DE); break;
    case 0x13: INC_16(&(Z80->R->DE)); break;
    case 0x14: INC_8(&(Z80->R->D)); break;
    case 0x15: DEC_8(&(Z80->R->D)); break;
    case 0x16: LD_r8_imm(&(Z80->R->D), nextByte); break;
    case 0x17: RLA(); break;
    case 0x18: JR(nextByte); break;
    case 0x19: ADD_HL_r16(&(Z80->R->DE)); break;
    case 0x1A: LD_A_addr(Z80->R->DE); break;
    case 0x1B: DEC_16(&(Z80->R->DE)); break;
    case 0x1C: INC_8(&(Z80->R->E)); break;
    case 0x1D: DEC_8(&(Z80->R->E)); break;
    case 0x1E: LD_r8_imm(&(Z80->R->E), nextByte); break;
    case 0x1F: RRA(); break;

    case 0x20: JR_C(NZ, nextByte); break;
    case 0x21: LD_r16_imm(&(Z80->R->HL), nextWord); break;
    case 0x22: LD_mem_A(Z80->R->HL); break;
    case 0x23: INC_16(&(Z80->R->HL)); break;
    case 0x24: INC_8(&(Z80->R->H)); break;
    case 0x25: DEC_8(&(Z80->R->H)); break;
    case 0x26: LD_r8_imm(&(Z80->R->H), nextByte); break;
    case 0x27: DAA(); break;
    case 0x28: JR_C(Z, nextByte); break;
    case 0x29: ADD_HL_r16(&(Z80->R->HL)); break;
    case 0x2A: LD_A_mem(Z80->R->HL); break;
    case 0x2B: DEC_16(&(Z80->R->HL)); break;
    case 0x2C: INC_8(&(Z80->R->L)); break;
    case 0x2D: DEC_8(&(Z80->R->L)); break;
    case 0x2E: LD_r8_imm(&(Z80->R->L), nextByte); break;
    case 0x2F: CPL(); break;

    case 0x30: JR_C(NC, nextByte); break;
    case 0x31: LD_r16_imm(&(Z80->R->SP), nextWord); break;
    case 0x32: LD_mem_A(Z80->R->SP); break;
    case 0x33: INC_16(&(Z80->R->SP)); break;
    case 0x34: INC_16(&(Z80->R->HL)); break;
    case 0x35: DEC_16(&(Z80->R->HL)); break;
    case 0x36: break; // LD_mem_HL(nextByte);
    case 0x37: SCF(); break;
    case 0x38: JR_C(C, nextByte); break;
    case 0x39: ADD_HL_r16(&(Z80->R->SP)); break;
    case 0x3A: LD_A_mem(Z80->R->SP); // TODO break;
    case 0x3B: DEC_16(&(Z80->R->SP)); break;
    case 0x3C: INC_8(&(Z80->R->A)); break;
    case 0x3D: DEC_8(&(Z80->R->A)); break;
    case 0x3E: LD_r8_imm(&(Z80->R->A), nextByte); break;
    case 0x3F: CCF(); break;

    case 0x40: LD_r8_r8(&(Z80->R->B), &(Z80->R->B)); break;
    case 0x41: LD_r8_r8(&(Z80->R->B), &(Z80->R->C)); break;
    case 0x42: LD_r8_r8(&(Z80->R->B), &(Z80->R->D)); break;
    case 0x43: LD_r8_r8(&(Z80->R->B), &(Z80->R->E)); break;
    case 0x44: LD_r8_r8(&(Z80->R->B), &(Z80->R->H)); break;
    case 0x45: LD_r8_r8(&(Z80->R->B), &(Z80->R->L)); break;
    case 0x46: // LD_r8_r8(&(Z80->R->B), &(MMU->rb(Z80->R->HL))); TODO break;
    case 0x47: LD_r8_r8(&(Z80->R->B), &(Z80->R->A)); break;
    case 0x48: LD_r8_r8(&(Z80->R->C), &(Z80->R->B)); break;
    case 0x49: LD_r8_r8(&(Z80->R->C), &(Z80->R->C)); break;
    case 0x4A: LD_r8_r8(&(Z80->R->C), &(Z80->R->D)); break;
    case 0x4B: LD_r8_r8(&(Z80->R->C), &(Z80->R->E)); break;
    case 0x4C: LD_r8_r8(&(Z80->R->C), &(Z80->R->H)); break;
    case 0x4D: LD_r8_r8(&(Z80->R->C), &(Z80->R->L)); break;
    case 0x4E: // LD_r8_r8(&(Z80->R->C), &(MMU->rb(Z80->R->HL))); TODO break;
    case 0x4F: LD_r8_r8(&(Z80->R->C), &(Z80->R->A)); break;

    case 0x50: LD_r8_r8(&(Z80->R->D), &(Z80->R->B)); break;
    case 0x51: LD_r8_r8(&(Z80->R->D), &(Z80->R->C)); break;
    case 0x52: LD_r8_r8(&(Z80->R->D), &(Z80->R->D)); break;
    case 0x53: LD_r8_r8(&(Z80->R->D), &(Z80->R->E)); break;
    case 0x54: LD_r8_r8(&(Z80->R->D), &(Z80->R->H)); break;
    case 0x55: LD_r8_r8(&(Z80->R->D), &(Z80->R->L)); break;
    case 0x56: // LD_r8_r8(&(Z80->R->D), &(MMU->rb(Z80->R->HL))); TODO break;
    case 0x57: LD_r8_r8(&(Z80->R->D), &(Z80->R->A)); break;
    case 0x58: LD_r8_r8(&(Z80->R->E), &(Z80->R->B)); break;
    case 0x59: LD_r8_r8(&(Z80->R->E), &(Z80->R->C)); break;
    case 0x5A: LD_r8_r8(&(Z80->R->E), &(Z80->R->D)); break;
    case 0x5B: LD_r8_r8(&(Z80->R->E), &(Z80->R->E)); break;
    case 0x5C: LD_r8_r8(&(Z80->R->E), &(Z80->R->H)); break;
    case 0x5D: LD_r8_r8(&(Z80->R->E), &(Z80->R->L)); break;
    case 0x5E: // LD_r8_r8(&(Z80->R->E), &(MMU->rb(Z80->R->HL))); TODO break;
    case 0x5F: LD_r8_r8(&(Z80->R->E), &(Z80->R->A)); break;

    case 0x60: LD_r8_r8(&(Z80->R->H), &(Z80->R->B)); break;
    case 0x61: LD_r8_r8(&(Z80->R->H), &(Z80->R->C)); break;
    case 0x62: LD_r8_r8(&(Z80->R->H), &(Z80->R->D)); break;
    case 0x63: LD_r8_r8(&(Z80->R->H), &(Z80->R->E)); break;
    case 0x64: LD_r8_r8(&(Z80->R->H), &(Z80->R->H)); break;
    case 0x65: LD_r8_r8(&(Z80->R->H), &(Z80->R->L)); break;
    case 0x66: // LD_r8_r8(&(Z80->R->H), &(MMU->rb(Z80->R->HL))); TODO break;
    case 0x67: LD_r8_r8(&(Z80->R->H), &(Z80->R->A)); break;
    case 0x68: LD_r8_r8(&(Z80->R->L), &(Z80->R->B)); break;
    case 0x69: LD_r8_r8(&(Z80->R->L), &(Z80->R->C)); break;
    case 0x6A: LD_r8_r8(&(Z80->R->L), &(Z80->R->D)); break;
    case 0x6B: LD_r8_r8(&(Z80->R->L), &(Z80->R->E)); break;
    case 0x6C: LD_r8_r8(&(Z80->R->L), &(Z80->R->H)); break;
    case 0x6D: LD_r8_r8(&(Z80->R->L), &(Z80->R->L)); break;
    case 0x6E: // LD_r8_r8(&(Z80->R->L), &(MMU->rb(Z80->R->HL))); TODO break;
    case 0x6F: LD_r8_r8(&(Z80->R->L), &(Z80->R->A)); break;

    case 0x70: break; // TODO
    case 0x71: break; // TODO
    case 0x72: break; // TODO
    case 0x73: break; // TODO
    case 0x74: break; // TODO
    case 0x75: break; // TODO
    case 0x76: HALT(); break;
    case 0x77: break;
    case 0x78: LD_r8_r8(&(Z80->R->A), &(Z80->R->B)); break;
    case 0x79: LD_r8_r8(&(Z80->R->A), &(Z80->R->C)); break;
    case 0x7A: LD_r8_r8(&(Z80->R->A), &(Z80->R->D)); break;
    case 0x7B: LD_r8_r8(&(Z80->R->A), &(Z80->R->E)); break;
    case 0x7C: LD_r8_r8(&(Z80->R->A), &(Z80->R->H)); break;
    case 0x7D: LD_r8_r8(&(Z80->R->A), &(Z80->R->L)); break;
    case 0x7E: // LD_r8_r8(&(Z80->R->A), &(MMU->rb(Z80->R->HL))); TODO break;
    case 0x7F: LD_r8_r8(&(Z80->R->A), &(Z80->R->A)); break;

    case 0x80: ADD_A_r8(Z80->R->B); break;
    case 0x81: ADD_A_r8(Z80->R->C); break;
    case 0x82: ADD_A_r8(Z80->R->D); break;
    case 0x83: ADD_A_r8(Z80->R->E); break;
    case 0x84: ADD_A_r8(Z80->R->H); break;
    case 0x85: ADD_A_r8(Z80->R->L); break;
    case 0x86: // ADD_A_r8(MMU->rb(Z0->R->HL))); TODO break;
    case 0x87: ADD_A_r8(Z80->R->A); break;
    case 0x88: ADC_A_r8(Z80->R->B); break;
    case 0x89: ADC_A_r8(Z80->R->C); break;
    case 0x8A: ADC_A_r8(Z80->R->D); break;
    case 0x8B: ADC_A_r8(Z80->R->E); break;
    case 0x8C: ADC_A_r8(Z80->R->H); break;
    case 0x8D: ADC_A_r8(Z80->R->L); break;
    case 0x8E: // ADC_A_r8(MMU->rb(Z0->R->HL))); TODO break;
    case 0x8F: ADC_A_r8(Z80->R->A); break;

    case 0x90: SUB_A_r8(Z80->R->B); break;
    case 0x91: SUB_A_r8(Z80->R->C); break;
    case 0x92: SUB_A_r8(Z80->R->D); break;
    case 0x93: SUB_A_r8(Z80->R->E); break;
    case 0x94: SUB_A_r8(Z80->R->H); break;
    case 0x95: SUB_A_r8(Z80->R->L); break;
    case 0x96: // SUB_A_r8(MMU->rb(Z0->R->HL))); TODO break;
    case 0x97: SUB_A_r8(Z80->R->A); break;
    case 0x98: SBC_A_r8(Z80->R->B); break;
    case 0x99: SBC_A_r8(Z80->R->C); break;
    case 0x9A: SBC_A_r8(Z80->R->D); break;
    case 0x9B: SBC_A_r8(Z80->R->E); break;
    case 0x9C: SBC_A_r8(Z80->R->H); break;
    case 0x9D: SBC_A_r8(Z80->R->L); break;
    case 0x9E: // SBC_A_r8(MMU->rb(Z0->R->HL))); TODO break;
    case 0x9F: SBC_A_r8(Z80->R->A); break;

    case 0xA0: AND_A_r8(Z80->R->B); break;
    case 0xA1: AND_A_r8(Z80->R->C); break;
    case 0xA2: AND_A_r8(Z80->R->D); break;
    case 0xA3: AND_A_r8(Z80->R->E); break;
    case 0xA4: AND_A_r8(Z80->R->H); break;
    case 0xA5: AND_A_r8(Z80->R->L); break;
    case 0xA6: // AND_A_r8(&(MMU->rb(Z80->R->HL))); TODO break;
    case 0xA7: AND_A_r8(&(Z80->R->A)); break;
    case 0xA8: XOR_A_r8(&(Z80->R->B)); break;
    case 0xA9: XOR_A_r8(&(Z80->R->C)); break;
    case 0xAA: XOR_A_r8(&(Z80->R->D)); break;
    case 0xAB: XOR_A_r8(&(Z80->R->E)); break;
    case 0xAC: XOR_A_r8(&(Z80->R->H)); break;
    case 0xAD: XOR_A_r8(&(Z80->R->L)); break;
    case 0xAE: // XOR_A_r8(&(MMU->rb(Z80->R->HL))); TODO break;
    case 0xAF: XOR_A_r8(&(Z80->R->A)); break;

    case 0xB0: OR_A_r8(Z80->R->B); break;
    case 0xB1: OR_A_r8(Z80->R->C); break;
    case 0xB2: OR_A_r8(Z80->R->D); break;
    case 0xB3: OR_A_r8(Z80->R->E); break;
    case 0xB4: OR_A_r8(Z80->R->H); break;
    case 0xB5: OR_A_r8(Z80->R->L); break;
    case 0xB6: // OR_A_r8(&(MMU->rb(Z80->R->HL))); TODO break;
    case 0xB7: OR_A_r8(Z80->R->A); break;
    case 0xB8: CP_A_r8(Z80->R->B); break;
    case 0xB9: CP_A_r8(Z80->R->C); break;
    case 0xBA: CP_A_r8(Z80->R->D); break;
    case 0xBB: CP_A_r8(Z80->R->E); break;
    case 0xBC: CP_A_r8(Z80->R->H); break;
    case 0xBD: CP_A_r8(Z80->R->L); break;
    case 0xBE: // CP_A_r8(MMU->rb(Z0->R->HL))); TODO break;
    case 0xBF: CP_A_r8(Z80->R->A); break;

    case 0xC0: RET_C(NZ); break;
    case 0xC1: POP_(&(Z80->R->BC)); break;
    case 0xC2: JP_C(NZ, nextWord); break;
    case 0xC3: JP(nextWord); break;
    case 0xC4: CALL_C(NZ, nextWord); break;
    case 0xC5: PUSH_(&(Z80->R->BC)); break;
    case 0xC6: ADD_A_imm(nextByte); break;
    case 0xC7: RST(0x00); break;
    case 0xC8: RET_C(Z); break;
    case 0xC9: RET(); break;
    case 0xCA: JP_C(Z, nextWord); break;
    case 0xCC: CALL_C(Z, nextWord); break;
    case 0xCD: CALL(nextWord); break;
    case 0xCE: ADC_A_imm(nextByte); break;
    case 0xCF: RST(0x08); break;

    case 0xD0: RET_C(NC); break;
    case 0xD1: POP_(&(Z80->R->DE)); break;
    case 0xD2: JP_C(NC, nextWord); break;
    case 0xD3: NOP_(); break;
    case 0xD4: CALL_C(NC, nextWord); break;
    case 0xD5: PUSH_(&(Z80->R->DE)); break;
    case 0xD6: SUB_A_imm(nextByte); break;
    case 0xD7: RST(0x10); break;
    case 0xD8: RET_C(C); break;
    case 0xD9: RETI(); break;
    case 0xDA: JP_C(C, nextWord); break;
    case 0xDB: NOP_(); break;
    case 0xDC: CALL_C(C, nextWord); break;
    case 0xDD: NOP_(); break;
    case 0xDE: SBC_A_imm(nextByte); break;
    case 0xDF: RST(0x18); break;

    case 0xE0: LDH_C_A();// TODO break;
    case 0xE1: POP_(&(Z80->R->HL)); break;
    case 0xE2: LDH_C_A();// TODO break;
    case 0xE3:
    case 0xE4: NOP_(); break;
    case 0xE5: PUSH_(&(Z80->R->HL)); break;
    case 0xE6: AND_A_imm(nextByte); break;
    case 0xE7: RST(0x20); break;
    case 0xE8: ADD_sp_imm(nextByte); break;
    case 0xE9: JP_HL(); break;
    case 0xEA: LD_addr_A(nextWord); break;
    case 0xEB:
    case 0xEC:
    case 0xED: NOP_(); break;
    case 0xEE: XOR_A_imm(nextByte); break;
    case 0xEF: RST(0x28); break;

    case 0xF0: LDH_A_C();// TODO break;
    case 0xF1: POP_(&(Z80->R->AF)); break;
    case 0xF2: LDH_A_C();// TODO break;
    case 0xF3: DI(); break;
    case 0xF4: NOP_(); break;
    case 0xF5: PUSH_(&(Z80->R->AF)); break;
    case 0xF6: OR_A_imm(nextByte); break;
    case 0xF7: RST(0x30); break;
    case 0xF8: LD_HL_sp_imm(nextByte); break;
    case 0xF9: LD_sp_HL(); break;
    case 0xFA: LD_A_addr(nextWord); break;
    case 0xFB: EI(); break;
    case 0xFC:
    case 0xFD: NOP_(); break;
    case 0xFE: CP_A_imm(nextByte); break;
    case 0xFF: RST(0x38); break;

    case 0xCB: decodeCB(opcode, nextByte, nextWord); break;
    
    default:
        break;
    }
}

void decodeCB(uint8_t opcode, uint8_t nextByte, uint16_t nextWord){
    return;
}