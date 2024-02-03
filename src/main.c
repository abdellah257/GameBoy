#include <cpu.h>
#include <memoryUnit.h>
#include <SDL.h>

SDL_Window* screen;
SDL_Renderer* render;

int main(){
    int status;

    Z80 = initCPU();
    MMU = initMMU();

    status = init_window();
    if(status) return EXIT_FAILURE;
    
    close_window();
    return EXIT_SUCCESS;
}


int init_window()
{

    if(!SDL_init(SDL_INIT_VIDEO)){
        printf("Unable to initialize SDL 2");
        return EXIT_FAILURE;
    }

    screen = SDL_CreateWindow("GameBoy", 0, 0, 200, 200, SDL_WINDOW_SHOWN);

    if(screen == NULL){
        printf("Unable to open Window");
        return EXIT_FAILURE;
    }

    render = SDL_CreateRenderer(screen, -1, SDL_RENDERER_SOFTWARE);
    if(!render){
        printf("Could not create renderer");
        return EXIT_FAILURE;
    }

    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderClear(render);
    SDL_RenderPresent(render);
    SDL_Delay(1000);

    return EXIT_SUCCESS;
}

int close_window()
{
    SDL_DestroyWindow(screen);
    SDL_Quit();

    return EXIT_SUCCESS;
}