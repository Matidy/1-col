#include "GameController.h"
#include <stdio.h>

class Window {
	
  public:
	//SDL data structures
	SDL_Window* gWindow;
	SDL_Surface* gScreenSurface;
	SDL_Surface* gImage;
	SDL_Renderer* gRenderer;

	//Window dimension constant
	static const int WINDOW_WIDTH = Tile::base_width*Board::hori_tiles;
	static const int WINDOW_HEIGHT = Tile::base_height*Board::vert_tiles;

	Window(void);
	//intialise SDL
	bool init();
	//close and free memory
	void close();

	void draw(std::vector<Tile> tiles_to_draw, int col_percent_offset);
	void draw(std::set<Tile*> tiles_to_draw, int col_percent_offset);
	void draw(Tile tile_to_draw, int col_percent_offset);
	void drawHighlight(Tile TileInFocus);
	void drawUnhighlight(Tile prevTileInFocus);

  private:
	ValRGBA shiftShade(ValRGBA colour, int col_percent_offset);
};