#include <stdio.h>
#include <raylib.h>

#include <cpu.h>
#include <gpu.h>
#include <screen.h>
#include <memoryUnit.h>

static const int scale = 4;
static const int screenWidth = scale*160;     // 160  16x16 tiles
static const int screenHeight = scale*144;    // 144 

int main()
{

    Z80 = initCPU();
    MMU = initMMU();
    GB_GPU = initGPU();
    GB_Win = initScreen(screenWidth, screenHeight);

    InitWindow(screenWidth, screenHeight, "GameBoy Emulator");

    SetTargetFPS(24);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Welcome to GameBoy !", scale*50, scale*70, scale*6, LIGHTGRAY);
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
