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
void NPO_();

void LD_mm(short* r16, short imm16);
void LD_mm_A(short addr);
void LD_A_mm();
void LD_imm_sp(short imm16);

void INC_16(short* r16);
void DEC_16(short* r16);
void ADD_r16(char* X, char* Y, short* r16);

void INC(char* r8);
void DEC(char* r8);

void LD_mm8(char* X, char imm8);

void RLCA();
void RRCA();
void RLA();
void RRA();
void DAA();
void CPL();
void SCF();
void CCF();

void JR(char imm8);
void JR_C(char* cond, char imm8);

void STOP();

// Block 1: 8-bit r to r loads
void LD_(char* X, char* Y);
void HALT();

// Block 2: 8-bit arithmetic 
void ADD_A(char Y);
void ADC_A(char Y);
void SUB_A(char Y);
void SBC_A(char Y);
void AND_A(char Y);
void XOR_A(char Y);
void OR_A(char Y);
void CP_A(char Y);

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
