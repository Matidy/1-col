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

    //Load splash image
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

void Window::eventClick() {
	int x, y;
	SDL_GetMouseState(&x, &y);

	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xFF, 0xFF);
	// init struct SDL_Rect by giving an array. Sets members in the order they are delcared with corresponding array elements.
	SDL_Rect rect1 = {x-30, y-30, 60, 60};
	SDL_RenderFillRect(gRenderer, &rect1);
}