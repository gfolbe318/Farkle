#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Player.h"

using namespace std;

const int NUM_PLAYERS = 2;

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

	void runTurn() {

		currentRoller->getDice();

		updateTurn();
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

	do {
		farkle.printScore();
		farkle.runTurn();

	} while (playerVector[0]->getScore() < 10000 && playerVector[1]->getScore() < 10000);




	return 0;
}

