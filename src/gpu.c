#include <gpu.h>
#include <cpu.h>
#include <memoryUnit.h>
#include <raylib.h>
#include <stdlib.h>

GPU *GB_GPU;

GPU *initGPU(int width, int height)
{
    GPU *gpu = (GPU *)malloc(sizeof(GPU));
    gpu->screen_size[0] = width;
    gpu->screen_size[0] = height;

    gpu->pixels = (Color *)calloc(width * height, sizeof(Color));

    for (int i = 0; i < width / 2; i++)
    {
        for (int j = 0; j < height; j++)
        {
            gpu->pixels[j * width + i].a = 255;
            gpu->pixels[j * width + i].b = 255;
            gpu->pixels[j * width + i].g = 255;
            gpu->pixels[j * width + i].r = 255;
        }
    }
    return gpu;
}

void resetTiles()
{
    for (int i = 0; i < TILES_NUMBER; i++)
    {
        Tile temp = GB_GPU->tiles_set[i];
        for (int j = 0; j < TILE_DIM; j++)
        {
            for (int k = 0; k < TILE_DIM; k++)
            {
                temp.p[j][k] = 0;
            }
        }
    }
}

void gpuStep()
{
    GB_GPU->modeclock += Z80->t;

    switch (GB_GPU->mode)
    {
    // Hblank
    case 0:
        if (GB_GPU->modeclock >= 204)
        {
            GB_GPU->modeclock = 0;
            GB_GPU->line++;
            if (GB_GPU->line == 143)
            {
                // Enter vblank
                GB_GPU->mode = 1;
                // TODO: ....
            }
            else
            {
                GB_GPU->mode = 2;
            }
        }
        break;
    // Vblank (10 lines)
    case 1:
        if (GB_GPU->modeclock >= 456)
        {
            GB_GPU->modeclock = 0;
            GB_GPU->line++;

            if (GB_GPU->line > 153)
            {
                // Restart scanning
                GB_GPU->mode = 2;
                GB_GPU->line = 0;
            }
        }
        break;
    // OAM Read Mode, scaline active
    case 2:
        if (GB_GPU->modeclock >= 80)
        {
            GB_GPU->modeclock = 0;
            GB_GPU->mode = 3;
        }
        break;
    // VRAM read mode, scaline active
    case 3:
        if (GB_GPU->modeclock >= 172)
        {
            GB_GPU->modeclock = 0;
            GB_GPU->mode = 0;

            // TODO: write a scanline to framebuffer
        }
        break;
    }
}

void updateTile(unsigned short addr, unsigned char val)
{
    unsigned short temp = addr & 0x1FFE;

    unsigned char tileIndex = (temp >> 4) & 511;
    unsigned char line = (temp >> 1) & 7;

    for (int i = 0; i < 8; i++)
    {
        unsigned char index = 1 << (7 - i);
        GB_GPU->tiles_set[tileIndex].p[line][i] = ((MMU->vram[temp] & index) ? 1 : 0) + ((MMU->vram[temp + 1] & index) ? 2 : 0);
    }
}