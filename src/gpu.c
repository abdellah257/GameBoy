#include <gpu.h>
#include <stdlib.h>

GPU* GB_GPU;

GPU* initGPU()
{
    GPU* gpu = (GPU*) malloc(sizeof(GPU));
    return gpu;
}