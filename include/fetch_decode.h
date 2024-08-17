#ifndef FETCH_DECODE_H
#define FETCH_DECODE_H

#include <stdint.h>
#include <stdio.h>

void read_ROM(char *filename);

void execute_instruction(uint16_t pc);

uint8_t hex2int(char ch);

#endif