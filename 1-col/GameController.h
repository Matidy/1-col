#include "Board.h"
#include <set>

class GameController {
public:
	const static int TILES_NEEDED_TO_CLAIM = 5;

	enum Player {
		ORANGE,
		BLUE
	};

	Player current_player;
	SDL_Rect end_turn_button;
	int orange_tile_count;
	int blue_tile_count;
	int* current_player_tile_count;

	std::set<Tile*> tiles_claimed_this_turn;
	std::set<Tile*> tiles_to_check;

	GameController(void);
	int init();
	~GameController(void);

	void eventClick(Tile* clickedTile, SDL_MouseButtonEvent mouse_event);
	void claimMarkedTiles();
	void switchPlayer();
	void generateTilesToCheck(Board *board, std::set<Tile*> claimed_tiles);
	std::set<Tile*> expandTerritory(Board *board);
	std::set<Tile*> checkAdjacency(Board *board, std::set<Tile*> tiles_to_check);
	
};

