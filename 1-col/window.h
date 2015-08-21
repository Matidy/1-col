#include <SDL.h>
#include "board.h"

class Window {
	
	//////////////////////////////////
	//Global variables/constants
	//////////////////////////////////
  public:
	//SDL data structures
	SDL_Window* gWindow;
	SDL_Surface* gScreenSurface;
	SDL_Surface* gImage;
	SDL_Renderer* gRenderer;

	//Window dimension constant
	static const int WINDOW_WIDTH = 640;
	static const int WINDOW_HEIGHT = 480;

	static const int MOUSE_EVENT = 1026;
	//////////////////////////////////
	//Function prototypes
	//////////////////////////////////

	//constructor
	Window(void);
	//intialise SDL
	bool init();
	//load media
	bool loadMedia();
	//close and free memory
	void close();
	void draw(std::vector<Tile> tiles_to_draw);
	void setDrawColour(Tile tile);
	void findTileInFocus();
	void eventClick();
};