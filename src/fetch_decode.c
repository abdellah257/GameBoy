#include <stdio.h>
#include <stdlib.h>

#include <fetch_decode.h>
#include <memoryUnit.h>
#include <cpu.h>
#include <log.h>

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
    int counter = 0;
    log_info("Loading BIOS ...");
    while (counter < 256 && !feof(rom)) {
        c = fgetc(rom);
        if (c == '\n') continue;    // Skip return character
        sum = ascii_to_hex(c) << 4;
        c = fgetc(rom);
        sum = sum | ascii_to_hex(c);
        MMU->bios[counter] = sum;
        counter ++;
    }
    log_info("Loading ROM ...");
    counter = 0;
    while(counter < 32512 && !feof(rom)){
        c = fgetc(rom);
        if (c == '\n') continue;
        sum = ascii_to_hex(c) << 4;
        sum = sum | ascii_to_hex(c);
        MMU->rom[counter] = sum;
        counter ++;
    }
    fclose(rom);
}

void execute_instruction(uint16_t pc){
    unsigned char opcode = rb(pc);
    uint8_t nextByte = rb(pc+1);
    uint16_t next2Bytes = rw(pc+1);
    decode8(opcode, nextByte, next2Bytes);
}