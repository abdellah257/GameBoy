#ifndef CPU_H
#define CPU_H

#include <registers.h>
#include <stdint.h>

/* Z80 CPU Structure */
typedef struct CPU {
    /* data */
    struct Registers* R;

    unsigned short pc; short sp;
    unsigned char m; unsigned char t;

} CPU;

CPU *Z80;
/* Initialize the CPU */
CPU* initCPU();

void resetCPU();

/* Instructions */

// Block 0

void NOP_();

void LD_imm(unsigned char* r16, unsigned short imm16);
void LD_mm_A(unsigned short addr);
void LD_A_mm(unsigned short addr);
void LD_mm_sp(unsigned short imm16);
void LD_sp_imm(unsigned short imm16);

void INC(char* r8);
void DEC(char* r8);

void INC_16(unsigned char* r16);
void DEC_16(unsigned char* r16);
void ADD_hl_r16(unsigned short* r16);

void LD_r8_mm8(unsigned char* X, unsigned char imm8);

void RLCA();
void RRCA();
void RLA();
void RRA();
void DAA();
void CPL();
void SCF();
void CCF();

void JR(uint16_t addr);
void JR_C(char cond, uint16_t addr);

void STOP();

// Block 1: 8-bit r to r loads
void LD_(char* X, char* Y);
void HALT();

// Block 2: 8-bit arithmetic 
void ADD_A(unsigned char Y);
void ADC_A(unsigned char Y);
void SUB_A(unsigned char Y);
void SBC_A(unsigned char Y);
void AND_A(unsigned char Y);
void XOR_A(unsigned char Y);
void OR_A(unsigned char Y);
void CP_A(unsigned char Y);

// Block 3
void RET();
void RETI();
void RET_C(char* cond);
void JP(short imm16);
void JP_C(char* cond, short imm16);
void JP_HL();
void CALL(short imm16);
void CALL_C(char* cond, short imm16);
void RST(char target[3]);

void PUSH_(char* X, char* Y);
void POP_(char* X, char* Y);



#endif
