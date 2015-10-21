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

void GameController::eventClick(Tile* clickedTile, SDL_MouseButtonEvent mouse_event) {
	
	if (mouse_event.button == SDL_BUTTON_LEFT) {
		if (clickedTile->owner == Tile::NEUTRAL && tile_count > 0) { // Case: Claim Tile
			switch(current_player) {
				case(YELLOW): clickedTile->owner = Tile::YELLOW;
							  break;
				case(BLUE):   clickedTile->owner = Tile::BLUE;
							  break;
			}
			tile_count -= 1;
			if (tiles_unclaimed_this_turn.find(clickedTile) == tiles_unclaimed_this_turn.end()) {
				tiles_claimed_this_turn.insert(clickedTile);
			}
			else {
				tiles_unclaimed_this_turn.erase(clickedTile);
			}
		}
	}
	else if (mouse_event.button == SDL_BUTTON_RIGHT) {
		if (int(clickedTile->owner) == int(current_player)) {
			clickedTile->owner = Tile::NEUTRAL;
			tile_count += 1;
			if (tiles_claimed_this_turn.find(clickedTile) == tiles_claimed_this_turn.end()) {
				tiles_unclaimed_this_turn.insert(clickedTile);
			}
			else {
				tiles_claimed_this_turn.erase(clickedTile);
			}
		}
	}
	///////////////////////// DEBUG /////////////////////////
	printf("Claimed: {");
	for (auto it=tiles_claimed_this_turn.begin(); it!=tiles_claimed_this_turn.end(); ++it) {
		printf("%d, ", (**it).ID);
	}
	printf("}\n");

	printf("Unclaimed: {");
	for (auto it=tiles_unclaimed_this_turn.begin(); it!=tiles_unclaimed_this_turn.end(); ++it) {
		printf("%d, ", (**it).ID);
	}
	printf("}\n");
	///////////////////////// DEBUG /////////////////////////
}

void GameController::switchPlayer() {
	switch(current_player) {
		case(YELLOW): current_player = BLUE;
					  break;
		case(BLUE):   current_player = YELLOW;
					  break;
	}
	tile_count = 5;
	tiles_claimed_this_turn.clear();
	tiles_unclaimed_this_turn.clear();
}

// Test this.
// Rewrite checkAdjacenecy to iterate through tiles_to_check rather than all of board.
// Rewrite this for general case - needed for adding tiles adjacent to those returned from check adjacency to tile_to_check for t+1 expandTerritory call
// Store tile's index with tile object on creation.
void GameController::generateTilesToCheck(Board *board) {
	// Case 1: Add unclaimed tiles to tiles_to_check
	tiles_to_check = tiles_unclaimed_this_turn;

	// Case 2: Add tiles surrounding tiles claimed this turn that the current player doesn't already own.
	int claimed_tile_index;
	Tile* boardering_tile;
	for (auto it = tiles_claimed_this_turn.begin(); it != tiles_claimed_this_turn.end(); ++it) {
		claimed_tile_index = (**it).ID-1;
		for (int i=-1; i<2; ++i) {
			for (int j=-1; j<2; ++j) {
				boardering_tile = board->getTile(claimed_tile_index/Board::hori_tiles+i, claimed_tile_index%Board::hori_tiles+j);
				if (boardering_tile->ID != NULL) {
					if (int(boardering_tile->owner) != int(current_player) && int(boardering_tile->owner) != Tile::Owner::UNOWNABLE) {
						tiles_to_check.insert(boardering_tile);
					}
				}
			}
		}
	}
	///////////////////////// DEBUG /////////////////////////
	printf("Tiles To Check: {");
	for (auto it=tiles_to_check.begin(); it!=tiles_to_check.end(); ++it) {
		printf("%d, ", (**it).ID);
	}
	printf("}\n");
	///////////////////////// DEBUG /////////////////////////
}

// Current expansion rule: tiles which have 4 or more of your tiles boardering them become yours. 
std::set<Tile*> GameController::expandTerritory(Board *board) {
	std::set<Tile*> new_surrounded_tiles= checkAdjacency(board);
	tiles_claimed_this_turn = new_surrounded_tiles;
	tiles_unclaimed_this_turn.clear();
	tiles_to_check.clear();
	generateTilesToCheck(board);

	///////////////////////// DEBUG /////////////////////////
	printf("{");
	for (auto it=new_surrounded_tiles.begin(); it!=new_surrounded_tiles.end(); ++it) {
		printf("%d, ", (**it).ID);
	}
	printf("}\n");
	///////////////////////// DEBUG /////////////////////////

	Tile *current_tile;
	for (auto it=new_surrounded_tiles.begin(); it!=new_surrounded_tiles.end(); ++it) {
		current_tile = *it;
		switch(current_player) {
			case(YELLOW): current_tile->owner = Tile::YELLOW;
						  break;
			case(BLUE):	  current_tile->owner = Tile::BLUE;
						  break;
		}
	}

	return new_surrounded_tiles;
}

// Iterates a 3x3 grid over the board checking each Tile's boarding Tiles for
// Condition: Tiles with 3 or more bordering Tiles belonging to the same player are automatically claimed by that player
std::set<Tile*> GameController::checkAdjacency(Board *board) {
	// create array with IDs of tiles that have 3 adjacent tiles belonging to the current player
	std::set<Tile*> new_surrounded_tiles; //array to keep track of which Tiles have 3 or more of thier surrounding Tiles owned by a single player.
	Tile* current_tile;
	int current_index;
	Tile* boardering_tile;
	int k, n, sum;
	for(auto it=tiles_to_check.begin(); it!=tiles_to_check.end(); ++it) {
		k=-1, n=-1, sum=0;
		current_tile = *it;
		current_index = current_tile->ID-1;
		if(int(current_tile->owner) != int(current_player)) {
			for(int u=0; u<=8; u++) {
				boardering_tile = board->getTile(current_index/Board::hori_tiles+n, current_index%Board::hori_tiles+k);
				if(int(boardering_tile->owner) == int(current_player))
					sum += 1; //each tile that the current player owns bordering a tile adds 1 to sum
				if(sum >= TILES_NEEDED_TO_CLAIM) {
					new_surrounded_tiles.insert(current_tile);
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
	return new_surrounded_tiles;
}