#include <stdio.h>
#include <raylib.h>

#include <cpu.h>
#include <gpu.h>
#include <memoryUnit.h>

static const int screenWidth = 800;
static const int screenHeight = 600;

int main(){


    Z80 = initCPU();
    MMU = initMMU();
    GB_GPU = initGPU();

    InitWindow(screenWidth, screenHeight, "GameBoy Emulator");

    SetTargetFPS(24);

    while( !WindowShouldClose() )
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }
    
    CloseWindow();
    return EXIT_SUCCESS;
}
