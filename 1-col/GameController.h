#include "Board.h"
#include <set>

class GameController {
public:
	const static int TILES_NEEDED_TO_CLAIM = 4;

	enum Player {
		YELLOW,
		BLUE
	};

	Player current_player;
	SDL_Rect end_turn_button;
	int tile_count;

	std::set<Tile*> tiles_claimed_this_turn;
	std::set<Tile*> tiles_unclaimed_this_turn;
	std::set<Tile*> tiles_to_check;

	GameController(void);
	int init();
	~GameController(void);

	void switchPlayer();
	void generateTilesToCheck(Board *board);
	std::set<Tile*> expandTerritory(Board *board);
	std::set<Tile*> checkAdjacency(Board *board);
	void eventClick(Tile* clickedTile, SDL_MouseButtonEvent mouse_event);
};

