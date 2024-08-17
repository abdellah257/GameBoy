#include <stdio.h>
#include <fetch_decode.h>
#include <memoryUnit.h>
#include <cpu.h>

uint8_t hex2int(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return -1;
}
void read_ROM(char *filename){
    FILE *rom = fopen(filename, "rb");
    if (rom == NULL){
        printf("Error opening %s\n", filename);
        exit(1);
    }
    for (int i = 0; i < 256; i++){
        char temp = 0;
        char val = 0;
        fread(&temp, 1, 1, rom);
        val = temp >> 4;
        fread(&temp, 1, 1, rom);
        val |= temp;
        MMU->bios[i] = val;
    }
    
    // fread(MMU->rom, 32512, 1, rom); 
    fclose(rom);
}

void execute_instruction(uint16_t pc){
    unsigned char opcode = rb(pc);
    uint8_t nextByte = rb(pc+1);
    uint16_t next2Bytes = rw(pc+1);
    decode8(opcode, nextByte, next2Bytes);
}