#include "tile.h"
#include <SDL.h>
#include <vector>

class Board {

  public:
	static const int hori_tiles = 9;
	static const int vert_tiles = 9;

	Tile *tileInFocus;
	Tile *prevTileInFocus;

	Tile NULL_tile;
	std::vector<Tile> board_tiles;
	int tile_array_size;
	int board_width;
	int board_height;
	int claimable_tiles;

	Board(void);
	int init();
	~Board(void);

	Tile* getTile(int row, int col);
	int getUID(Tile tile);
	int getUID(int row, int col);
	Tile* findTileInFocus();

  private:

};