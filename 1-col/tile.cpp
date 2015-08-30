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

ValRGBA Tile::getColour() {
	ValRGBA RGBA;
	switch(owner) {
	case Tile::YELLOW:		  RGBA.r = 0xFF;
							  RGBA.g = 0xAD;
							  RGBA.b = 0x33;
							  RGBA.a = 0xFF;
							  break;
		case Tile::BLUE:	  RGBA.r = 0x33;
							  RGBA.g = 0x85;
							  RGBA.b = 0xFF;
							  RGBA.a = 0xFF;
							  break;
		case Tile::NEUTRAL:   RGBA.r = 0x52;
							  RGBA.g = 0x52;
							  RGBA.b = 0x52;
							  RGBA.a = 0xFF;
							  break;
		case Tile::UNOWNABLE: RGBA.r = 0x33;
							  RGBA.g = 0x33;
							  RGBA.b = 0x33;
							  RGBA.a = 0xFF;
							  break;
	}
	return RGBA;
}
