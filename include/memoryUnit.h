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

    unsigned char bios[256];        // BIOS
    unsigned char rom[32512];       // ROM
    unsigned char vram[8192];       // Video RAM
    unsigned char eram[8192];       // External RAM
    unsigned char wram[8192];       // Working RAM
    unsigned char wshadow[7680];    // Working Shadow RAM
    unsigned char sram[160];        // Rendered Sprite information
    unsigned char mmio[128];        // Memory Mapped I/O
    unsigned char zram[128];        // Zero Page RAM
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