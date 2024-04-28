#ifndef GPU_H
#define GPU_H

#include <stdlib.h>
#include <raylib.h>

#define TILES_NUMBER 384
#define TILE_DIM 8
#define BGD_DIM 256

// Color Palette of the GameBoy
#define LIGHT_GRAY Color(192, 192, 192, 255)
#define DARK_GRAY Color(96, 96, 96, 255)

// Each Pixel in the Tile is represented by 2 bits
typedef struct Tile{
    unsigned char p[2][TILE_DIM];
} Tile;

typedef struct TileMap{
    unsigned char map[1024];
}TileMap;

typedef struct GPU
{
    int mode;                                   // GPU Mode
    int modeclock; 
    int line;
    int index;

    Tile tiles_set[TILES_NUMBER];               // The Set of unique Tiles
    TileMap tile_maps[2];                       // the Tile Maps 

    unsigned char cX, cY;                       // Scroll X, Scroll Y ; GPU registers
    unsigned char background[BGD_DIM][BGD_DIM]; // Screen Background Computed
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

// Updates the Tile Map from the VRAM
void updateMap(unsigned short addr, unsigned char val);

// Return the palette of a pixel within a Tile (0...3)
int getPixelFromTile(Tile* t, int x, int y);

// Compute the Screen Background from the TileMap
void computeBackground();

#endif