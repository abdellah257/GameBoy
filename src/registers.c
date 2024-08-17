#include <registers.h>


Registers* initRegs(){
    Registers* p = (Registers*)malloc(sizeof(Registers));
    resetRegs(p);
    return p;
}

void resetRegs(Registers *p){
    p->A = 0; p->B = 0; p->C = 0; p->D = 0;
    p->E = 0; p->F = 0; p->H = 0; p->L = 0;
    p->AF = 0; p->BC = 0; p->DE = 0; p->HL = 0;
    p->SP = 0; p->PC = 0; p->m = 0;
}