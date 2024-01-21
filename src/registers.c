#include <registers.h>


Registers* initRegs(){

    Registers* p = (Registers*)malloc(sizeof(Registers));

    p->A = 0; p->BC[0] = 0; p->BC[1] = 0; p->DE[0] = 0;
    p->DE[1] = 0; p->F = 0; p->HL[0] = 0; p->HL[1] = 0;

    return p;
}