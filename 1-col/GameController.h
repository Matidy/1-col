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

	void endTurn(Board *board);
	std::vector<int> checkAdjacency(Board *board);
	void eventClick(Board *board, SDL_MouseButtonEvent mouse_event);
};

