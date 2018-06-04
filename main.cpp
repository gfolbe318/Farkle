#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Player.h"

using namespace std;

const int NUM_PLAYERS = 2;

bool isValidSelection(string& input);

class Game {
private:
	vector<Player*> playerVector;
	Player* one = playerVector[0];
	Player* two = playerVector[1];
	Player* currentTurn;

	vector<int> roll;

public:
	Game() {}

	Game(vector<Player*> vec_in) : playerVector(vec_in) {
		currentTurn = playerVector[0];
	}

	void updateTurn() {
		if (currentTurn == one) {
			currentTurn = two;
		}
		else {
			currentTurn = one;
		}
	}

	void rollDice() {
		roll.resize(6);
		int x;
		for (int i = 0; i < 6; i++) {
			x = (rand() % 6) + 1;
			roll[i] = x;
		}
	}

	void printRoll() {

		cout << currentTurn->getName() << "'s roll:" << endl;

		for (size_t i = 0; i < roll.size(); i++) {
			cout << "[" << i + 1 << "]  ";
		}
		cout << endl;
		for (size_t i = 0; i < roll.size(); i++) {
			cout << " " << roll[i] << "   ";
		}
		cout << endl;
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

	bool isFarkle() {
		return true;
	}

	void runTurn() {
		bool goodInput = false;
		string selection;

		bool farkleRoll = false;

		do {
			rollDice();
			printRoll();

			farkleRoll = isFarkle;

			if (farkleRoll) {
				cout << "Farkle! You earn 0 points for this roll. " << endl;
				farkleRoll = true;
			}

			else {
				do {
					cout << "Please enter the dice you wish to bank: ";
					getline(cin, selection);

					goodInput = isValidSelection(selection);

					if (!goodInput) {
						printRoll();

					} 
				} while (!goodInput);
			}

			if (roll.size() == 0) {
				roll.resize(6);
			}

		} while (!farkleRoll);
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
		cout << "Are you playing with a friend? Please enter \"Yes\" or \"No\" ";
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

		farkle.runTurn();

		

	} while (playerVector[0]->getScore() < 10000 && playerVector[1]->getScore() < 10000);

	


	return 0;
}


bool isValidSelection(string& input) {
	size_t length = input.length();

	if (input[0] == ' ') {
		cout << "Invalid input, please do not enter a space before your input" << endl;
		return false;
	}

	else if (!isdigit(input[0])) {
		cout << "Error, invalid input" << endl;
		return false;
	}

	for (size_t i = 1; i < length; i++) {
		if ((i + 1) % 3 == 0) {
			if (input[i] != ' ') {
				cout << "Invalid input. Spacing error" << endl;
				return false;
			}
		}
		else if (i % 3 == 0) {
			if (!isdigit(input[0])) {
				cout << "Error, invalid input" << endl;
				return false;
			}
		}

		else if ((i + 2) % 3 == 0) {
			if (input[i] != ',') {
				cout << "Error, invalid input" << endl;
				return false;
			}
		}
	}
	return true;
}