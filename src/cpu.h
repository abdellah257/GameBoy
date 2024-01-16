#ifndef CPU_H
#define CPU_H

#include <registers.h>
#include <stdint.h>

/* Z80 CPU Structure */
typedef struct CPU {
    /* data */
    struct Registers* R;

    char pc; char sp;
    unsigned char m;
    unsigned char t;

} CPU;

CPU *Z80;
/* Initialize the CPU */
CPU* initCPU();

/* Add Register Y to Register X (ADD X Y) */
void ADD_(char* X, char* Y);

#endif
