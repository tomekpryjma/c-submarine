#include "clock.h"
#include <stdio.h>
#include "../globals.h"

Clock* Clock_Alloc() {
	Clock* clock = malloc(sizeof(Clock));

	if (clock == NULL) {
		printf("Could not allocate memory for clock. Goodbye.");
		exit(1);
	}

	clock->now = SDL_GetTicks();
	clock->delta = 0;
	clock->last = SDL_GetTicks();

	return clock;
}
void Clock_Tick(Clock* clock) {
	clock->now = SDL_GetTicks();
	clock->delta = (SDL_GetTicks() - clock->last) / 1000.f;
	clock->last = SDL_GetTicks();

	if (clock->delta > DELTA_TIME_MAX) {
		clock->delta = DELTA_TIME_MAX;
	}
}