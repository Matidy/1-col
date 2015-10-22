#include "tile.h"
#include <SDL.h>
#include <vector>

class Board {

  public:
	static const int hori_tiles = 10;
	static const int vert_tiles = 12;

	Tile *tileInFocus;
	Tile *prevTileInFocus;

	Tile NULL_tile;
	std::vector<Tile> board_tiles;
	int tile_array_size;
	int board_width;
	int board_height;

	Board(void);
	int init();
	~Board(void);

	void update();
	Tile* getTile(int row, int col);
	int getUID(Tile tile);
	int getUID(int row, int col);
	Tile* findTileInFocus();
};