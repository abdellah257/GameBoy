#include <stdio.h>
#include <raylib.h>

#include <cpu.h>
#include <gpu.h>
#include <memoryUnit.h>

static const int screenWidth = 160;
static const int screenHeight = 144;

int main()
{

    Z80 = initCPU();
    MMU = initMMU();
    GB_GPU = initGPU(screenWidth, screenHeight);

    InitWindow(screenWidth, screenHeight, "GameBoy Emulator");

    SetTargetFPS(24);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Welcome to GameBoy !", 30, 70, 5, LIGHTGRAY);
        for (int x = 0; x < screenWidth; x++)
        {
            for (int y = 0; y < screenHeight; y++)
            {
                DrawPixel(x, y, GB_GPU->pixels[y * screenWidth + x]);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return EXIT_SUCCESS;
}
