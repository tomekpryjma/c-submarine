#pragma once

#include "enums/thing_type_enum.h"
#include "../maths/maths.h"
#include <stdbool.h>

typedef struct Thing {
	ThingType type;
	Vector direction;
	Vector position;
	bool isAlive;
} Thing;

Thing* Thing_Spawn(Thing* thing, ThingType type, Vector direction, Vector position);
void Thing_DetermineIsAlive(Thing* thing);