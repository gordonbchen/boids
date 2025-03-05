#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>

static const int N_BIRDS = 250;
static const double BIRD_SIZE = 25.0;
static const double INIT_VELOCITY_BIAS = 110.0;
static const double INIT_VELOCITY_SCALE = 20.0;

static const double BLIND_DIST = 200.0;
static const double SEPARATION_DIST = 50.0;
static const double COHESION_LERP = 0.015;
static const double SEPARATION_LERP = 0.05;
static const double ALIGNMENT_LERP = 0.01;
static const double MIN_VELOCITY = 90.0;
static const double MAX_VELOCITY = 300.0;
static const double BOUNDARY = 20.0;
static const double IN_BOUNDS_FACTOR = 0.6;

static const int SCREEN_WIDTH = 1600;
static const int SCREEN_HEIGHT = 1200;
static const int FPS = 30;

typedef struct Vector2d {
	double x;
	double y;
} Vector2d;

double rand1() {
	return ((double) rand()) / RAND_MAX;
}

double abs_clip(double x, double abs_max) {
	if (x > abs_max) {
		return abs_max;
	}
	if (x < -abs_max) {
		return -abs_max;
	}
	return x;
}

void update_velocity(const int i, Vector2d* positions, Vector2d* velocities) {
	double avg_x = 0.0;
	double avg_y = 0.0;
	double avg_vx = 0.0;
	double avg_vy = 0.0;
	int neighbors = 0;

	double delta_x = 0.0;
	double delta_y = 0.0;
	
	for (int j = 0; j < N_BIRDS; ++j) {
		if (j == i) {continue;}
		double dx = positions[j].x - positions[i].x;
		double dy = positions[j].y - positions[i].y;
		double dist = sqrt((dx*dx) + (dy*dy));
		if (dist > BLIND_DIST) {continue;}

		avg_x += positions[j].x;
		avg_y += positions[j].y;
		avg_vx += velocities[j].x;
		avg_vy += velocities[j].y;
		++neighbors;
		
		if (dist > SEPARATION_DIST) {continue;}
		delta_x -= dx;
		delta_y -= dy;
	}

	if (neighbors > 0) {
		avg_x /= neighbors;
		avg_y /= neighbors;
		avg_vx /= neighbors;
		avg_vy /= neighbors;

		velocities[i].x += COHESION_LERP * (avg_x - positions[i].x);
		velocities[i].y += COHESION_LERP * (avg_y - positions[i].y);
		velocities[i].x += ALIGNMENT_LERP * (avg_vx - velocities[i].x);
		velocities[i].y += ALIGNMENT_LERP * (avg_vy - velocities[i].y);
		velocities[i].x += SEPARATION_LERP * delta_x;
		velocities[i].y += SEPARATION_LERP * delta_y;
	}

	if (positions[i].x < BOUNDARY) {
		velocities[i].x += fabs(velocities[i].x) * IN_BOUNDS_FACTOR;
	}
	else if (positions[i].x > SCREEN_WIDTH-BOUNDARY) {
		velocities[i].x -= fabs(velocities[i].x) * IN_BOUNDS_FACTOR;
	}
	if (positions[i].y < BOUNDARY) {
		velocities[i].y += fabs(velocities[i].y) * IN_BOUNDS_FACTOR;
	}
	else if (positions[i].y > SCREEN_HEIGHT-BOUNDARY) {
		velocities[i].y -= fabs(velocities[i].y) * IN_BOUNDS_FACTOR;
	}
	
	double speed = sqrt((velocities[i].x)*(velocities[i].x)+(velocities[i].y)*(velocities[i].y));
	double unit_vx = velocities[i].x / speed;
	double unit_vy = velocities[i].y / speed;

	if (speed > MAX_VELOCITY) {
		velocities[i].x = unit_vx * MAX_VELOCITY;
		velocities[i].y = unit_vy * MAX_VELOCITY;
	}
	else if (speed < MIN_VELOCITY) {
		velocities[i].x = unit_vx * MIN_VELOCITY;
		velocities[i].y = unit_vy * MIN_VELOCITY;
	}
}

void update_bird(const int i, Vector2d* positions, Vector2d* velocities) {
	update_velocity(i, positions, velocities);
	positions[i].x += velocities[i].x / FPS;
	positions[i].y += velocities[i].y / FPS;
}

void draw_bird(Vector2d* position, Vector2d* velocity) {
	double speed = sqrt((velocity->x)*(velocity->x)+(velocity->y)*(velocity->y));
	double unit_x = velocity->x / speed;
	double unit_y = velocity->y / speed;
	
	double tip_dx = unit_x * BIRD_SIZE;
	double tip_dy = unit_y * BIRD_SIZE;

	double angle = atan2(unit_y, unit_x) + (PI / 4.0);
	double base_dx = cos(angle) * BIRD_SIZE / 4.0;
	double base_dy = sin(angle) * BIRD_SIZE / 4.0;

	DrawTriangle(
		(Vector2){position->x+tip_dx, position->y+tip_dy},
		(Vector2){position->x-base_dx, position->y-base_dy},
		(Vector2){position->x+base_dx, position->y+base_dy},
		BLUE
	);
}

int main() {
	Vector2d* positions = malloc(sizeof(Vector2d) * N_BIRDS);
	Vector2d* velocities = malloc(sizeof(Vector2d) * N_BIRDS);

	for (int i = 0; i < N_BIRDS; ++i) {
		positions[i].x = SCREEN_WIDTH * rand1();
		positions[i].y = SCREEN_HEIGHT * rand1();
		velocities[i].x = INIT_VELOCITY_BIAS + (INIT_VELOCITY_SCALE*rand1());
		velocities[i].y = INIT_VELOCITY_BIAS + (INIT_VELOCITY_SCALE*rand1());

		if (rand1() > 0.5) {velocities[i].x = -velocities[i].x;}
		if (rand1() > 0.5) {velocities[i].y = -velocities[i].y;}
	}

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "boids");
	SetTargetFPS(FPS);
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		for (int i = 0; i < N_BIRDS; ++i) {
			update_bird(i, positions, velocities);
			draw_bird(&positions[i], &velocities[i]);
		}
		EndDrawing();
	}
	CloseWindow();
	
	free(positions);
	free(velocities);
}

