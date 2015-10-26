#include "window.h"

Window::Window(void) {
	gWindow = NULL;
	gScreenSurface = NULL;
	gImage = NULL;
	gRenderer = NULL;
}


// Function Implementations
//////////////////////////////////
bool Window::init() {
	bool success = false;

	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		//Create window
		gWindow = SDL_CreateWindow("1-col", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else {
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if(gRenderer == NULL) {
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else {
				gScreenSurface = SDL_GetWindowSurface(gWindow);
				success = true;
			}
		}
	}

	return success;
}

bool Window::loadMedia() {
    //Loading success flag
    bool success = true;

    //Load image
    gImage = SDL_LoadBMP("Resource/city_night.bmp");
    if(gImage == NULL)
    {
        printf("Unable to load image %s! SDL Error: %s\n", "Resource/city_night.bmp", SDL_GetError());
        success = false;
    }

    return success;
}

void Window::close() {
	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

// Draws an array of Tiles to the screen
void Window::draw(std::vector<Tile> tiles_to_draw, int col_percent_offset) { 
	Tile current_tile;
	ValRGBA tile_colour;
	int rim_offset = -10;

	for(unsigned int i=0; i<tiles_to_draw.size(); i++) {
		current_tile = tiles_to_draw[i];
		SDL_Rect draw_rect = {current_tile.pos.x, current_tile.pos.y, current_tile.width, current_tile.height};
		tile_colour = shiftShade(current_tile.getColour(), col_percent_offset);
		SDL_SetRenderDrawColor(gRenderer, tile_colour.r, tile_colour.g, tile_colour.b, tile_colour.a);
		SDL_RenderFillRect(gRenderer, &draw_rect); //Tile Fill
		tile_colour = shiftShade(tile_colour, rim_offset);
		SDL_SetRenderDrawColor(gRenderer, tile_colour.r, tile_colour.g, tile_colour.b, tile_colour.a);
		SDL_RenderDrawRect(gRenderer, &draw_rect); //Tile Outline
	}
}

// function overload for array of pointers to tiles
void Window::draw(std::set<Tile*> tiles_to_draw, int col_percent_offset) { 
	Tile* current_tile;
	ValRGBA tile_colour;
	int rim_offset = -10;

	for(auto it = tiles_to_draw.begin(); it!=tiles_to_draw.end(); ++it) {
		current_tile = *it;
		SDL_Rect draw_rect = {current_tile->pos.x, current_tile->pos.y, current_tile->width, current_tile->height};
		tile_colour = shiftShade(current_tile->getColour(), col_percent_offset);
		SDL_SetRenderDrawColor(gRenderer, tile_colour.r, tile_colour.g, tile_colour.b, tile_colour.a);
		SDL_RenderFillRect(gRenderer, &draw_rect); //Tile Fill
		tile_colour = shiftShade(tile_colour, rim_offset);
		SDL_SetRenderDrawColor(gRenderer, tile_colour.r, tile_colour.g, tile_colour.b, tile_colour.a);
		SDL_RenderDrawRect(gRenderer, &draw_rect); //Tile Outline
	}
}

// function overload of above for single tiles 
void Window::draw(Tile tile_to_draw, int col_percent_offset) { 
	ValRGBA tile_colour;
	int rim_offset = -10;

	SDL_Rect draw_rect = {tile_to_draw.pos.x, tile_to_draw.pos.y, tile_to_draw.width, tile_to_draw.height};
	tile_colour = shiftShade(tile_to_draw.getColour(), col_percent_offset);
	SDL_SetRenderDrawColor(gRenderer, tile_colour.r, tile_colour.g, tile_colour.b, tile_colour.a);
	SDL_RenderFillRect(gRenderer, &draw_rect); //Tile Fill
	tile_colour = shiftShade(tile_colour, rim_offset);
	SDL_SetRenderDrawColor(gRenderer, tile_colour.r, tile_colour.g, tile_colour.b, tile_colour.a);
	SDL_RenderDrawRect(gRenderer, &draw_rect); //Tile Outline
}

// percentage shift of a RGBA colour
ValRGBA Window::shiftShade(ValRGBA colour, int col_percent_offset) {
	colour.r = colour.r*(100 + col_percent_offset)/100;
	colour.g = colour.g*(100 + col_percent_offset)/100;
	colour.b = colour.b*(100 + col_percent_offset)/100;
	colour.a = colour.a;
	return colour;
}

void Window::drawHighlight(Tile tileInFocus) {
	draw(tileInFocus, 15);
}

void Window::drawUnhighlight(Tile prevTileInFocus) {
	draw(prevTileInFocus, 0);
}