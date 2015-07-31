#include "tile.h"

Tile::Tile() {
	Owner owner = NEUTRAL;
	width = 64;
	height = 64;
}

Tile::~Tile() {

}

void Tile::initPosition(int x, int y) { // initialize a struct Position with x & y.
	pos->x = x;
	pos->y = y;
}