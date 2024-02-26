#include <gpu.h>
#include <raylib.h>
#include <stdlib.h>

GPU* GB_GPU;

GPU* initGPU(int width, int height)
{
    GPU* gpu = (GPU*) malloc(sizeof(GPU));
    gpu->screen_size[0] = width;
    gpu->screen_size[0] = height;

    gpu->pixels = (Color*) calloc(width*height, sizeof(Color));
    
    for(int i = 0; i<width/2; i++){
        for(int j = 0; j<height; j++){
            gpu->pixels[j*width+i].a = 255;
            gpu->pixels[j*width+i].b = 255;
            gpu->pixels[j*width+i].g = 255;
            gpu->pixels[j*width+i].r = 255;
        }
    }
    return gpu;
}