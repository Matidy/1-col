#include "ValRGBA.h"

class Tile {

  public:
	struct Position {
		int x, y;
	};

	enum Owner {
		YELLOW,
		BLUE,
		NEUTRAL,
		UNOWNABLE
	};

	static const int base_width = 64;
	static const int base_height = 64;

	int ID;
	int width;
	int height;
	struct Position pos;
	Owner owner;

	Tile();
	~Tile();

	void initPosition(int x, int y);
	ValRGBA getColour();

};