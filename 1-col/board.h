#include "tile.h"
#include <SDL.h>
#include <vector>

class Board {

  public:
	Tile tileInFocus;
	Tile prevTileInFocus;

	static const int hori_tiles = 10;
	static const int vert_tiles = 8;

	int tile_array_size;

	Board(void);
	int init();
	~Board(void);

	void update();
	Tile getTile(int row, int col);

	std::vector<Tile> board_tiles;

	Tile findTileInFocus(std::vector<Tile> board_tiles) ;
};