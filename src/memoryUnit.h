#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "cpu.h"

/* Memory Manangement Unit */
typedef struct MemoryUnit {

    bool inbios;

    char* bios;
    char* rom;
    char* wram;
    char* eram;
    char* zram;
    char* vram;
} MemoryUnit;

MemoryUnit* MMU;

/* Initialize the Memory MU */
MemoryUnit* initMMU();

void resetMMU();

void loadROM();

/* Read 8 bit data */
char rb(uint16_t addr);

/* Read 16 bit data */
short rw(uint16_t addr);

void wb(uint16_t addr, char val);

void ww(uint16_t addr, char val[2]);

#endif