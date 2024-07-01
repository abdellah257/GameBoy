#ifndef KEY_H
#define KEY_H

#include <stdlib.h>

typedef struct Keypad {

    unsigned char rows[2];
    unsigned char columns;

} Keypad;

extern Keypad *GB_KEYS;

void reset_keys();

void updateMMIO();

void key_up();

void key_down();


#endif