#ifndef GAMEWIN_H
#define GAMEWIN_H

#define TYPE_GAME_WIN "GameWin"
#define GAME_WIN_EVENT "__GameWin__"

#include "Common.h"
#include "Game.h"

class GameWin : public Object {
private:
	game_state* GameState;
	Game* game;
public:
	GameWin(game_state* GameState, Game* g);
	~GameWin();
	int eventHandler(Event* e);
	void draw();
};


#endif