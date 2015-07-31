#include <SDL.h>
#include <stdio.h>

#include "window.h"


int main(int argc, char* args[]) {
	Window window;

	// Find out how to reference class methods or something
	if(!window.init()) {
		printf("Failed to initialise SDL and/or game window.\n");
		}

	else {
		
		bool quit = false;
		SDL_Event e;

		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {       
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				else if (e.type == SDL_MOUSEBUTTONUP) {
					printf("Clicked\n");
					printf("Event type: %d\n", e.button.type);
					
					if (e.button.type == window.MOUSE_EVENT) {
						printf("Left Click");
						window.eventClick();
					}

				}
			}
			//Apply Image to window's surface
			//SDL_BlitSurface(gImage, NULL, gScreenSurface, NULL);

			SDL_RenderPresent(window.gRenderer);
			//Update the surface
			//SDL_UpdateWindowSurface(gWindow);

			int x = 10;
			vector<int>
		}
	}
	

	window.close();
	SDL_Delay(3000);
	return 0;
}