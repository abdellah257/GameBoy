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

/* Add Register Y to Register A (ADD A Y) */
void ADD_A(char Y);

void CP_A(char Y);

void NPO_();

void PUSH_(char* X, char* Y);

void POP_(char* X, char* Y);

void LD_A();

#endif
