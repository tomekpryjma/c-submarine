#include "compass.h"

int Compass_GetBearing(Vector otherDirection) {
	float radians = atan2f(otherDirection.y, otherDirection.x) + M_PI_2;
	return radians * RAD_TO_DEG;
}

void Compass_Setup(Compass* compass) {
	compass->ui = UiCompass_Create(50, 50, 100);
}