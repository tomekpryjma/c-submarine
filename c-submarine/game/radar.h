#pragma once

#include <stdbool.h>
#include "../core/clock.h"
#include "player.h"
#include "thing_manager.h"
#include "../ui/ui-radar.h"

typedef struct Radar {
	bool shouldPing;
	bool lastShouldPing;
	float angleRad;
	Vector lineOrigin;
	Vector linePosition;
	Vector lineEndpoint;
	Vector* detections[THINGS_NUM_MAX];
	Vector detectionPositions[THINGS_NUM_MAX];
	Vector detectionPositionsClamped[THINGS_NUM_MAX];
	unsigned int pingWhen;
	UiRadar ui;
} Radar;

void Radar_Setup(Radar* radar, unsigned int now);
void Radar_Ping(Radar* radar, const Clock* clock, const Player* player, const ThingManager* manager);
void Radar_Before(Radar* radar, unsigned int now);
void Radar_After(Radar* radar, unsigned int now);

// These functions should only be used like private functions.
void _reset_detections(Radar* radar);