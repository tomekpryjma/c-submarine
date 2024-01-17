#include "game-core.h"
#include "maths/maths.h"
#include "core/clock.h"
#include "game/compass.h"
#include "game/radar.h"
#include "ui/ui-radar.h"
#include "game/thing_manager.h"
#include <stdlib.h>
#include <time.h>

struct GameCore* GameCore_Alloc() {
	struct GameCore* gameCore = malloc(sizeof(struct GameCore));

	if (gameCore == NULL) {
		printf("Could not allocate memory for game core. Goodbye.");
		exit(1);
	}

	return gameCore;
}

void GameCore_Init(struct GameCore* gameCore) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		GameCore_Print(SDL_GetError());
	}

	if (SDL_CreateWindowAndRenderer(WIN_WIDTH, WIN_HEIGHT, 0, &gameCore->window, &gameCore->renderer) != 0) {
		printf("%s", SDL_GetError());
		GameCore_TearDown(gameCore);
	}

	gameCore->surface = SDL_GetWindowSurface(gameCore->window);

	if (gameCore->surface == NULL) {
		printf("%s", SDL_GetError());
		GameCore_TearDown(gameCore);
	}

	gameCore->state = GameState_Alloc();
	gameCore->clock = Clock_Alloc();
	gameCore->player = Player_Alloc();

	srand(time(NULL));
}

void GameCore_Print(char* message) {
	printf("%s", message);
}

void GameCore_GameLoop(struct GameCore* gameCore) {
	SDL_Event e;
	Compass compass;
	Radar radar;
	ThingManager thingManager;
	int mouseX, mouseY;

	Compass_Setup(&compass);
	Radar_Setup(&radar, gameCore->clock->now);
	ThingManager_Setup(&thingManager);
	
	gameCore->player->worldPosition = (Vector){300, 220};
	
	while (gameCore->state->isRunning) {
		Clock_Tick(gameCore->clock);
		Radar_Before(&radar, gameCore->clock->now);

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				GameState_SetIsRunning(gameCore->state, false);
			}
		}

		SDL_GetMouseState(&mouseX, &mouseY);
		ThingManager_Update(&thingManager, gameCore->clock);

		if (radar.shouldPing) {
			Radar_Ping(&radar, gameCore->clock, gameCore->player, &thingManager);
			Radar_After(&radar, gameCore->clock->now);
		}

		// 
		// Draw
		//
		SDL_SetRenderDrawColor(gameCore->renderer, 0, 0, 0, 255);
		SDL_RenderClear(gameCore->renderer);

		if (radar.shouldPing) {
			Vector lineParts[] = { radar.lineOrigin, radar.linePosition, radar.lineEndpoint };
			UiRadar_DrawPing(gameCore->renderer, radar.detectionPositionsClamped, lineParts);
		}
		
		UiRadar_DrawContainer(gameCore->renderer, radar.linePosition);
		SDL_RenderDrawRect(gameCore->renderer, &compass.ui.rect);

		// TODO: temporary test
		SDL_Point pt = { mouseX, mouseY };
		if (SDL_PointInRect(&pt, &compass.ui)) {
			SDL_SetRenderDrawColor(gameCore->renderer, 255, 0, 0, 255);
			SDL_RenderFillRect(gameCore->renderer, &compass.ui.rect);
		}

		SDL_RenderPresent(gameCore->renderer);
	}
}

void GameCore_TearDown(struct GameCore* gameCore) {
	SDL_DestroyRenderer(gameCore->renderer);
	SDL_FreeSurface(gameCore->surface);
	SDL_DestroyWindow(gameCore->window);
	free(gameCore->state);
	free(gameCore->clock);
	free(gameCore->player);

	// Finish up here
	free(gameCore);
	exit(0);
}