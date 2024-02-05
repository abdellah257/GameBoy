#include <gpu.h>
#include <stdlib.h>


GPU* initGPU()
{
    GPU* gpu = (GPU*) malloc(sizeof(GPU));
    return gpu;
}