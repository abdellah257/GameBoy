#include <stdio.h>
#include <fetch_decode.h>
#include <memoryUnit.h>
#include <cpu.h>

void read_ROM(char *filename){
    FILE *rom = fopen(filename, "rb");
    fread(MMU->bios, 256, 1, rom);
    fread(MMU->rom, 32512, 1, rom); 
    fclose(rom);
}

void decode_instructions(uint16_t pc){
    unsigned char opcode = rb(pc);
    uint8_t nextByte = rb(pc+1);
    uint16_t next2Bytes = rw(pc+1);

    decode8(opcode, nextByte, next2Bytes);
}