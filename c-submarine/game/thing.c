#include "thing.h"
#include "../globals.h"

Thing* Thing_Spawn(Thing* thing, ThingType type, Vector direction, Vector position) {
	thing->type = type;
	thing->direction = direction;
	thing->position = position;
	thing->isAlive = true;
	return thing;
}

void Thing_DetermineIsAlive(Thing* thing) {
	// Check OOB
	if (thing->position.x < -EDGE_SPAWN_BUFFER ||
		thing->position.x > WIN_WIDTH + EDGE_SPAWN_BUFFER ||
		thing->position.y < -EDGE_SPAWN_BUFFER ||
		thing->position.y > WIN_HEIGHT + EDGE_SPAWN_BUFFER
	) {
		thing->isAlive = false;
	}
}