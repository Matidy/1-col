#include "board.h"

Board::Board(void) {
	tile_array_size = hori_tiles*vert_tiles;
	
	Tile NULL_tile;
	NULL_tile.ID = NULL;
	tileInFocus = NULL_tile;
	prevTileInFocus = NULL_tile;
}

int Board::init() {

	int x = 0;
	int y = 0;
	Tile current_tile;
	board_tiles.resize(tile_array_size);

	////////////////////////////////////////////
	// init tile array with positions etc.
	//////////////////////////////////////
	for(int i=0; i < tile_array_size; i++) { // could look at previous tile width/height when incrementing to handle tiles of different sizes
		current_tile = board_tiles[i];
		if (i%hori_tiles == 0 && i!=0) {
			x = 0;
			y += current_tile.height;
		}
		board_tiles[i].initPosition(x, y);
		board_tiles[i].ID = i; // Set unique ID by loop counter
		printf("Iteration: %d\nAfter: x=%d, y=%d\n", i, board_tiles[i].pos.x, board_tiles[i].pos.y);

		x += current_tile.width;
	}

	return 1;
}

Board::~Board(void) {
}

Tile Board::getTile(int row, int col) {
	return board_tiles[row*hori_tiles+col];
}

// Gives the tile the mouse is currently hovering over
Tile Board::findTileInFocus(std::vector<Tile> board_tiles) {
	float split_bound; // boundary line divided along
	int x, y;
	SDL_GetMouseState(&x, &y);
	// maintain array of considered elements
	while (board_tiles.size() > Board::hori_tiles) { // binary divide array horizontally until only one row is left
		float split_bound = float((board_tiles.size()/Board::hori_tiles))/2;
		if (Board::vert_tiles%2 !=0) {
			split_bound -= 0.5;
		}

		if (y > (Tile::base_height*split_bound)) {
			board_tiles.erase(board_tiles.begin(), board_tiles.begin()+(Board::hori_tiles*(split_bound-1)));
		}
		else {
			board_tiles.erase(board_tiles.begin()+(split_bound*Board::hori_tiles), board_tiles.end());
		}
	}
	while (board_tiles.size() > 1) { // down to row: binary divide vertically to get a single tile
		float split_bound = float(board_tiles.size())/2;
		if (Board::vert_tiles%2 !=0) {
			split_bound -= 0.5;
		}
		if (x > (Tile::base_width*split_bound)) {
			board_tiles.erase(board_tiles.begin(), board_tiles.begin()+(split_bound-1));
		}
		else {
			board_tiles.erase(board_tiles.begin()+split_bound, board_tiles.end());
		}
	}

	return board_tiles[0];
} // set currenttileinfocus and previous. Compare to see if the tile has changed and if so call an update highlight method
