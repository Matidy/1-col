#include "tile.h"

Tile::Tile() {
	owner = NEUTRAL;
	width = base_width;
	height = base_height;
}

Tile::~Tile() {

}

void Tile::initPosition(int x, int y) { // initialize a struct Position with x & y.
	pos.x = x;
	pos.y = y;
}
