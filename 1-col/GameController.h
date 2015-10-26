#include "Board.h"
#include <set>

class GameController {
public:
	const static int TILES_NEEDED_TO_CLAIM = 5;

	enum Player {
		ORANGE,
		BLUE,
		NONE
	};

	enum Phase {
		TILE_CLAIM,
		TERRITORY_EXPAND,
		GAME_END
	};

	Phase phase;
	int unclaimed_tiles;
	int claimed_by_orange;
	int claimed_by_blue;

	Player current_player;
	int orange_tile_count;
	int blue_tile_count;
	int* current_player_tile_count;
	// Sets of the Tiles claimed by a player on their previous term.
	std::set<Tile*> orange_prev_claimed; 
	std::set<Tile*> blue_prev_claimed;
	std::set<Tile*>* current_player_prev_claimed;
	
	std::set<Tile*> tiles_to_check;

	GameController(void);
	int init(Board board);
	~GameController(void);

	void reset();
	void eventClick(Tile* clickedTile, SDL_MouseButtonEvent mouse_event);
	void claimMarkedTiles();
	void switchPlayer();
	void claimTile(Player current_player, Tile* tile_to_claim);
	Player getOtherPlayer (Player player);
	void generateTilesToCheck(Board *board, std::set<Tile*> claimed_tiles);
	std::set<Tile*> expandTerritory(Board *board);
	std::set<Tile*> checkAdjacency(Board *board, std::set<Tile*> tiles_to_check);
	
};

