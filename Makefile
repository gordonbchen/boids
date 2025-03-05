CC=gcc
CFLAGS=-g -Wall -std=c99

RAYLIB_FLAGS= -I./include -L./lib -lraylib -lpthread -ldl -lX11 -lm

all: boids

boids: boids.c
	$(CC) -o boids boids.c $(CFLAGS) $(RAYLIB_FLAGS)

clean:
	rm boids
