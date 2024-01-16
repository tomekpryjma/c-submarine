#include "ui-radar.h"

//UiRadar UiRadar_Create() {
//	UiRadar ui;
//	return ui;
//}

void UiRadar_DrawPing(SDL_Renderer* renderer, Vector pings[THINGS_NUM_MAX], Vector lineParts[3]) {
	Vector lineOrigin = lineParts[0];
	Vector linePosition = lineParts[1];
	Vector lineEndpoint = lineParts[2];
	
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

	for (int i = 0; i < THINGS_NUM_MAX; i++) {
		Vector clamped = pings[i];
		SDL_RenderDrawRect(renderer, &(SDL_Rect) {
			linePosition.x + clamped.x * RADAR_SIZE_XY / 2,
			linePosition.y + clamped.y * RADAR_SIZE_XY / 2,
			5, 5
		});
	}

	SDL_RenderDrawLine(
		renderer,
		linePosition.x + lineOrigin.x,
		linePosition.y + lineOrigin.y,
		linePosition.x + lineEndpoint.x * 20,
		linePosition.y + lineEndpoint.y * 20
	);
}

void UiRadar_DrawContainer(SDL_Renderer* renderer, Vector linePosition) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &(SDL_Rect) {
		linePosition.x - RADAR_SIZE_XY / 2,
		linePosition.y - RADAR_SIZE_XY / 2,
		RADAR_SIZE_XY, RADAR_SIZE_XY }
	);
}