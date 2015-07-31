#pragma once

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

	int width;
	int height;
	struct Position * pos;

	Tile();
	~Tile();

	void initPosition(int x, int y);
	void draw();
	bool containsMouse();
	void leftClick();
	void rightClick();

};