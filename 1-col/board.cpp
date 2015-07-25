#include "board.h"

Board::Board(void) {
	hori_tiles = 10;
	vert_tiles = 8;
}

Board::~Board(void) {
}

Tile Board::findMouseTile() {
	Tile tile;
	return tile;
}

int Board::init() {
	int array_size = hori_tiles*vert_tiles;
	tiles_array = new Tile [array_size]; //remember to deallocate when each game ends/board reset
	return 1;
}

Tile Board::getTile(int row, int col) {
	return tiles_array[row*hori_tiles+col];
}
