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
    unsigned char nextByte = rb(pc+1);
    unsigned short next2Bytes = rw(pc+1);

    switch(opcode){
        case 0x00: NOP_(); break;
        case 0x01: LD_imm(&Z80->R->BC, nextByte); break;
        case 0x02: LD_mm_A(Z80->R->BC); break;
        case 0x03: INC_16(&Z80->R->BC); break;
        case 0x04: INC(&Z80->R->BC[0]); break;
        case 0x05: DEC(&Z80->R->BC[0]); break;
        case 0x06: LD_r8_imm(&Z80->R->BC[0], nextByte); break;
        case 0x07: RLCA(); break;
        case 0x08: LD_mm_sp(next2Bytes); break;
        case 0x09: ADD_hl_r16(&Z80->R->BC); break;
        case 0x0A: LD_A_mm(Z80->R->BC); break;
        case 0x0B: DEC_16(&Z80->R->BC); break;
        case 0x0C: INC(&Z80->R->BC[1]); break;
        case 0x0D: DEC(&Z80->R->BC[1]); break;
        // case 0x0E: LD_imm(&Z80->R->BC[1], nextByte); break;
        case 0x0F: RRCA(); break;
        // case 0x10: STOP(); break;
        case 0x11: LD_imm(Z80->R->DE, next2Bytes); break;
        case 0x12: LD_mm_A(Z80->R->DE); break;
        case 0x13: INC_16(&Z80->R->DE); break;
        case 0x14: INC(&Z80->R->DE[0]); break;
        case 0x15: DEC(&Z80->R->DE[0]); break;
        case 0x16: LD_r8_imm(&Z80->R->DE[0], nextByte); break;
        case 0x17: RLA(); break;
        case 0x18: JR(next2Bytes); break;
        case 0x19: ADD_hl_r16(&Z80->R->DE); break;
        case 0x1A: LD_A_mm(Z80->R->DE); break;
        case 0x1B: DEC_16(&Z80->R->DE); break;
        case 0x1C: INC(Z80->R->DE[1]); break;
        case 0x1D: DEC(Z80->R->DE[1]); break;
        case 0x1E: LD_imm(Z80->R->DE[1], nextByte); break;
        case 0x1F: RRA(); break;
        case 0x20: JR_C(0, nextByte); break;
        case 0x21: LD_imm(Z80->R->HL, nextByte); break;
        case 0x22: LD_mm_A(Z80->R->HL); break;
        case 0x23: INC_16(Z80->R->HL); break;
        case 0x24: INC(Z80->R->HL[0]); break;
        case 0x25: DEC(Z80->R->HL[0]); break;
        case 0x26: LD_r8_imm(Z80->R->HL[0], nextByte); break;
        case 0x27: DAA(); break;
        case 0x28: JR_C(1, nextByte); break;
        case 0x29: ADD_hl_r16(Z80->R->HL); break;
        case 0x2A: LD_A_mm(Z80->R->HL); break;
        case 0x2B: DEC_16(Z80->R->HL); break;
        case 0x2C: INC(Z80->R->HL[1]); break;
        case 0x2D: DEC(Z80->R->HL[1]); break;
        case 0x2E: LD_imm(Z80->R->HL[1], nextByte); break;
        case 0x2F: CPL(); break;
        case 0x30: JR_C(2, nextByte); break;
        case 0x31: LD_imm(Z80->sp, nextByte); break;
        case 0x32: LD_mm_A(Z80->R->HL); break;
        case 0x33: INC_16(Z80->sp); break;
        case 0x34: INC(Z80->R->HL); break;
        case 0x35: DEC(Z80->R->HL); break;
        case 0x36: LD_imm(Z80->R->HL, nextByte); break;
        case 0x37: SCF(); break;
        case 0x38: JR_C(3, nextByte); break;
        case 0x39: ADD_hl_r16(Z80->sp); break;
        case 0x3A: LD_A_mm(Z80->R->HL); break;
        case 0x3B: DEC_16(Z80->sp); break;
        case 0x3C: INC(Z80->R->A); break;
        case 0x3D: DEC(Z80->R->A); break;
        case 0x3E: LD_imm(Z80->R->A, nextByte); break;
        case 0x3F: CCF(); break;
        case 0x40: LD_(&Z80->R->BC[0], &Z80->R->BC[0]); break;
        case 0x41: LD_(&Z80->R->BC[0], &Z80->R->BC[1]); break;
        case 0x42: LD_(&Z80->R->BC[0], &Z80->R->DE[0]); break;
        case 0x43: LD_(&Z80->R->BC[0], &Z80->R->DE[1]); break;
        case 0x44: LD_(&Z80->R->BC[0], &Z80->R->HL[0]); break;
        case 0x45: LD_(&Z80->R->BC[0], &Z80->R->HL[1]); break;
        // case 0x46: LD_(&Z80->R->BC[0], Z80->R->HL); break;
        case 0x47: LD_(&Z80->R->BC[0], &Z80->R->A); break;
        case 0x48: LD_(&Z80->R->BC[1], &Z80->R->BC[0]); break;
        case 0x49: LD_(&Z80->R->BC[1], &Z80->R->BC[1]); break;
        case 0x4A: LD_(&Z80->R->BC[1], &Z80->R->DE[0]); break;
        case 0x4B: LD_(&Z80->R->BC[1], &Z80->R->DE[1]); break;
        case 0x4C: LD_(&Z80->R->BC[1], &Z80->R->HL[0]); break;
        case 0x4D: LD_(&Z80->R->BC[1], &Z80->R->HL[1]); break;
        // case 0x4E: LD_(&Z80->R->BC[1], &Z80->R->HL); break;
        case 0x4F: LD_(&Z80->R->BC[1], &Z80->R->A); break;
        case 0x50: LD_(&Z80->R->DE[0], &Z80->R->BC[0]); break;
        case 0x51: LD_(&Z80->R->DE[0], &Z80->R->BC[1]); break;
        case 0x52: LD_(&Z80->R->DE[0], &Z80->R->DE[0]); break;
        case 0x53: LD_(&Z80->R->DE[0], &Z80->R->DE[1]); break;
        case 0x54: LD_(&Z80->R->DE[0], &Z80->R->HL[0]); break;
        case 0x55: LD_(&Z80->R->DE[0], &Z80->R->HL[1]); break;
        // case 0x56: LD_(&Z80->R->DE[0], Z80->R->HL); break;
        case 0x57: LD_(&Z80->R->DE[0], &Z80->R->A); break;
        case 0x58: LD_(&Z80->R->DE[1], &Z80->R->BC[0]); break;
        case 0x59: LD_(&Z80->R->DE[1], &Z80->R->BC[1]); break;
        case 0x5A: LD_(&Z80->R->DE[1], &Z80->R->DE[0]); break;
        case 0x5B: LD_(&Z80->R->DE[1], &Z80->R->DE[1]); break;
        case 0x5C: LD_(&Z80->R->DE[1], &Z80->R->HL[0]); break;
        case 0x5D: LD_(&Z80->R->DE[1], &Z80->R->HL[1]); break;
        // case 0x5E: LD_(&Z80->R->DE[1], &Z80->R->HL); break;
        case 0x5F: LD_(&Z80->R->DE[1], &Z80->R->A); break;
        case 0x60: LD_(&Z80->R->HL[0], &Z80->R->BC[0]); break;
        case 0x61: LD_(&Z80->R->HL[0], &Z80->R->BC[1]); break;
        case 0x62: LD_(&Z80->R->HL[0], &Z80->R->DE[0]); break;
        case 0x63: LD_(&Z80->R->HL[0], &Z80->R->DE[1]); break;
        case 0x64: LD_(&Z80->R->HL[0], &Z80->R->HL[0]); break;
        case 0x65: LD_(&Z80->R->HL[0], &Z80->R->HL[1]); break;
        // case 0x66: LD_(&Z80->R->HL[0], &Z80->R->HL); break;
        case 0x67: LD_(&Z80->R->HL[0], &Z80->R->A); break;
        case 0x68: LD_(&Z80->R->HL[1], &Z80->R->BC[0]); break;
        case 0x69: LD_(&Z80->R->HL[1], &Z80->R->BC[1]); break;
        case 0x6A: LD_(&Z80->R->HL[1], &Z80->R->DE[0]); break;
        case 0x6B: LD_(&Z80->R->HL[1], &Z80->R->DE[1]); break;
        case 0x6C: LD_(&Z80->R->HL[1], &Z80->R->HL[0]); break;
        case 0x6D: LD_(&Z80->R->HL[1], &Z80->R->HL[1]); break;
        // case 0x6E: LD_(&Z80->R->HL[1], &Z80->R->HL); break;
        case 0x6F: LD_(&Z80->R->HL[1], &Z80->R->A); break;
        // case 0x70: LD_(&Z80->R->HL, &Z80->R->BC[0]); break;
        // case 0x71: LD_(&Z80->R->HL, &Z80->R->BC[1]); break;
        // case 0x72: LD_(&Z80->R->HL, &Z80->R->DE[0]); break;
        // case 0x73: LD_(&Z80->R->HL, &Z80->R->DE[1]); break;
        // case 0x74: LD_(&Z80->R->HL, &Z80->R->HL[0]); break;
        // case 0x75: LD_(&Z80->R->HL, &Z80->R->HL[1]); break;
        case 0x76: HALT(); break;
        default: break;
    }
}