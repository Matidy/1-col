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
	static const int WINDOW_WIDTH = 64*10;
	static const int WINDOW_HEIGHT = 64*8;

	//constructor
	Window(void);
	//intialise SDL
	bool init();
	//load media
	bool loadMedia();
	//close and free memory
	void close();

	void draw(std::vector<Tile> tiles_to_draw, int col_percent_offset);
	void draw(std::set<Tile*> tiles_to_draw, int col_percent_offset);
	void draw(Tile tile_to_draw, int col_percent_offset);
	ValRGBA shiftShade(ValRGBA colour, int col_percent_offset);
	void drawHighlight(Tile TileInFocus);
	void drawUnhighlight(Tile prevTileInFocus);
	void findTileInFocus(std::vector<Tile> board_tiles);
};