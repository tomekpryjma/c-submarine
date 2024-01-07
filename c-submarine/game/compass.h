#pragma once

#include "../maths/maths.h"

typedef struct Compass {
	Vector direction;
} Compass;

int Compass_GetBearing(Vector otherDirection);
void Compass_ChangeDirection(); // Should be changeable by mouse movement