#include "ValRGBA.h"

class Tile {

  public:
	struct Vec2 {
		int x, y;

		void init(int x, int y);
	};

	enum Owner {
		ORANGE,
		BLUE,
		ORANGE_MARKED,
		BLUE_MARKED,
		NEUTRAL,
		UNOWNABLE
	};

	static const int base_width = 64;
	static const int base_height = 64;

	int width;
	int height;
	struct Vec2 pos;
	struct Vec2 index;
	Owner owner;

	Tile();
	~Tile();

	void initPosition(int x, int y);
	void initIndex(int i);
	ValRGBA getColour();
};