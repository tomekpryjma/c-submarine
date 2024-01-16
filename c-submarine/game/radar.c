#include "radar.h"
#include "../maths/maths.h"
#include "../globals.h"

void Radar_Setup(Radar* radar, unsigned int now) {
	radar->linePosition = (Vector){100, WIN_HEIGHT - 100};
	radar->lineOrigin = (Vector){ 0, 0 };
	radar->lineEndpoint = (Vector){ 0, -1 };
	radar->angleRad = -M_PI_2;
	radar->pingWhen = now + RADAR_DELAY;

	for (int i = 0; i < THINGS_NUM_MAX; i++) {
		radar->detectionPositions[i] = (Vector){ 0,0 };
		radar->detectionPositionsClamped[i] = (Vector){ 0,0 };
	}
}

void Radar_Ping(Radar* radar, const Clock* clock, const Player* player, const ThingManager* manager) {
	radar->angleRad += 5 * clock->delta;
	Vector rayDir = { cos(radar->angleRad), sin(radar->angleRad) };
	Vector rayEndpoint = { player->worldPosition.x, player->worldPosition.y };
	Vector detection = { -1, -1 };
	radar->lineEndpoint.x = rayDir.x;
	radar->lineEndpoint.y = rayDir.y;
	bool noTargetDetected = true;
	bool rayEndpointInBounds = true;

	while (rayEndpointInBounds && noTargetDetected) {
		rayEndpoint.x += rayDir.x;
		rayEndpoint.y += rayDir.y;

		// Check OOB
		if (rayEndpoint.x < 0 || rayEndpoint.y > WIN_WIDTH) {
			rayEndpointInBounds = false;
		}
		else if (rayEndpoint.y < 0 || rayEndpoint.y > WIN_HEIGHT) {
			rayEndpointInBounds = false;
		}

		// Do collision detection by interpolation to prevent tunnel problem.
		float t = 0;
		bool quitLerp = false;
		while (t < 1) {
			float xLerp = M_Lerp(rayEndpoint.x - rayDir.x, rayEndpoint.x, t);
			float yLerp = M_Lerp(rayEndpoint.y - rayDir.y, rayEndpoint.y, t);

			for (int i = 0; i < THINGS_NUM_MAX; i++) {
				Vector* ufo = &manager->things[i];
				if (
					xLerp > ufo->x && xLerp < ufo->x + 5 && // 5 is hardcoded for now as the width
					yLerp > ufo->y && yLerp < ufo->y + 5
					) { // We have a hit!
					noTargetDetected = false;
					quitLerp = true;
					radar->detections[i] = ufo;
					break;
				}
			}

			if (quitLerp) break;

			t += 0.1f;
		}
	}

	for (int i = 0; i < THINGS_NUM_MAX; i++) {
		if (radar->detections[i] == NULL) {
			continue;
		}

		// TODO: The below can be tidied up.
		Vector* ufo = radar->detections[i];
		float ufoDx = ufo->x - player->worldPosition.x;
		float ufoDy = ufo->y - player->worldPosition.y;
		Vector ufoMinimapPosition = {
			(ufoDx - 0) / RADAR_SIZE_XY,
			(ufoDy - 0) / RADAR_SIZE_XY
		};
		Vector ufoMinimapPositionCl = { ufoMinimapPosition.x, ufoMinimapPosition.y };
		if (ufoMinimapPositionCl.x > 1)	ufoMinimapPositionCl.x = 1;
		else if (ufoMinimapPositionCl.x < -1)	ufoMinimapPositionCl.x = -1;
		if (ufoMinimapPositionCl.y > 1)	ufoMinimapPositionCl.x = 1;
		else if (ufoMinimapPositionCl.y < -1)	ufoMinimapPositionCl.y = -1;

		radar->detectionPositions[i] = (Vector){ ufoMinimapPosition.x, ufoMinimapPosition.y };
		radar->detectionPositionsClamped[i] = (Vector){ ufoMinimapPositionCl.x, ufoMinimapPositionCl.y };
	}
}

void Radar_Before(Radar* radar, unsigned int now) {
	_reset_detections(radar);
	radar->shouldPing = now > radar->pingWhen;
}

void Radar_After(Radar* radar, unsigned int now) {
	if (radar->angleRad >= M_2_PI - M_PI_2) {
		radar->angleRad = -M_PI_2;
		radar->pingWhen = now + RADAR_DELAY;
	}
}

void _reset_detections(Radar* radar) {
	if (radar->lastShouldPing && !radar->shouldPing) {
		for (int i = 0; i < THINGS_NUM_MAX; i++) {
			radar->detections[i] = NULL;
			radar->detectionPositions[i] = (Vector){ 0, 0 };
			radar->detectionPositionsClamped[i] = (Vector){ 0, 0 };
		}
	}
}