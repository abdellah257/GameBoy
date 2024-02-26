CC = gcc
CFLAGS = -g -std=gnu11 -Wall -Wparentheses
INCLUDES = -I$(GAMEBOY)/include -I$(GAMEBOY)/packages/raylib-5.0/include
LIBS = -L$(GAMEBOY)/packages/raylib-5.0/lib/
LISTSRC = cpu gpu memoryUnit registers

PATH2SRC = $(GAMEBOY)/src
PATH2OBJ = $(GAMEBOY)/build
SRCFILES = $(patsubst %, $(PATH2SRC)/%.c, $(LISTSRC))
OBJFILES = $(patsubst $(PATH2SRC)%.c, $(PATH2OBJ)%.o, $(SRCFILES))
MAINSRC = $(PATH2SRC)/main.c

BINFILE = $(GAMEBOY)/build/GameBoy.exe

all: $(BINFILE)

$(BINFILE): obj
	$(CC) -o $@ $(CFLAGS) $(MAINSRC) $(OBJFILES) $(INCLUDES) $(LIBS) -lraylib

obj: $(OBJFILES)

$(PATH2OBJ)/%.o: $(PATH2SRC)/%.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $(LIBS) -lraylib $< -o $@

clean:
	rm -rf build/* 
