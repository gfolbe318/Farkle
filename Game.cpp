#include "Game.h"	

using namespace std;

Game::Game() {}

Game::Game(vector<Player*> vec_in) : playerVector(vec_in) {
	currentRoller = playerVector[0];
}

void Game::updateTurn() {
	if (currentRoller == one) {
		currentRoller = two;
	}
	else {
		currentRoller = one;
	}
}

void Game::printScore() {
	cout << "SCOREBOARD:" << endl;

	//Print score in order
	if (one->getScore() >= two->getScore()) {
		cout << one->getName() << ": " << one->getScore() << endl;
		cout << two->getName() << ": " << two->getScore() << endl;
	}
	else {
		cout << two->getName() << ": " << two->getScore() << endl;
		cout << one->getName() << ": " << one->getScore() << endl;
	}
}

void Game::runTurn(int winningScore) {

	currentRoller->getDice(winningScore);

	updateTurn();
}

int Game::getWinningScore() {
	if (one->getScore() > two->getScore()) {
		return one->getScore();
	}
	else {
		return two->getScore();
	}
}

void Game::getWinner() {
	if (one->getScore() > two->getScore()) {
		cout << one->getName();
	}

	else {
		cout << two->getName();
	}

	cout << " Wins!!" << endl << endl;
}