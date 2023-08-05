#include <stdlib.h>
#include <stdint.h>
#include "cpu.h"

/* Memory Manangement Unit */
struct MMU {

    bool inbios;

    char* bios;
    char* rom;
    char* wram;
    char* eram;
    char* zram;
    char* vram;
};

/* Initialize the Memory MU */
MMU* initMMU(){
    MMU* p = (MMU*) malloc(sizeof(MMU));
    p->inbios = false;
    return p;
}

/* Read 8 bit data */
char rb(CPU* cpu,MMU* munit, uint16_t addr){
    switch (addr & 0xF000)
    {
    case 0x0000:
        /* code */
        if(munit->inbios){
            if(addr < 0x0100){
                return munit->bios[addr];
            }
            else if(cpu->R->sp == 0x100){
                munit->inbios = true;
            }
        }
        return munit->bios[addr];
    
    // ROM 0
    case 0x1000:
    case 0x2000:
    case 0x3000:
        return munit->rom[addr];
    // ROM 1 unbanked
    case 0x4000:
    case 0x5000:
    case 0x6000:
    case 0x7000:
        return munit->rom[addr];

    case 0x8000:
    case 0x9000:
        return munit->vram[addr & 0x1FFF];

    case 0xA000:
    case 0xB000:
        return munit->eram[addr & 0x1FFF];
    
    case 0xC000:
    case 0xD000:
        return munit->wram[addr & 0x1FFF];
    case 0xE000:
        return munit->wram[addr & 0x1FFF];
    case 0xF000:
        switch (addr & 0x0F00)
        {
        case 0x000: case 0x100: case 0x200: case 0x300:
        case 0x400: case 0x500: case 0x600: case 0x700:
        case 0x800: case 0x900: case 0xA00: case 0xB00:
        case 0xC00: case 0xD00:
            return munit->wram[addr & 0x1FFF];
        case 0xF00:
            if(addr >= 0xFF80)
			{
			    return munit->zram[addr & 0x7F];
			}
			else
			{
			    // I/O control handling
			    // Currently unhandled
			    return 0;
			}
        default:
            return -1;
        }
    default:
        return -1;
    }
}

/* Read 16 bit data */
short rw(CPU* cpu, MMU* munit, uint16_t addr){
    return rb(cpu, munit, addr) + (rb(cpu, munit, addr+1) << 8);
}