#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "cpu.h"

/* Memory Manangement Unit */
typedef struct MemoryUnit
{
    bool inbios;

    unsigned char *bios;
    unsigned char *rom;
    unsigned char *wram;
    unsigned char *eram;
    unsigned char *zram;
    unsigned char *vram;
} MemoryUnit;

// The MMU instance
extern MemoryUnit *MMU;

/* Initialize the Memory MU */
MemoryUnit *initMMU();

// Resets the MMU
void resetMMU();

// Load the ROM
void loadROM();

// Read 8 bit data
char rb(uint16_t addr);

// Read 16 bit data
short rw(uint16_t addr);

// Write 8 bit data
void wb(uint16_t addr, unsigned char val);

// Write 16 bit data
void ww(uint16_t addr, unsigned char val[2]);

#endif