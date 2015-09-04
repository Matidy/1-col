#include "GameController.h"


GameController::GameController(void) {
	tile_count = 5; // case: player ends turn without placing all tiles
	/*SDL_Rect temp_rect = {0, 0, 0, 0}; // set dimensions on end turn button (not currently used)
	end_turn_button = temp_rect;*/
}

int GameController::init() {
	current_player = YELLOW;
	return 1;
}

GameController::~GameController(void) {
}

void GameController::eventClick(Board *board, SDL_MouseButtonEvent mouse_event) {
	Tile * mem_tile = &board->board_tiles[board->tileInFocus.ID-1];
	if (mouse_event.button == SDL_BUTTON_LEFT) {
		if (mem_tile->owner == Tile::NEUTRAL && tile_count > 0) { // Case: Claim Tile
			switch(current_player) {
				case(YELLOW): mem_tile->owner = Tile::YELLOW;
											  break;
				case(BLUE):   mem_tile->owner = Tile::BLUE;
											  break;
			}
			tile_count -= 1;
		}
	}
	else if (mouse_event.button == SDL_BUTTON_RIGHT) {
		printf("tile_owner:%d, current_player:%d\n", mem_tile->owner, current_player);
		if (int(mem_tile->owner) == int(current_player)) {
			mem_tile->owner = Tile::NEUTRAL;
			tile_count += 1;
		}
	}
	board->tileInFocus = *mem_tile; // better to use pointers
	board->prevTileInFocus = *mem_tile;

	// right-click
	// display text on screen
	// adjacency update
	// win state

	//On mouse click set a mode of clicking base on above cases. Dragging with mouse down keeps this mode e.g. click on an unowned tile and drag to claim multiple tiles
}

void GameController::endTurn() {
	switch(current_player) {
		case(YELLOW): current_player = BLUE;
					  break;
		case(BLUE): current_player = YELLOW;
					  break;
	}
	tile_count = 5;
}