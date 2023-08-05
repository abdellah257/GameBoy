#include "registers.h"
#include <stdint.h>

/* Z80 CPU Structure */
struct CPU
{
    /* data */
    Registers* R;

    uint8_t m;
    uint8_t t;


};

/* Initialize the CPU */
CPU* initCPU(){
    CPU* p = (CPU*)malloc(sizeof(CPU));
    p->R = initRegs();
    p->m = 0;
    p->t = 0;
    
    return p;
}



