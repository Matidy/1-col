#include <SDL.h>
#include <stdio.h>

#include "window.h"


int main(int argc, char* args[]) {
	Window window;

	if(!window.init()) {
		printf("Failed to initialise SDL and/or game window.\n");
		}

	else {
		Board board;

		if(!board.init()) {
			printf("Failed to intitialise board instance.\n");
		}

		else {

			// Draw initial board state to screen
			window.draw(board.board_tiles);
			//////////////////
			//Event Loop
			////////////
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
							printf("Left Click\n");
							window.eventClick();
						}

					}
				}
				//Apply Image to window's surface
				//SDL_BlitSurface(gImage, NULL, gScreenSurface, NULL);

				SDL_RenderPresent(window.gRenderer);
				//Update the surface
				//SDL_UpdateWindowSurface(gWindow);
			}
		}
	}
	

	window.close();
	SDL_Delay(100);
	return 0;
}