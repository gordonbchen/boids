#include <stdio.h>
#include <stdlib.h>
#include <math.h>


static const double M_PI = 3.14159265359;

static const int N_BIRDS = 2;
static const double SCREEN_DIST = 100.0;
static const double BLIND_DIST2 = 10.0*10.0;
static const double BLIND_ANGLE = (3.0 * M_PI) / 4.0;
static const double MAX_SPEED = 2.0;


double rand1() {
	return ((double) rand()) / RAND_MAX;
}

void update_bird(const int i, double* xs, double* ys, double* headings) {
	int n_neighbors = 0;
	double avg_x = 0.0;
	double avg_y = 0.0;
	double avg_heading = 0.0;

	for (int j = 0; j < N_BIRDS; ++j) {
		if (i == j) {continue;}

		double dx = xs[j] - xs[i];
		double dy = ys[j] - ys[i];
		double dist2 = (dx*dx) + (dy*dy);
		if (dist2 > BLIND_DIST2) {continue;}

		double angle = atan2(dy, dx);
		if (fabs(angle) > BLIND_ANGLE) {continue;}

		++n_neighbors;
		avg_x += xs[j];
		avg_y += ys[j];
		avg_heading += headings[j];
	}

	if (n_neighbors == 0) {return;}

	avg_x /= n_neighbors;
	avg_y /= n_neighbors;
	avg_heading /= n_neighbors;

	double dy = avg_y - ys[i];
	double dx = avg_x - xs[i];
	
	double cohesion_heading = atan2(dy, dx);
	double separation_heading = -cohesion_heading;
	
	double dist2 = (dx*dx) + (dy*dy);
	double cohesion_weight = dist2 / BLIND_DIST2;

	cohesion_heading *= cohesion_weight;
	separation_heading *= (1.0 - cohesion_weight);
	
	// TODO: steer away from walls.
	double new_heading = (avg_heading + cohesion_heading + separation_heading) / 3.0;
	
	// TODO: lerp with delta time.
	headings[i] = (0.9 * headings[i]) + (0.1 * new_heading);  

	xs[i] += MAX_SPEED * cos(headings[i]);
	ys[i] += MAX_SPEED * sin(headings[i]);
}

int main() {
	double* xs = malloc(sizeof(double) * N_BIRDS);
	double* ys = malloc(sizeof(double) * N_BIRDS);
	double* headings = malloc(sizeof(double) * N_BIRDS);  // right is 0, CCW +.
	// TODO: variable speed;

	srand(42);
	for (int i = 0; i < N_BIRDS; ++i) {
		xs[i] = SCREEN_DIST * rand1();
		ys[i] = SCREEN_DIST * rand1();
		headings[i] = M_PI * (2.0 *(rand1() - 0.5));
	}

	xs[0] = 0.0;
	ys[0] = 0.0;
	headings[0] = M_PI / 4.0;
	xs[1] = 5.0;
	ys[1] = 5.0;
	headings[1] = (-3.0 * M_PI) / 4.0;
	
	// TODO: raylib draw birds.
	for (int i = 0; i < N_BIRDS; ++i) {
		printf("%d:  %f %f %f\n", i, xs[i], ys[i], headings[i]);
		update_bird(i, xs, ys, headings);
		printf("%d:  %f %f %f\n", i, xs[i], ys[i], headings[i]);
	}

	free(xs);
	free(ys);
	free(headings);
}

