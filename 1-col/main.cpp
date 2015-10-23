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
					switch(game_controller.phase) {

/*-->*/				case GameController::TILE_CLAIM:

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
								if (e.key.keysym.sym == SDLK_SPACE) {
									// END TURN
									game_controller.claimMarkedTiles();
									window.draw(*game_controller.current_player_prev_claimed, 0);
									game_controller.switchPlayer();
									// CHANGE STATE
									game_controller.phase = GameController::TERRITORY_EXPAND;
									game_controller.generateTilesToCheck(&board, *game_controller.current_player_prev_claimed);
									game_controller.current_player_prev_claimed->clear();		
								}
								else if (e.key.keysym.sym == SDLK_r) {
									for(auto it=board.board_tiles.begin(); it!=board.board_tiles.end(); ++it) {
										if (it->owner != Tile::UNOWNABLE) 
											it->owner = Tile::NEUTRAL;
									}
									window.draw(board.board_tiles, 0);
									window.drawHighlight(*board.tileInFocus);
								}
							}
						}
						break;

/*-->*/				case GameController::TERRITORY_EXPAND:	
						printf("entered\n");
						tiles_to_draw = game_controller.expandTerritory(&board);
						SDL_Delay(500);
						window.draw(tiles_to_draw, 0);
						if(game_controller.tiles_to_check.empty()) {
							game_controller.phase = GameController::TILE_CLAIM;
							window.drawHighlight(*board.tileInFocus);
						}
						break;
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