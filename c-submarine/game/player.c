#include "player.h"
#include <stdio.h>
#include <stdlib.h>

Player* Player_Alloc() {
	Player* player = malloc(sizeof(Player));

	if (player == NULL) {
		printf("Failed to allocate memory for player.");
		exit(1);
	}

	player->worldPosition = (Vector){ 0, 0 };
	//player->localPosition = (Vector){ 0, 0 };
	player->bearing = 0;
	player->direction = (Vector){ 0, 0 };

	return player;
}