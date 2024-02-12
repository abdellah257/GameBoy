#include <stdio.h>
#include <raylib.h>

#include <cpu.h>
#include <gpu.h>
#include <memoryUnit.h>

static const int screenWidth = 180;
static const int screenHeight = 140;

int main(){

    int status;

    Z80 = initCPU();
    MMU = initMMU();
    GB_GPU = initGPU();

    InitWindow(screenWidth, screenHeight, "GameBoy Emulator");

    SetTargetFPS(24);

    while(! WindowShouldClose())
    {
        // Update the Draw Frame
    }
    
    CloseWindow();
    return EXIT_SUCCESS;
}
