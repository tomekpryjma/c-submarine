#include "thing_manager.h"
#include "../globals.h"
#include "../maths/maths.h"
#include <stdlib.h>

void ThingManager_Setup(ThingManager* thingManager) {
	thingManager->spawnRateMs = 2000;
	thingManager->nextSpawnTime = 0;

	for (int i = 0; i < THINGS_NUM_MAX; i++) {
		thingManager->things[i] = (Thing){
			.direction = (Vector) {0,0},
			.position = (Vector) {0,0},
			.type = DEBRIS
		};
	}
}
void ThingManager_Update(ThingManager* thingManager, const Clock* clock) {
	for (int i = 0; i < THINGS_NUM_MAX; i++) {
		Thing thing = thingManager->things[i];

		if (thing.isAlive) {
			Thing_DetermineIsAlive(&thing);
		}
		else if (!thing.isAlive && clock->now > thingManager->nextSpawnTime) {
			// TODO Pick random position along random edge
			/*Vector thingPosition = {
				.x = (rand() % 2),
				.y = (rand() % 2),
			};*/

			// Pick random direction 
			Thing_Spawn(&thing, ENEMY, (Vector) { 0, 0 }, (Vector) { 0, 0 });
			thingManager->nextSpawnTime = clock->now + thingManager->spawnRateMs;
		}
	}
	//if (clock->now > thingManager->nextSpawnTime) {

	//}
}
//void ThingManager_SpawnThing();
//void ThingManager_ReuseThing(Thing* thing);