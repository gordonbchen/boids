CC=gcc
CFLAGS=-g -Wall -std=c99 -lm

all: boids

boids: boids.c
	$(CC) -o boids boids.c $(CFLAGS)

clean:
	rm boids
