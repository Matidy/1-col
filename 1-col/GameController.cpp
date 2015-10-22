/////////////////////////////////////////////////////
//				  GAMEPLAY OPTIONS
/////////////////////////////////////////////////////
// 1. Single propagate per turn - propagate when you end your turn.
// 2. Call expandTerritory after evey tile is placed.
// 3. Iterate propagate until all Tiles that can be claim are claimed - propagate at the end of your opponents next turn.
// 4. Expand claiming - Opponent_Owned->NEUTRAL->Your_tile (each -> takes a turn).
/////////////////////////////////////////////////////
//                    ADDITIONS
/////////////////////////////////////////////////////
// display bar at top of the screen and bar at the bottom showing how many tiles each player has left.
// display text on screen
// win state
// On mouse click set a mode of clicking base on above cases. Dragging with mouse down keeps this mode e.g. click on an unowned tile and drag to claim multiple tiles
/////////////////////////////////////////////////////

#include "GameController.h"

GameController::GameController(void) {
	orange_tile_count = 5;
	blue_tile_count = 0;
}

int GameController::init() {
	current_player = ORANGE;
	current_player_tile_count = &orange_tile_count;
	return 1;
}

GameController::~GameController(void) {
}

void GameController::eventClick(Tile* clickedTile, SDL_MouseButtonEvent mouse_event) {
	
	if (mouse_event.button == SDL_BUTTON_LEFT) {
		if (clickedTile->owner == Tile::NEUTRAL && *current_player_tile_count > 0) { // Case: Claim Tile
			switch(current_player) {
				case(ORANGE): clickedTile->owner = Tile::ORANGE_MARKED;
							  break;
				case(BLUE):   clickedTile->owner = Tile::BLUE_MARKED;
							  break;
			}
			*current_player_tile_count -= 1;
			tiles_claimed_this_turn.insert(clickedTile);
		}
	}
	else if (mouse_event.button == SDL_BUTTON_RIGHT) {
		if (tiles_claimed_this_turn.find(clickedTile) != tiles_claimed_this_turn.end()) {
			clickedTile->owner = Tile::NEUTRAL;
			*current_player_tile_count += 1;
			tiles_claimed_this_turn.erase(clickedTile);
		}
	}

	///////////////////////// DEBUG /////////////////////////
	printf("Claimed: {");
	for (auto it=tiles_claimed_this_turn.begin(); it!=tiles_claimed_this_turn.end(); ++it) {
		printf("(%d, %d), ", (**it).index.x, (**it).index.y);
	}
	printf("}\n");
	///////////////////////// DEBUG /////////////////////////
}

void GameController::claimMarkedTiles() {
	for(auto it=tiles_claimed_this_turn.begin(); it!=tiles_claimed_this_turn.end(); ++it) {
		switch(current_player) {
			case(ORANGE): (**it).owner = Tile:: ORANGE;
						  break;
			case(BLUE):	  (**it).owner = Tile:: BLUE;
						  break;
		}
	}
}

void GameController::switchPlayer() {
	switch(current_player) {
		case(ORANGE): current_player = BLUE;
					  current_player_tile_count = &blue_tile_count;
					  break;
		case(BLUE):   current_player = ORANGE;
					  current_player_tile_count = &orange_tile_count;
					  break;
	}
	*current_player_tile_count += 5;
	tiles_claimed_this_turn.clear();
}

// Rewrite this for general case - needed for adding tiles adjacent to those returned from check adjacency to tile_to_check for t+1 expandTerritory call
// Rewrite to remove elements from the fed in array as they are processed.
void GameController::generateTilesToCheck(Board *board, std::set<Tile*> claimed_tiles) {
	// Case: Add tiles surrounding tiles claimed this turn that the current player doesn't already own.
	Tile::Vec2 claimed_tile_index;
	Tile* boardering_tile;
	for (auto it = claimed_tiles.begin(); it != claimed_tiles.end(); ++it) {
		claimed_tile_index = (**it).index;
		for (int i=-1; i<2; ++i) {
			for (int j=-1; j<2; ++j) {
				boardering_tile = board->getTile(claimed_tile_index.x+i, claimed_tile_index.y+j);
				if (boardering_tile->index.x != NULL) {
					if (int(boardering_tile->owner) != int(current_player) && int(boardering_tile->owner) != Tile::UNOWNABLE) {
						tiles_to_check.insert(boardering_tile);
					}
				}
			}
		}
	}
	///////////////////////// DEBUG /////////////////////////
	printf("Tiles To Check: {");
	for (auto it=tiles_to_check.begin(); it!=tiles_to_check.end(); ++it) {
		printf("(%d, %d), ", (**it).index.x, (**it).index.y);
	}
	printf("}\n");
	///////////////////////// DEBUG /////////////////////////
}

// Current expansion rule: tiles which have 5 or more of your tiles boardering them become yours. 
// Broken
std::set<Tile*> GameController::expandTerritory(Board *board) {
	std::set<Tile*> new_surrounded_tiles = checkAdjacency(board, tiles_to_check);
	tiles_claimed_this_turn = new_surrounded_tiles;
	tiles_to_check.clear();	
	generateTilesToCheck(board, tiles_claimed_this_turn);

	///////////////////////// DEBUG /////////////////////////
	printf("{");
	for (auto it=new_surrounded_tiles.begin(); it!=new_surrounded_tiles.end(); ++it) {
		printf("(%d, %d), ", (**it).index.x, (**it).index.y);
	}
	printf("}\n");
	///////////////////////// DEBUG /////////////////////////

	Tile *current_tile;
	for (auto it=new_surrounded_tiles.begin(); it!=new_surrounded_tiles.end(); ++it) {
		current_tile = *it;
		switch(current_player) {
			case(ORANGE): current_tile->owner = Tile::ORANGE;
						  break;
			case(BLUE):	  current_tile->owner = Tile::BLUE;
						  break;
		}
	}

	return new_surrounded_tiles;
}

// Iterates a 3x3 grid over the board checking each Tile's boarding Tiles for
// Condition: Tiles with 3 or more bordering Tiles belonging to the same player are automatically claimed by that player
std::set<Tile*> GameController::checkAdjacency(Board *board, std::set<Tile*> tiles_to_check) {
	// create array with IDs of tiles that have 3 adjacent tiles belonging to the current player
	std::set<Tile*> new_surrounded_tiles; //array to keep track of which Tiles have 3 or more of thier surrounding Tiles owned by a single player.
	Tile* current_tile;
	Tile::Vec2 current_index;
	Tile* boardering_tile;
	int k, n, sum;
	for(auto it=tiles_to_check.begin(); it!=tiles_to_check.end(); ++it) {
		k=-1, n=-1, sum=0;
		current_tile = *it;
		current_index = current_tile->index;
		if(int(current_tile->owner) != int(current_player)) {
			for(int u=0; u<=8; u++) {
				boardering_tile = board->getTile(current_index.x+n, current_index.y+k);
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