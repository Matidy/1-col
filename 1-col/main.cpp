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
			GameController game_controller;

			if(!game_controller.init()) {
				printf("Failed to initialise game controller.\n");
			}

			else {
				// Draw initial board state to screen
				window.draw(board.board_tiles, 0);

				//////////////////////////////////
				///// Main Loop
				////////////////////
				bool quit = false;
				SDL_Event e;
				int pos_x, pos_y;
		
				while (!quit) {
					///////////////////////////////////////////////
					// Highlight Hovered Over Tile
					////////////////////////////////
					board.tileInFocus = (board.findTileInFocus());
					if (board.tileInFocus.ID != board.prevTileInFocus.ID) {
						if (board.prevTileInFocus.ID != NULL) {
							window.drawUnhighlight(board.prevTileInFocus);
						}
						if(board.tileInFocus.ID != NULL) {
							window.drawHighlight(board.tileInFocus);
						}
						board.prevTileInFocus = board.tileInFocus;
					}
					////////////////////////////////////////////////
					///// User Event Handling
					////////////////////////////////
					while (SDL_PollEvent(&e) != 0) {       
						if (e.type == SDL_QUIT) {
							quit = true;
						}
						// 1=left, 2=middle, 3=right
						else if (e.button.type == SDL_MOUSEBUTTONUP) {
							printf("Event type: %d\n", e.button.button);
							game_controller.eventClick(&board, e.button);
							window.drawHighlight(board.tileInFocus);
						}
						else if (e.type == SDL_KEYUP) {
							if (e.key.keysym.sym == SDLK_SPACE) {
								game_controller.endTurn(&board);
								window.draw(board.board_tiles, 0); // Inefficient: Redraws all Tiles rather than just those which have changed
								
							}
						}
					}

					SDL_RenderPresent(window.gRenderer);
					//Update the surface
					//SDL_UpdateWindowSurface(window.gWindow);
				}
			}
		}
	}
	
	window.close();
	SDL_Delay(100);
	return 0;
}