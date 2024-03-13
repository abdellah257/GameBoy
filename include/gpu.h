#ifndef GPU_H
#define GPU_H

#include <stdlib.h>
#include <raylib.h>

#define TILES_NUMBER 384
#define TILE_DIM 8

// Color Palette of the GameBoy
#define LIGHT_GRAY Color(192, 192, 192, 255)
#define DARK_GRAY Color(96, 96, 96, 255)
typedef struct Tile
{
    unsigned char p[TILE_DIM][TILE_DIM];
} Tile;
typedef struct GPU
{
    int mode;
    int modeclock;
    int line;

    int index;
    int screen_size[2];

    Tile tiles_set[TILES_NUMBER];
    Color *pixels;

} GPU;

// The GPU instance
extern GPU *GB_GPU;

// Initializes the GPU
GPU *initGPU(int width, int height);

// Resets the Tile Data Set
void resetTiles();

// Updates the GPU state after certain cpu cycles
void gpuStep();

// Updates the Tile Data Set from the VRAM
void updateTile(unsigned short addr, unsigned char val);

#endif