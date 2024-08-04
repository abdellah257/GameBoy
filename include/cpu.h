#ifndef CPUU_H
#define CPUU_H

#include <registers.h>
#include <stdint.h>

/* Z80 CPU Structure */
typedef struct CPU {
    /* data */
    struct Registers* R;                    // Registers

    uint8_t m;                        // CPU M-Cycles
    uint8_t t;                        // CPU T-Cycles

} CPU;

extern CPU *Z80;

enum cond{
    NZ,
    Z,
    NC,
    C
};

/* Initialize the CPU */
CPU* initCPU();

void resetCPU();

/* Instructions */

// Block 0

void NOP_();

/* Load 16-bit immediate value on 16-bit register */
void LD_r16_imm(uint16_t* r16, uint16_t imm16);
/* Load value from A into memory[addr] */
void LD_mem_A(uint16_t addr);
void LD_A_mem(uint16_t addr);
void LD_mem_sp(uint16_t imm16);
void LD_sp_imm(uint16_t imm16);

void INC_8(uint8_t* r8);
void DEC_8(uint8_t* r8);

void INC_16(uint16_t* r16);
void DEC_16(uint16_t* r16);
void ADD_HL_r16(uint16_t* r16);

void LD_r8_imm(uint8_t* X, uint8_t imm8);

void RLCA();
void RRCA();
void RLA();
void RRA();
void DAA();
void CPL();
void SCF();
void CCF();

void JR(uint16_t addr);
void JR_C(enum cond c, uint16_t addr);

void STOP();

// Block 1: 8-bit r to r loads
void LD_r8_r8(uint8_t* X, uint8_t* Y);
void HALT();

// Block 2: 8-bit arithmetic 
void ADD_A_r8(uint8_t r8);
void ADC_A_r8(uint8_t r8);
void SUB_A_r8(uint8_t r8);
void SBC_A_r8(uint8_t r8);
void AND_A_r8(uint8_t r8);
void XOR_A_r8(uint8_t r8);
void OR_A_r8(uint8_t r8);
void CP_A_r8(uint8_t r8);

// Block 3

void ADD_A_imm(uint8_t imm);
void ADC_A_imm(uint8_t imm);
void SUB_A_imm(uint8_t imm);
void SBC_A_imm(uint8_t imm);
void AND_A_imm(uint8_t imm);
void XOR_A_imm(uint8_t imm);
void OR_A_imm(uint8_t imm);
void CP_A_imm(uint8_t imm);

void RET();
void RETI();
void RET_C(enum cond c);
void JP(uint16_t imm16);
void JP_C(enum cond c, uint16_t imm16);
void JP_HL();
void CALL(uint16_t imm16);
void CALL_C(enum cond c, uint16_t imm16);
void RST(char target[3]);

void PUSH_(uint16_t *r16);
void POP_(uint16_t *r16);

void LDH_C_A();
void LDH_imm_A(uint8_t imm8);
void LD_addr_A(uint16_t addr);
void LDH_A_C();
void LDH_A_imm(uint8_t imm8);
void LD_A_addr(uint16_t addr);

void ADD_sp_imm(uint8_t imm8);
void LD_HL_sp_imm(uint8_t imm8);
void LD_sp_HL();

void DI();
void EI();

// CB block instructions
void RLC(uint8_t r8);
void RRC(uint8_t r8);
void RL(uint8_t r8);
void RR(uint8_t r8);
void SLA(uint8_t r8);
void SRA(uint8_t r8);
void SWAP(uint8_t r8);
void SRL(uint8_t r8);

// Decode 8-bit instructions
void decode8(uint8_t opcode, uint8_t nextByte, uint16_t nextWord);
void decodeCB(uint8_t opcode, uint8_t nextByte, uint16_t nextWord);

#endif
