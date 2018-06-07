#ifndef GAME_H
#define GAME_H

#include "Player.h"

#include <vector>
#include <iostream>

using namespace std;

class Game {
private:
	vector<Player*> playerVector;
	Player* one = playerVector[0];
	Player* two = playerVector[1];
	Player* currentRoller;

public:
	Game();

	Game(vector<Player*> vec_in);

	void updateTurn();

	void printScore();

	void runTurn(int winningScore);

	int getWinningScore();

	void getWinner();
};

#endif GAME_H