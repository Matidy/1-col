#include "tile.h"

Tile::Tile() {
	owner = NEUTRAL;
	width = base_width;
	height = base_height;
}

Tile::~Tile() {

}

void Tile::Vec2::init(int x, int y) {
	Vec2::x = x;
	Vec2::y = y;
}

ValRGBA Tile::getColour() {
	ValRGBA RGBA;
	switch(owner) {
		case Tile::ORANGE:			 RGBA.r = 0xDB;
									 RGBA.g = 0x31;
									 RGBA.b = 0x0B;
									 RGBA.a = 0xFF;
									 break;
		case Tile::BLUE:			 RGBA.r = 0x0B;
									 RGBA.g = 0xB5;
									 RGBA.b = 0xDB;
									 RGBA.a = 0xFF;
									 break;
		case Tile::ORANGE_MARKED:    RGBA.r = 0xB7;
									 RGBA.g = 0x5A;
									 RGBA.b = 0x47;
									 RGBA.a = 0xFF;
									 break;
		case Tile::BLUE_MARKED:		 RGBA.r = 0x60;
									 RGBA.g = 0x93;
									 RGBA.b = 0x9E;
									 RGBA.a = 0xFF;
									 break;
		case Tile::NEUTRAL:			 RGBA.r = 0x52;
									 RGBA.g = 0x52;
									 RGBA.b = 0x52;
									 RGBA.a = 0xFF;
									 break;
		case Tile::UNOWNABLE:		 RGBA.r = 0x33;
								     RGBA.g = 0x33;
							         RGBA.b = 0x33;
									 RGBA.a = 0xFF;
									 break;
	}

	return RGBA;
}
