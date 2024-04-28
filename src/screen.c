#include <screen.h>
#include <gpu.h>

#include <raylib.h>

Screen* GB_Win;

Screen* initScreen(int w, int h){
    
    Screen* s = (Screen*)malloc(sizeof(Screen));
    s->width = w;
    s->height = h;
    s->pixels = (Color*)malloc(sizeof(Color)*w*h);

    return s;
}

void computeColor(unsigned char tile_palette, Color* c)
{
    switch (tile_palette){
        case 0:
        case 1:
        case 2:
        case 3:
        default:
            break;
    }
}
/*
* Update the Pixels of the Background from the GPU
*/
void updateScreen(){
    // 1- Loop over the the Tile Map
    // 2- Associate each pixel integer to the Palette
    // 3- Update the pixels of the screen 

    int offsetX = GB_GPU->cX;
    int offsetY = GB_GPU->cY;

    for (int i=0; i<GB_Win->width; i++){
        for(int j=0; j<GB_Win->height; j++){

        }
    }
}