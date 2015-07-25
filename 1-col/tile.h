#pragma once

class Tile {

  public:
	enum Owner {
		YELLOW,
		BLUE,
		NEUTRAL,
		UNOWNABLE
	};
	int width;
	int height;

	Tile();
	~Tile();

	void draw();
	bool containsMouse();
	void leftClick();
	void rightClick();

};