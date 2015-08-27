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

void Window::draw(std::vector<Tile> tiles_to_draw) {
	Tile current_tile;
	for(unsigned int i=0; i<tiles_to_draw.size(); i++) {
		current_tile = tiles_to_draw[i];
		SDL_Rect draw_rect = {current_tile.pos.x, current_tile.pos.y, current_tile.width, current_tile.height};
		setDrawColour(current_tile);
		SDL_RenderFillRect(gRenderer, &draw_rect); //Tile Fill
		struct ValRGBA colour_offset = {0x00, -0x10, -0x30, 0x00};
		setDrawColour(current_tile, colour_offset);
		SDL_RenderDrawRect(gRenderer, &draw_rect); //Tile Outline
	}
}

void Window::setDrawColour(Tile tile) {
	switch(tile.owner) {
		case Tile::YELLOW:    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xAD, 0x33, 0xFF);
							  break;
		case Tile::BLUE:	  SDL_SetRenderDrawColor(gRenderer, 0x33, 0x85, 0xFF, 0xFF);
							  break;
		case Tile::NEUTRAL:   SDL_SetRenderDrawColor(gRenderer, 0x52, 0x52, 0x52, 0xFF);
							  break;
		case Tile::UNOWNABLE: SDL_SetRenderDrawColor(gRenderer, 0x33, 0x33, 0x33, 0xFF);
							  break;
	}
}

// Allows specifiying an offset from the base tile colours. Currently a poor implmentations as it assumes RGB values
// change uniformly for an offset. Counter example, values cannot increase/decrease above FF and below 00 and no reason
// all RGB values will be at these caps at the same time.
void Window::setDrawColour(Tile tile, struct ValRGBA colour_offset) {
	struct ValRGBA draw_colour;
	switch(tile.owner) {
		case Tile::YELLOW:		draw_colour.r = 0xEE + colour_offset.r;
								draw_colour.g = 0xAD + colour_offset.g;
								draw_colour.b =	0x33 + colour_offset.b;
								draw_colour.a = 0xFF + colour_offset.a;
								SDL_SetRenderDrawColor(gRenderer, draw_colour.r, draw_colour.g, draw_colour.b, draw_colour.a);
								break;
		case Tile::BLUE:		draw_colour.r = 0x33 + colour_offset.r;
								draw_colour.g = 0x85 + colour_offset.g;
								draw_colour.b = 0xEE + colour_offset.b;
								draw_colour.a = 0xFF + colour_offset.a;
								SDL_SetRenderDrawColor(gRenderer, draw_colour.r, draw_colour.g, draw_colour.b, draw_colour.a);
								break;
		case Tile::NEUTRAL:		draw_colour.r = 0x52 + colour_offset.r;
								draw_colour.g = 0x52 + colour_offset.g;
								draw_colour.b = 0x52 + colour_offset.b;
								draw_colour.a = 0xFF + colour_offset.a;
								SDL_SetRenderDrawColor(gRenderer, draw_colour.r, draw_colour.g, draw_colour.b, draw_colour.a);
								break;
		case Tile::UNOWNABLE:	draw_colour.r = 0x33 + colour_offset.r;
								draw_colour.g = 0x33 + colour_offset.g;
								draw_colour.b = 0x33 + colour_offset.b;
								draw_colour.a = 0xFF + colour_offset.a;
								SDL_SetRenderDrawColor(gRenderer, draw_colour.r, draw_colour.g, draw_colour.b, draw_colour.a);
								break;
	}
}

// Duplicated a lot of code in draw method, could rework draw to include highlight draw as a case
void Window::drawHighlight(Tile tileInFocus) {
	SDL_Rect highlight_rect = {tileInFocus.pos.x,
							   tileInFocus.pos.y, 
							   tileInFocus.width, 
							   tileInFocus.height};
	ValRGBA highlight_offset = {0x00, 0x10, 0x30, 0xFF};
	setDrawColour(tileInFocus, highlight_offset);
	SDL_RenderFillRect(gRenderer, &highlight_rect);
}

void Window::drawUnhighlight(Tile prevTileInFocus) {
	SDL_Rect unhighlight_rect = {prevTileInFocus.pos.x,
								 prevTileInFocus.pos.y, 
								 prevTileInFocus.width, 
								 prevTileInFocus.height};
	setDrawColour(prevTileInFocus);
	SDL_RenderFillRect(gRenderer, &unhighlight_rect);
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