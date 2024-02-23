#include <stdint.h>
#include <memoryUnit.h>
#include <cpu.h>

MemoryUnit* MMU;

MemoryUnit* initMMU(){
    MemoryUnit* p = malloc(sizeof(MemoryUnit));
    p->inbios = false;
    return p;
}

char rb(uint16_t addr){
    switch (addr & 0xF000)
    {
    case 0x0000:
        /* code */
        if(MMU->inbios){
            if(addr < 0x0100){
                return MMU->bios[addr];
            }
            else if(Z80->sp == 0x100){
                MMU->inbios = true;
            }
        }
        return MMU->bios[addr];
    
    // ROM 0
    case 0x1000:
    case 0x2000:
    case 0x3000:
        return MMU->rom[addr];
    // ROM 1 unbanked
    case 0x4000:
    case 0x5000:
    case 0x6000:
    case 0x7000:
        return MMU->rom[addr];

    case 0x8000:
    case 0x9000:
        return MMU->vram[addr & 0x1FFF];

    case 0xA000:
    case 0xB000:
        return MMU->eram[addr & 0x1FFF];
    
    case 0xC000:
    case 0xD000:
        return MMU->wram[addr & 0x1FFF];
    case 0xE000:
        return MMU->wram[addr & 0x1FFF];
    case 0xF000:
        switch (addr & 0x0F00)
        {
        case 0x000: case 0x100: case 0x200: case 0x300:
        case 0x400: case 0x500: case 0x600: case 0x700:
        case 0x800: case 0x900: case 0xA00: case 0xB00:
        case 0xC00: case 0xD00:
            return MMU->wram[addr & 0x1FFF];
        case 0xF00:
            if(addr >= 0xFF80)
			{
			    return MMU->zram[addr & 0x7F];
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

short rw(uint16_t addr){
    return rb(addr) + (rb(addr+1) << 8);
}


void wb(uint16_t addr, unsigned char val)
{

}

void ww(uint16_t add, unsigned char val[2])
{

}