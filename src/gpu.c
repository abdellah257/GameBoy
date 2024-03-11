#include <gpu.h>
#include <cpu.h>
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

void gpuStep()
{
    GB_GPU->modeclock += Z80->t;

    switch(GB_GPU->mode)
    {
        // Hblank
        case 0:
            if( GB_GPU->modeclock >= 204 ){
                GB_GPU->modeclock = 0;
                GB_GPU->line++;
                if( GB_GPU->line == 143 ){
                    // Enter vblank
                    GB_GPU->mode = 1;
                    // TODO: ....
                } else {
                    GB_GPU->mode = 2;
                }
            }
            break;
        // Vblank (10 lines)
        case 1:
            if( GB_GPU->modeclock >= 456 )
            {
                GB_GPU->modeclock = 0;
                GB_GPU->line++;

                if(GB_GPU->line > 153)
                {
                    // Restart scanning
                    GB_GPU->mode = 2;
                    GB_GPU->line = 0;
                }
            }
            break;
        // OAM Read Mode, scaline active
        case 2:
            if(GB_GPU->modeclock >= 80){
                GB_GPU->modeclock = 0;
                GB_GPU->mode = 3;
            }
            break;
        // VRAM read mode, scaline active
        case 3:
            if(GB_GPU->modeclock >= 172){
                GB_GPU->modeclock = 0;
                GB_GPU->mode = 0;
                
                // TODO: write a scanline to framebuffer
            }
            break;
    }
}