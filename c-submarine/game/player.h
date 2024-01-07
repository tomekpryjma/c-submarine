#pragma once

#include "../maths/maths.h"

typedef struct Player {
	Vector worldPosition;
	//Vector localPosition; // TODO: probably dont need this
	int bearing;
	Vector direction;
} Player;

Player* Player_Alloc();