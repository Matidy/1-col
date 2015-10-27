#include "window.h"

const Uint32 FRAME_RATE = 60; //Currently frame rate = 2*FRAME_RATE
const float MINIMUM_FRAME_DURATION = 1000.0f/FRAME_RATE;

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

			if(!game_controller.init(board)) {
				printf("Failed to initialise game controller.\n");
			}

			else {
				// Draw initial board state to screen
				window.draw(board.board_tiles, 0);

				///////////////////////////
				///		 Main Loop      ///
				///////////////////////////
				bool quit = false;
				SDL_Event e;
				int pos_x, pos_y; //UNUSED: Maybe move SDL out to a higher level?
				std::set<Tile*> tiles_to_draw;
				Uint32 start_time = SDL_GetTicks();
				Uint32 current_time = start_time;
				Uint32 time_last_frame;
				Uint32 time_between_frames;
				
				int counted_frames = 0;
		
				while (!quit) {
					/// Frame Rate Cap ///
					time_last_frame = current_time;
					current_time = SDL_GetTicks();
					time_between_frames = current_time - time_last_frame;
					if (time_between_frames <= MINIMUM_FRAME_DURATION)
						SDL_Delay(MINIMUM_FRAME_DURATION - time_between_frames);
					
					/*
					if (current_time - start_time >= 1000) {
						printf("Frame Rate: %d\n", counted_frames);
						start_time = SDL_GetTicks();
						counted_frames = 0;
					}
					++counted_frames;
					*/

					switch(game_controller.phase) {

/*-->*/				case GameController::TILE_CLAIM:

						/// Highlight Hovered Over Tile ///
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
					
						/// User Event Handling ///
						while (SDL_PollEvent(&e) != 0) {       
							if (e.type == SDL_QUIT) {
								quit = true;
							}
							else if (e.button.type == SDL_MOUSEBUTTONUP) {
								game_controller.eventClick(board.tileInFocus, e.button);
								window.drawHighlight(*board.tileInFocus);
							}
							else if (e.type == SDL_KEYUP) { 
								if (e.key.keysym.sym == SDLK_SPACE) {
									window.drawUnhighlight(*board.tileInFocus);
									// END TURN
									game_controller.claimMarkedTiles();
									window.draw(*game_controller.current_player_prev_claimed, 0);
									// CHANGE STATE
									game_controller.phase = GameController::TERRITORY_EXPAND;
									game_controller.generateTilesToCheck(&board, *game_controller.current_player_prev_claimed);
									game_controller.current_player_prev_claimed->clear();
								}
								else if (e.key.keysym.sym == SDLK_r) { // Reset game
									for(auto it=board.board_tiles.begin(); it!=board.board_tiles.end(); ++it) {
										if (it->owner != Tile::UNOWNABLE) 
											it->owner = Tile::NEUTRAL;
									}
									game_controller.reset();
									window.draw(board.board_tiles, 0);
									window.drawHighlight(*board.tileInFocus);
								}
							}
						}
						break;

/*-->*/				case GameController::TERRITORY_EXPAND:	
						tiles_to_draw = game_controller.expandTerritory(&board);
						SDL_Delay(500);
						window.draw(tiles_to_draw, 0);
						if(game_controller.tiles_to_check.empty()) {
							if (game_controller.unclaimed_tiles <=0) {
								game_controller.phase = GameController::GAME_END;
							}
							else {
								game_controller.switchPlayer();
								game_controller.phase = GameController::TILE_CLAIM;
								window.drawHighlight(*board.tileInFocus);
							}
						}
						break;
					
/*-->*/				case GameController::GAME_END:

						/// Highlight Winner's Tiles ///
						GameController::Player winner;
						if (game_controller.claimed_by_orange - game_controller.claimed_by_blue > 0) 
							winner = GameController::ORANGE;
						else if (game_controller.claimed_by_orange - game_controller.claimed_by_blue < 0)
							winner = GameController::BLUE;
						else
							winner = GameController::NONE;
						
						for (auto it = board.board_tiles.begin(); it != board.board_tiles.end(); ++it) {
							if (int(it->owner) == int(winner)) {
								window.draw(*it, 15);
							}
							else {
								window.draw(*it, -15);
							}
						}

						/// User Event Handling ///
						while (SDL_PollEvent(&e) != 0) {       
							if (e.type == SDL_QUIT) {
								quit = true;
							}
							else if (e.type == SDL_KEYUP) { 
								if (e.key.keysym.sym == SDLK_r) { // Reset game
									for(auto it=board.board_tiles.begin(); it!=board.board_tiles.end(); ++it) {
										if (it->owner != Tile::UNOWNABLE) 
										it->owner = Tile::NEUTRAL;
									}
									game_controller.reset();
									window.draw(board.board_tiles, 0);
									window.drawHighlight(*board.tileInFocus);
									game_controller.phase = GameController::TILE_CLAIM;
								}
							}
						}
						break;
					}
					SDL_RenderPresent(window.gRenderer);
				}
			}
		}
	}
	
	window.close();
	SDL_Delay(100);
	return 0;
}