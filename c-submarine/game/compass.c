#include "compass.h"

int Compass_GetBearing(Vector otherDirection) {
	float radians = atan2f(otherDirection.y, otherDirection.x) + M_PI_2;
	return radians * RAD_TO_DEG;
}