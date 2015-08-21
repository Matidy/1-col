#include "board.h"

Board::Board(void) {
	hori_tiles = 10;
	vert_tiles = 8;
	tile_array_size = hori_tiles*vert_tiles; 
}

int Board::init() {
	//tiles_array = new Tile [tile_array_size]; // remember to deallocate when each game ends/board reset

	int x = 0;
	int y = 0;
	Tile current_tile;
	board_tiles.resize(tile_array_size);

	////////////////////////////////////////////
	// init tile array with positions etc.
	//////////////////////////////////////
	for(int i=0; i <tile_array_size; i++) { // could look at previous tile width/height when incrementing to handle tiles of different sizes
		current_tile = board_tiles[i];
		board_tiles[i].initPosition(x, y);
		printf("After: x=%d, y=%d\n", current_tile.pos.x, current_tile.pos.y);
		x += current_tile.width;
		if (i%hori_tiles == 0 && i!=0) {
			x = 0;
			y += current_tile.height;
		}
	}

	return 1;
}

Board::~Board(void) {
}

Tile Board::getTile(int row, int col) {
	return board_tiles[row*hori_tiles+col];
}
