#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Player.h"

using namespace std;

class Game {
private:
	vector<Player*> playerVector;
	Player* one = playerVector[0];
	Player* two = playerVector[1];
	Player* currentRoller;

public:
	Game() {}

	Game(vector<Player*> vec_in) : playerVector(vec_in) {
		currentRoller = playerVector[0];
	}

	void updateTurn() {
		if (currentRoller == one) {
			currentRoller = two;
		}
		else {
			currentRoller = one;
		}
	}

	void printScore() {
		cout << "SCOREBOARD:" << endl;
		if (one->getScore() >= two->getScore()) {
			cout << one->getName() << ": " << one->getScore() << endl;
			cout << two->getName() << ": " << two->getScore() << endl;
		}
		else {
			cout << two->getName() << ": " << two->getScore() << endl;
			cout << one->getName() << ": " << one->getScore() << endl;
		}
	}

	void runTurn(int winningScore) {

		currentRoller->getDice(winningScore);

		updateTurn();
	}

	Player* getCurrentPlayer() {
		return currentRoller;
	}

	int getWinningScore() {
		if (one->getScore() > two->getScore()) {
			return one->getScore();
		}
		else {
			return two->getScore();
		}
	}

	void getWinner() {
		if (one->getScore() > two->getScore()){
			cout << one->getName();
		}

		else {
			cout << two->getName();
		}

		cout << " Wins!!" << endl;
	}
};

int main() {

	srand(time(NULL));

	cout << "Welcome to Farkle! To select dice..." << endl;

	vector<Player*> playerVector;

	playerVector.resize(2);

	string playerNameOne;
	string playerNameTwo;

	cout << "Please enter the name of Player One: ";
	getline(cin, playerNameOne);
	Player* playerOne = new Human(playerNameOne);
	playerVector[0] = playerOne;

	string answer;
	do {
		cout << "Are you playing with a friend? Please enter \"Yes\" or \"No\": ";
		getline(cin, answer);

		if (answer != "Yes" && answer != "yes" && answer != "No" && answer != "no") {
			cout << "Invald input" << endl;
		}

	} while (answer != "Yes" && answer != "yes" && answer != "No" && answer != "no");

	if (answer == "No" || answer == "no") {
		Computer* playerTwo = new Computer();
		playerVector[1] = playerTwo;
	}

	else {

		do {
			cout << "Please enter the name of Player Two: ";
			getline(cin, playerNameTwo);

			if (playerNameOne == playerNameTwo) {
				cout << "Please ensure that the names of the players are distinct. Try again." << endl;
			}

		} while (playerNameOne == playerNameTwo);

		Player* playerTwo = new Human(playerNameTwo);
		playerVector[1] = playerTwo;
	}

	cout << endl;

	Game farkle(playerVector);

	int winningScore = farkle.getWinningScore();

	do {	

		farkle.printScore();
		farkle.runTurn(winningScore);

		winningScore = farkle.getWinningScore();

	} while (playerVector[0]->getScore() < POINTSFORWIN && playerVector[1]->getScore() < POINTSFORWIN);

	cout << "LAST CHANCE!!!" << endl;

	farkle.printScore();
	farkle.runTurn(winningScore);

	farkle.getWinner();

	farkle.printScore();
	
	return 0;
}