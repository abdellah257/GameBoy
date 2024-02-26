#include <stdio.h>
#include <raylib.h>

#include <cpu.h>
#include <gpu.h>
#include <memoryUnit.h>

static const int screenWidth = 600;
static const int screenHeight = 400;

int main(){


    Z80 = initCPU();
    MMU = initMMU();
    GB_GPU = initGPU();

    InitWindow(screenWidth, screenHeight, "GameBoy Emulator");

    SetTargetFPS(24);

    while( !WindowShouldClose() )
    {
        BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Welcome to GameBoy !", 150, 200, 30, LIGHTGRAY);
        EndDrawing();
    }
    
    CloseWindow();
    return EXIT_SUCCESS;
}
