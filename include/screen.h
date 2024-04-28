#ifndef SCREEN_H
#define SCREEN_H

#include <raylib.h>

typedef struct Screen {

    Color *pixels;
    int width;
    int height;
    int resize;

} Screen;

extern Screen *GB_Win;

Screen* initScreen(int width, int height);

void computeColor(unsigned char tile_palette, Color* c);

void updateScreen();

#endif