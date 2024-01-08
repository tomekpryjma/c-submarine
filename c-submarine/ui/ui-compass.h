#pragma once

#include <SDL.h>

typedef struct UiCompass {
	SDL_Rect rect;
} UiCompass;

UiCompass UiCompass_Create(int x, int y, int size);