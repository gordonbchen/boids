CC=gcc
CFLAGS=-g -Wall -std=c99 -lm

RAYLIB_FLAGS= -I./include -L./lib -lraylib -lpthread -ldl -lX11

all: boids

boids: boids.c
	$(CC) -o boids boids.c $(CFLAGS) $(RAYLIB_FLAGS)

clean:
	rm boids
