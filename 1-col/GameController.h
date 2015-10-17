#include "Board.h"

class GameController {
public:
	enum Player {
		YELLOW,
		BLUE
	};

	Player current_player;
	SDL_Rect end_turn_button;
	int tile_count;

	GameController(void);
	int init();
	~GameController(void);

	std::vector<Tile> endTurn(Board *board);
	std::vector<Tile*> checkAdjacency(Board *board);
	void eventClick(Tile* clickedTile, SDL_MouseButtonEvent mouse_event);
};

