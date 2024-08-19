#include <stdio.h>
#include <stdlib.h>
#include <fetch_decode.h>
#include <memoryUnit.h>
#include <cpu.h>

uint8_t ascii_to_hex(char c){
    uint8_t num = (uint8_t) c;
    if(num < 58 && num > 47)
    {
        return num - 48; 
    }
    if(num < 71 && num > 64)
    {
        return num - 55;
    }
    return num;
}

void read_ROM(char *filename){
    FILE *rom = fopen(filename, "rb");
    if (rom == NULL){
        printf("Error opening %s\n", filename);
        exit(1);
    }
    uint8_t sum;
    unsigned char c;
    int i = 1; int counter = 0;
    while (counter < 256 && !feof(rom)) {
        c = fgetc(rom);
        if (c == '\n') continue;
        if (i == 1) {
            sum = ascii_to_hex(c) << 4;
            i = 2;
        } else {
            sum = sum | ascii_to_hex(c);
            MMU->bios[counter] = sum;
            counter ++;
            i = 1;
        }
    }
    fclose(rom);
}

void execute_instruction(uint16_t pc){
    unsigned char opcode = rb(pc);
    uint8_t nextByte = rb(pc+1);
    uint16_t next2Bytes = rw(pc+1);
    decode8(opcode, nextByte, next2Bytes);
}