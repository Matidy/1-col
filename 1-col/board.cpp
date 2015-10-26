#include "board.h"

Board::Board(void) {
	tile_array_size = hori_tiles*vert_tiles;
	board_width = Tile::base_width*hori_tiles;
	board_height = Tile::base_height*vert_tiles;
	claimable_tiles = tile_array_size;
	
	NULL_tile.index.init(0, 0);
	tileInFocus = &NULL_tile;
	prevTileInFocus = &NULL_tile;
}

int Board::init() {
	int x = 0;
	int y = 0;
	Tile current_tile;
	board_tiles.resize(tile_array_size); //needs to be pointers to pass objects rather than values when changing parameters

	////////////////////////////////////////////
	// init tile array with positions etc.
	//////////////////////////////////////
	for(int i=0; i < tile_array_size; i++) { // could look at previous tile width/height when incrementing to handle tiles of different sizes
		current_tile = board_tiles[i];
		if (i%hori_tiles == 0 && i!=0) {
			x = 0;
			y += current_tile.height;
		}
		board_tiles[i].pos.init(x, y);
		board_tiles[i].index.init(i/hori_tiles+1, i%hori_tiles+1);

		x += current_tile.width;

		if (current_tile.owner == Tile::UNOWNABLE) 
			--claimable_tiles;
	}
	return 1;
}

Board::~Board(void) {
}

Tile* Board::getTile(int row, int col) {
	Tile* found_tile;

	if (row < 1 || row > vert_tiles || col < 1 || col > hori_tiles) {
		found_tile = &NULL_tile;
	}
	else {
		int index = getUID(row, col)-1;
		found_tile = &board_tiles[index];
	}
	
	return found_tile;
}

int Board::getUID(Tile tile) {
	Tile::Vec2 tile_index = tile.index;
	return (tile_index.x-1)*hori_tiles+tile_index.y;
}

int Board::getUID(int row, int col) {
	return (row-1)*hori_tiles+col;
}

// Gives the tile the mouse is currently hovering over
Tile* Board::findTileInFocus() {
	std::vector<Tile> temp_tiles = board_tiles; //search method is destructive requiring the board tiles are copied in order to be searched
	Tile *found_tile;
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (x >= board_width-1 || x <= 0 || y >= board_height-1 || y <= 0) {
		found_tile = &NULL_tile;
	}
	else {
		float split_bound; // boundary line divided along
		int tiles_index;
		
		while (temp_tiles.size() > Board::hori_tiles) { // binary divide array horizontally until only one row is left
			split_bound = float((temp_tiles.size()/Board::hori_tiles))/2;
			if ((temp_tiles.size()/Board::hori_tiles)%2 !=0) {
				split_bound -= 0.5; // round down split_bound if odd number of rows
			}

			if (y > (temp_tiles[0].pos.y+(Tile::base_height*split_bound))) {
				tiles_index = Board::hori_tiles*split_bound; // array is 1D contiguous
				temp_tiles.erase(temp_tiles.begin(), temp_tiles.begin()+tiles_index);
			}
			else {
				tiles_index = split_bound*Board::hori_tiles;
				temp_tiles.erase(temp_tiles.begin()+tiles_index, temp_tiles.end());
			}
		}
		while (temp_tiles.size() > 1) { // down to row: binary divide vertically to get a single tile
			split_bound = float(temp_tiles.size())/2;
			if (temp_tiles.size()%2 !=0) {
				split_bound -= 0.5;
			}

			if (x > (temp_tiles[0].pos.x+(Tile::base_width*split_bound))) {
				temp_tiles.erase(temp_tiles.begin(), temp_tiles.begin()+split_bound);
			}
			else {
				temp_tiles.erase(temp_tiles.begin()+split_bound, temp_tiles.end());
			}
		}
		found_tile = getTile(temp_tiles[0].index.x, temp_tiles[0].index.y);
	}
	return found_tile;
}
