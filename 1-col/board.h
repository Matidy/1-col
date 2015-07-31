#include "tile.h"
#include <SDL.h>
//#include<vector>

class Board {

  public:
	Tile * tileInFocus;
	Tile * prevTileInFocus;
	int hori_tiles;
	int vert_tiles;
	int tile_array_size;

	Board(void);
	int init();
	~Board(void);

	void update();
	Tile findMouseTile();
	Tile getTile(int row, int col);

  private:
	Tile * tiles_array;
};

