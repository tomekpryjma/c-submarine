#pragma once

#include <SDL.h>
#include "../maths/maths.h"
#include "../globals.h"

typedef struct UiRadar {
	SDL_Rect rect;
} UiRadar;

UiRadar UiRadar_Create();
void UiRadar_DrawPing(SDL_Renderer* renderer, Vector pings[THINGS_NUM_MAX], Vector lineParts[3]);
void UiRadar_DrawContainer(SDL_Renderer* renderer, Vector linePosition);