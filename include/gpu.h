#ifndef GPU_H
#define GPU_H

#include <stdlib.h>

typedef struct GPU 
{
    int index;
    char* vram;

}GPU;


extern GPU* GB_GPU;


GPU* initGPU();

void resetVRAM();



#endif