#include "game-core.h"
#include "maths/maths.h"
#include "core/clock.h"
#include "game/compass.h"

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
}

void GameCore_Print(char* message) {
	printf("%s", message);
}

void GameCore_GameLoop(struct GameCore* gameCore) {
	SDL_Event e;
	bool radarPing;
	bool lastRadarShouldPing = false;
	Uint32 radarRun = gameCore->clock->now + RADAR_DELAY;
	
	Compass compass;
	Compass_Setup(&compass);
	
	gameCore->player->worldPosition = (Vector){300, 220};
	
	Vector linePosition = { 100, WIN_HEIGHT - 100 };
	Vector lineOrigin = { 0, 0 };
	Vector lineEndpoint = { 0, -1 };
	Vector ufo1 = { 380, 190 };
	Vector ufo2 = { 300, 300 };
	Vector ufosInScene[2] = {ufo1, ufo2}; // TODO: set the max as a var
	Vector* radarDetections[2] = {NULL}; // TODO: set the max as a var
	float angleRad = -M_PI_2;

	Vector miniMapPositions[2] = { (Vector) { 0,0 } };
	Vector miniMapPositionsClamped[2] = { (Vector) { 0,0 } };
	Vector gameWorldPositions[2] = { (Vector) { 0,0 } };
	int mouseX, mouseY;
	
	while (gameCore->state->isRunning) {
		Clock_Tick(gameCore->clock);
		bool radarShouldPing = gameCore->clock->now > radarRun;

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				GameState_SetIsRunning(gameCore->state, false);
			}
		}

		SDL_GetMouseState(&mouseX, &mouseY);

		if (radarShouldPing) {
			angleRad += 5 * gameCore->clock->delta;
			Vector rayDir = { cos(angleRad), sin(angleRad)};
			Vector rayEndpoint = { gameCore->player->worldPosition.x, gameCore->player->worldPosition.y };
			Vector detection = {-1, -1};
			lineEndpoint.x = rayDir.x;
			lineEndpoint.y = rayDir.y;
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

					// So far to test two hardcoded UFOs exist
					// Loop through all ufos in scene (2 for now)
					for (int i = 0; i < 2; i++) {
						Vector* ufo = &ufosInScene[i];
						if (
							xLerp > ufo->x && xLerp < ufo->x + 5 && // 5 is hardcoded for now as the width
							yLerp > ufo->y && yLerp < ufo->y + 5
						) { // We have a hit!
							noTargetDetected = false;
							quitLerp = true;
							radarDetections[i] = ufo;
							break;
						}
					}

					if (quitLerp) break;

					t += 0.1f;
				}
			}

			for (int i = 0; i < 2; i++) {
				if (radarDetections[i] == NULL) {
					continue;
				}

				Vector* ufo = radarDetections[i];
				float ufoDx = ufo->x - gameCore->player->worldPosition.x;
				float ufoDy = ufo->y - gameCore->player->worldPosition.y;
				Vector ufoMinimapPosition = {
					(ufoDx - 0) / RADAR_SIZE_XY,
					(ufoDy - 0) / RADAR_SIZE_XY
				};
				Vector ufoMinimapPositionCl = { ufoMinimapPosition.x, ufoMinimapPosition.y };
				if (ufoMinimapPositionCl.x > 1)	ufoMinimapPositionCl.x = 1;
				else if (ufoMinimapPositionCl.x < -1)	ufoMinimapPositionCl.x = -1;
				if (ufoMinimapPositionCl.y > 1)	ufoMinimapPositionCl.x = 1;
				else if (ufoMinimapPositionCl.y < -1)	ufoMinimapPositionCl.y = -1;

				miniMapPositions[i] = (Vector){ ufoMinimapPosition.x, ufoMinimapPosition.y };
				miniMapPositionsClamped[i] = (Vector){ ufoMinimapPositionCl.x, ufoMinimapPositionCl.y };
				gameWorldPositions[i] = (Vector){ ufo->x, ufo->y };
			}
		}

		if (angleRad >= M_2_PI - M_PI_2) {
			angleRad = -M_PI_2;
			radarRun = gameCore->clock->now + RADAR_DELAY;
		}

		SDL_SetRenderDrawColor(gameCore->renderer, 0, 0, 0, 255);
		SDL_RenderClear(gameCore->renderer);

		// Draw radar scan line
		//    - Line should rotate 360deg
		//	  - Delay for 2sec
		if (radarShouldPing) {
			// Draw Clamped UFOs
			SDL_SetRenderDrawColor(gameCore->renderer, 0, 255, 0, 255);

			for (int i = 0; i < 2; i++) {
				Vector ufoMiniMapClamped = miniMapPositionsClamped[i];
				SDL_RenderDrawRect(gameCore->renderer, &(SDL_Rect) {
					linePosition.x + ufoMiniMapClamped.x * RADAR_SIZE_XY / 2,
						linePosition.y + ufoMiniMapClamped.y * RADAR_SIZE_XY / 2,
						5, 5
				});
			}

			SDL_RenderDrawLine(
				gameCore->renderer,
				linePosition.x + lineOrigin.x,
				linePosition.y + lineOrigin.y,
				linePosition.x + lineEndpoint.x * 20,
				linePosition.y + lineEndpoint.y * 20
			);
		}

		// Draw radar container box
		SDL_SetRenderDrawColor(gameCore->renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(gameCore->renderer, &(SDL_Rect) { linePosition.x - RADAR_SIZE_XY / 2, linePosition.y - RADAR_SIZE_XY / 2, RADAR_SIZE_XY, RADAR_SIZE_XY });

		SDL_RenderDrawRect(gameCore->renderer, &compass.ui.rect);

		// TODO: temporary test
		SDL_Point pt = { mouseX, mouseY };
		if (SDL_PointInRect(&pt, &compass.ui)) {
			SDL_PixelFormat pxfmt = { .format = SDL_GetWindowPixelFormat(gameCore->window) };
			SDL_Surface *surf = SDL_CreateRGBSurface(0, compass.ui.rect.w, compass.ui.rect.h, 1, 1, 0, 0, 0);
			SDL_Texture* texture = SDL_CreateTextureFromSurface(gameCore->renderer, surf);
			SDL_RenderCopy(gameCore->renderer, texture, NULL, &compass.ui);
			SDL_FreeSurface(surf);
			SDL_DestroyTexture(texture);
		}

		SDL_RenderPresent(gameCore->renderer);

		if (lastRadarShouldPing && !radarShouldPing) {
			for (int i = 0; i < 2; i++) {
				radarDetections[i] = NULL;
				miniMapPositions[i] = (Vector){ 0, 0 };
				miniMapPositionsClamped[i] = (Vector){ 0, 0 };
				gameWorldPositions[i] = (Vector){ 0, 0 };
			}
		}

		lastRadarShouldPing = radarShouldPing;
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