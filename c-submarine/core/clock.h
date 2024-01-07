#pragma once

#include <SDL.h>

typedef struct Clock {
	Uint32 now;
	float delta;
	Uint32 last;
} Clock;

Clock* Clock_Alloc();
void Clock_Tick(Clock* clock);