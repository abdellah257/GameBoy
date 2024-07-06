#ifndef FETCH_DECODE_H
#define FETCH_DECODE_H

#include <stdint.h>
#include <stdio.h>

void read_ROM(char *filename);

void decode_instructions(uint16_t pc);

#endif