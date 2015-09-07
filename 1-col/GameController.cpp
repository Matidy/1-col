// display text on screen
// win state
//On mouse click set a mode of clicking base on above cases. Dragging with mouse down keeps this mode e.g. click on an unowned tile and drag to claim multiple tiles

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
	//#################################################################################################
	Tile * mem_tile = &board->board_tiles[board->tileInFocus.ID-1];
	//#################################################################################################

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
	//#################################################################################################
	board->tileInFocus = *mem_tile; // better to use pointers
	board->prevTileInFocus = *mem_tile;
	//#################################################################################################
}

void GameController::endTurn(Board *board) {
	//#################################################################################################
	Tile * mem_tile = &board->board_tiles[board->tileInFocus.ID-1];
	//#################################################################################################

	std::vector<int> surrounded_tiles_ids = checkAdjacency(board);

	printf("{");
	for (int i=0; i<surrounded_tiles_ids.size(); i++) {
		printf("%d, ", surrounded_tiles_ids[i]);
	}
	printf("}\n");
	int current_id;
	for (int i=0; i<surrounded_tiles_ids.size(); i++) {
		current_id = surrounded_tiles_ids[i];
		switch(current_player) {
			case(YELLOW): board->board_tiles[current_id-1].owner = Tile::YELLOW;
						  break;
			case(BLUE):	  board->board_tiles[current_id-1].owner = Tile::BLUE;
						  break;
		}
		
	}

	switch(current_player) {
		case(YELLOW): current_player = BLUE;
					  break;
		case(BLUE):   current_player = YELLOW;
					  break;
	}
	tile_count = 5;

	//#################################################################################################
	board->tileInFocus = *mem_tile; // better to use pointers
	board->prevTileInFocus = *mem_tile;
	//#################################################################################################
}

// Iterates a 3x3 grid over the board checking each Tile's boarding Tiles for
// Condition: Tiles with 3 or more bordering Tiles belonging to the same player are automatically claimed by that player
std::vector<int> GameController::checkAdjacency(Board *board) {
	// create array with IDs of tiles that have 3 adjacent tiles belonging to the current player
	std::vector<int> surrounded_tiles_ids; //array to keep track of which Tiles have 3 or more of thier surrounding Tiles owned by a single player.
	Tile current_tile;
	int k, n, sum;
	for(int i=0; i<board->board_tiles.size(); i++) {
		k=-1, n=-1, sum=0;
		current_tile = board->board_tiles[i];
		if(int(current_tile.owner) != int(current_player)) {
			for(int u=0; u<=8; u++) {
				if(int(board->getTile(i/Board::hori_tiles+n, i%Board::hori_tiles+k).owner) == int(current_player))
					sum += 1;// each tile that the current player owns bordering a tile adds 1 to sum
				if(sum >=4) {
					surrounded_tiles_ids.push_back(current_tile.ID);
					break;
				}
				k += 1;
				if (k>1) {
					k = -1;
					n += 1;
				}
			}
		}
	}
	return surrounded_tiles_ids;
}