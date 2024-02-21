CC = gcc
CFLAGS = -g -std=c99 -Wall
INCLUDES = -I /include  -I packages/raylib-5.0/include
LIBS = -L packages/raylib-5.0/lib

SRC = cpu.c gpu.c memotyUnit.c registers.c 


all: obj
	$(CC) -o $(CFLAGS) 

obj:


clean:
	cd build && rm -rf *