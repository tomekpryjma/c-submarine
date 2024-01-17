#pragma once

#define WIN_WIDTH 640
#define WIN_HEIGHT 480
#define RAYS 30
#define FPS_MAX 60.f
#define DELTA_TIME_MAX 0.01666666666f // 1sec / 60

#define RADAR_SIZE_XY 150
#define RADAR_DELAY 2000

#define THINGS_NUM_MAX 10
#define EDGE_SPAWN_BUFFER 10 // 10px at the edge of the playing window so Things can spawn off-screen & not get destroyed immediately.