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
	case Tile::YELLOW:		  RGBA.r = 0xDB;
							  RGBA.g = 0x31;
							  RGBA.b = 0x0B;
							  RGBA.a = 0xFF;
							  break;
		case Tile::BLUE:	  RGBA.r = 0x0B;
							  RGBA.g = 0xB5;
							  RGBA.b = 0xDB;
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
