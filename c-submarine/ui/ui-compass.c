#include "ui-compass.h"

UiCompass UiCompass_Create(int x, int y, int size) {
	UiCompass ui;
	ui.rect = (SDL_Rect){ x, y, size, size };
	return ui;
}