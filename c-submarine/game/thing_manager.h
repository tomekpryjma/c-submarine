#pragma once

#include "../globals.h"
#include "thing.h"
#include "../core/clock.h"

/*
* Responsabilities:
* 
* - Initial set up of Things
* - Handle spawning of Things
*	- They should be sapawning somewhere from the edges of the window?
* - Handle destructing of things
*/

typedef struct ThingManager {
	Thing things[THINGS_NUM_MAX];
	unsigned int spawnRateMs;
	unsigned int nextSpawnTime;
} ThingManager;

void ThingManager_Setup(ThingManager* thingManager);
void ThingManager_Update(ThingManager* thingManager, const Clock* clock);
//void ThingManager_SpawnThing();
//void ThingManager_ReuseThing(Thing* thing);
