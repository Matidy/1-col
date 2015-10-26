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
// -display bar at top of the screen and bar at the bottom showing how many tiles each player has left.
// -display text on screen
// -On mouse click set bool. On mouse release unset bool. While bool is set,
//  trigger clickEvent(left_mouse) every time board->tileInFocus changes. i.e. implementation of click and drag to claim multiple tiles
/////////////////////////////////////////////////////

#include "GameController.h"

GameController::GameController(void) {
	orange_tile_count = 5;
	blue_tile_count = 0;
	claimed_by_orange = 0;
	claimed_by_blue = 0;
}

int GameController::init(Board board) {
	phase = TILE_CLAIM;
	current_player = ORANGE;
	current_player_tile_count = &orange_tile_count;
	current_player_prev_claimed = &orange_prev_claimed;
	unclaimed_tiles = board.claimable_tiles;
	return 1;
}

GameController::~GameController(void) {
}

void GameController::reset() {
	unclaimed_tiles += claimed_by_orange + claimed_by_blue;
	claimed_by_orange = 0;
	claimed_by_blue = 0;
	orange_tile_count = 5;
	blue_tile_count = 0;
	current_player = ORANGE;
	current_player_tile_count = &orange_tile_count;
	orange_prev_claimed.clear();
	blue_prev_claimed.clear();
	current_player_prev_claimed = &orange_prev_claimed;
}

void GameController::eventClick(Tile* clickedTile, SDL_MouseButtonEvent mouse_event) {
	
	if (mouse_event.button == SDL_BUTTON_LEFT) {
		if (clickedTile->owner == Tile::NEUTRAL && *current_player_tile_count > 0) {
			switch(current_player) {
				case(ORANGE): clickedTile->owner = Tile::ORANGE_MARKED;
							  break;
				case(BLUE):   clickedTile->owner = Tile::BLUE_MARKED;
							  break;
			}
			*current_player_tile_count -= 1;
			current_player_prev_claimed->insert(clickedTile);
		}
	}
	else if (mouse_event.button == SDL_BUTTON_RIGHT) {
		if (current_player_prev_claimed->find(clickedTile) != current_player_prev_claimed->end()) {
			clickedTile->owner = Tile::NEUTRAL;
			*current_player_tile_count += 1;
			current_player_prev_claimed->erase(clickedTile);
		}
	}

	///////////////////////// DEBUG /////////////////////////
	printf("Claimed: {");
	for (auto it=current_player_prev_claimed->begin(); it!=current_player_prev_claimed->end(); ++it) {
		printf("(%d, %d), ", (**it).index.x, (**it).index.y);
	}
	printf("}\n");
	///////////////////////// DEBUG /////////////////////////
}

void GameController::claimMarkedTiles() {
	for(auto it=current_player_prev_claimed->begin(); it!=current_player_prev_claimed->end(); ++it) {
		claimTile(current_player, *it);
	}
}

void GameController::switchPlayer() {
	switch(current_player) {
		case(ORANGE): current_player = BLUE;
					  current_player_tile_count = &blue_tile_count;
					  current_player_prev_claimed = &blue_prev_claimed;
					  break;
		case(BLUE):   current_player = ORANGE;
					  current_player_tile_count = &orange_tile_count;
					  current_player_prev_claimed = &orange_prev_claimed;
					  break;
	}
	*current_player_tile_count += 5;
}

void GameController::claimTile(Player current_player, Tile* tile_to_claim) {
	bool already_owned = false;
	if (int(tile_to_claim->owner) == int(getOtherPlayer(current_player))) {
		already_owned = true;
	}
		
	switch(current_player) {
	case ORANGE: tile_to_claim->owner = Tile::ORANGE;
				 ++claimed_by_orange;
				 if (already_owned) --claimed_by_blue;
				 break;
	case BLUE:   tile_to_claim->owner = Tile::BLUE;
				 ++claimed_by_blue;
				 if (already_owned) --claimed_by_orange;
				 break;
	}
	
	if(!already_owned) --unclaimed_tiles;
		
	printf("\nPhase: %d\n\n", phase);

	///////////////////////// DEBUG /////////////////////////
	printf("Unclaimed: %d\nOrange: %d\nBlue: %d\n", unclaimed_tiles, claimed_by_orange, claimed_by_blue);
	///////////////////////// DEBUG /////////////////////////
}

GameController::Player GameController::getOtherPlayer (Player player) {
	Player other_player;
	switch(player) {
		case ORANGE: other_player = BLUE;
					 break;
		case BLUE:   other_player = ORANGE;
					 break;
	}
	
	return other_player;
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
// TO-DO: MAKE checkAdjacenecy and generateTilesToCheck destructive.
std::set<Tile*> GameController::expandTerritory(Board *board) {
	std::set<Tile*> new_surrounded_tiles = checkAdjacency(board, tiles_to_check);
	tiles_to_check.clear();	
	generateTilesToCheck(board, new_surrounded_tiles);

	///////////////////////// DEBUG /////////////////////////
	printf("{");
	for (auto it=new_surrounded_tiles.begin(); it!=new_surrounded_tiles.end(); ++it) {
		printf("(%d, %d), ", (**it).index.x, (**it).index.y);
	}
	printf("}\n");
	///////////////////////// DEBUG /////////////////////////

	for (auto it=new_surrounded_tiles.begin(); it!=new_surrounded_tiles.end(); ++it) {
		claimTile(current_player, *it);
	}

	return new_surrounded_tiles;
}

// Iterates a 3x3 grid over the board checking each Tile's boardering Tiles.
// Condition: Tiles with 5 or more bordering Tiles belonging to the same player are automatically claimed by that player.
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