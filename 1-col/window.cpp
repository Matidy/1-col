//Using SDL and standard IO
//#include <SDL.h>
#include <stdio.h>

#include "window.h"



// using namespace std;
Window::Window(void) {
	gWindow = NULL;
	gScreenSurface = NULL;
	gImage = NULL;
	gRenderer = NULL;
}

//////////////////////////////////
//Function Implementations
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

bool Window::loadMedia()
{
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

void Window::draw(std::vector<Tile> tiles_to_draw, int col_percent_offset) { //col updated to percent, update methods (don't like having to provide dummy 0 for base draw)
	Tile current_tile;
	ValRGBA tile_colour;
	int rim_offset;
	for(unsigned int i=0; i<tiles_to_draw.size(); i++) {
		current_tile = tiles_to_draw[i];
		SDL_Rect draw_rect = {current_tile.pos.x, current_tile.pos.y, current_tile.width, current_tile.height};
		tile_colour = shiftShade(current_tile.getColour(), col_percent_offset);
		SDL_SetRenderDrawColor(gRenderer, tile_colour.r, tile_colour.g, tile_colour.b, tile_colour.a);
		SDL_RenderFillRect(gRenderer, &draw_rect); //Tile Fill
		rim_offset = -10;
		tile_colour = shiftShade(tile_colour, rim_offset);
		SDL_SetRenderDrawColor(gRenderer, tile_colour.r, tile_colour.g, tile_colour.b, tile_colour.a);
		SDL_RenderDrawRect(gRenderer, &draw_rect); //Tile Outline
	}
}

ValRGBA Window::shiftShade(ValRGBA colour, int col_percent_offset) {
	colour.r = colour.r*(100 + col_percent_offset)/100;
	colour.g = colour.g*(100 + col_percent_offset)/100;
	colour.b = colour.b*(100 + col_percent_offset)/100;
	colour.a = 0xFF;
	return colour;
}

// Duplicated a lot of code in draw method, could rework draw to include highlight draw as a case
void Window::drawHighlight(Tile tileInFocus) {
	std::vector<Tile> dummy_vector; //wrapper for tile as draw expects a vector
	dummy_vector.resize(1);
	dummy_vector[0] = tileInFocus;
	draw(dummy_vector, 15);
}

void Window::drawUnhighlight(Tile prevTileInFocus) {
	std::vector<Tile> dummy_vector; //wrapper for tile as draw expects a vector
	dummy_vector.resize(1);
	dummy_vector[0] = prevTileInFocus;
	draw(dummy_vector, 0);
}

void Window::eventClick() {
	int x, y;
	SDL_GetMouseState(&x, &y);

	printf("Click at: %d, %d\n", x, y);
	/*SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xFF, 0xFF);
	// init struct SDL_Rect by giving an array. Sets members in the order they are delcared with corresponding array elements.
	SDL_Rect rect1 = {x-30, y-30, 60, 60};
	SDL_RenderFillRect(gRenderer, &rect1);*/
}