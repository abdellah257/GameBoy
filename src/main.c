#include <stdio.h>
#include <raylib.h>

#include <log.h>
#include <cpu.h>
#include <gpu.h>
#include <screen.h>
#include <memoryUnit.h>
#include <fetch_decode.h>

static const int scale = 4;
static const int screenWidth = scale*160;     // 160  16x16 tiles
static const int screenHeight = scale*144;    // 144 


void frameStep(uint8_t breakpoint){
    uint16_t frameClock = Z80->clock_m + 17556;

    if (breakpoint > 0){
        while(Z80->clock_m < Z80->clock_m + breakpoint){
            if(Z80->halt){
                Z80->R->m = 1;
            }
            execute_instruction(Z80->R->PC);
            Z80->R->PC++;
            Z80->clock_m += Z80->R->m;
        }
    }else{
        while(Z80->clock_m < frameClock){
        execute_instruction(Z80->R->PC);
        Z80->R->PC++;
        Z80->clock_m += Z80->R->m;
        }
    }
}
int main(int argc, char *argv[])
{

    Z80 = initCPU();
    MMU = initMMU();
    GB_GPU = initGPU();
    GB_Win = initScreen(screenWidth, screenHeight);

    // Load ROM 
    log_info("Loading ROM: %s", argv[1]);
    char *filename = argv[1];
    resetCPU();
    read_ROM(filename);


    // Print bios loaded
    log_info("BIOS loaded");

    InitWindow(screenWidth, screenHeight, "GameBoy Emulator");

    SetTargetFPS(24);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        
        // frameStep(1);

        for (int x = 0; x < screenWidth; x++)
        {
            for (int y = 0; y < screenHeight; y++)
            {
                DrawPixel(x, y, GB_Win->pixels[y * screenWidth + x]);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return EXIT_SUCCESS;
}
