#pragma once

#include "../maths/maths.h"
#include "../ui/ui-compass.h"

typedef struct Compass {
	Vector direction;
	UiCompass ui;
} Compass;

int Compass_GetBearing(Vector otherDirection);
void Compass_Setup(Compass* compass);
void Compass_ChangeDirection(); // Should be changeable by mouse movement