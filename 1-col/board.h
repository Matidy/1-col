#include "tile.h"
#include <SDL.h>

class Board {

  public:
	Tile * tileInFocus;
	Tile * prevTileInFocus;
	int hori_tiles;
	int vert_tiles;

	Board(void);
	~Board(void);

	int init();
	Tile findMouseTile();
	Tile getTile(int row, int col);

  private:
	Tile * tiles_array;
};

