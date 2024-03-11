#ifndef GPU_H
#define GPU_H

#include <stdlib.h>
#include <raylib.h>

typedef struct GPU 
{
    int mode;
    int modeclock;
    int line;

    int index;
    int screen_size[2];
    char* vram;
    Color* pixels;

}GPU;


extern GPU* GB_GPU;


GPU* initGPU(int width, int height);

void resetVRAM();

void gpuStep();


#endif