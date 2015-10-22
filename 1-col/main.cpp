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

				///// Main Loop
				////////////////////
				bool quit = false;
				SDL_Event e;
				int pos_x, pos_y; //UNUSED: Maybe move SDL out to a higher level?
				std::set<Tile*> tiles_to_draw;
		
				while (!quit) {

					///// Highlight Hovered Over Tile
					////////////////////////////////////
					board.tileInFocus = board.findTileInFocus();
					if (board.getUID(*board.tileInFocus) != board.getUID(*board.prevTileInFocus)) {
						if (board.getUID(*board.prevTileInFocus) != 0) { // 0->Invalid(NULL)
							window.drawUnhighlight(*board.prevTileInFocus);
						}
						if(board.getUID(*board.tileInFocus) != 0) {
							window.drawHighlight(*board.tileInFocus);
						}
						board.prevTileInFocus = board.tileInFocus;
					}
					
					///// User Event Handling
					////////////////////////////////
					while (SDL_PollEvent(&e) != 0) {       
						if (e.type == SDL_QUIT) {
							quit = true;
						}
						// 1=left, 2=middle, 3=right
						else if (e.button.type == SDL_MOUSEBUTTONUP) {
							///////////////////////// DEBUG /////////////////////////
							printf("Event type: %d\n", e.button.button);
							///////////////////////// DEBUG /////////////////////////
							game_controller.eventClick(board.tileInFocus, e.button);
							window.drawHighlight(*board.tileInFocus);
						}
						else if (e.type == SDL_KEYUP) { 
							if (e.key.keysym.sym == SDLK_SPACE) { //END TURN
								window.drawUnhighlight(*board.tileInFocus);
								game_controller.claimMarkedTiles();
								window.draw(game_controller.tiles_claimed_this_turn, 0);
								do {
									game_controller.generateTilesToCheck(&board, game_controller.tiles_claimed_this_turn);
									tiles_to_draw = game_controller.expandTerritory(&board);
									window.draw(tiles_to_draw, 0);
									SDL_RenderPresent(window.gRenderer);
									SDL_Delay(500);
								} while(!game_controller.tiles_to_check.empty());
								game_controller.switchPlayer();
								window.drawHighlight(*board.tileInFocus); //Need to handle the cursor being over a different tile after the territory expand phase has finished.
							}
							if (e.key.keysym.sym == SDLK_r) {
								for(auto it=board.board_tiles.begin(); it!=board.board_tiles.end(); ++it) {
									if (it->owner != Tile::UNOWNABLE) 
										it->owner = Tile::NEUTRAL;
								}
								window.draw(board.board_tiles, 0);
								window.drawHighlight(*board.tileInFocus);
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