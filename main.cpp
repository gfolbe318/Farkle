#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Player.h"

using namespace std;

const int NUM_PLAYERS = 2;

struct info {
	vector<int> dice;
	vector<int> selected;
	string input;
	int numPoints;
	bool goodInput;

	info(string input_in, int numPoints_in, bool goodInput_in)
		:dice(6, 0), input(input_in), numPoints(numPoints_in), goodInput(goodInput_in) {}
};

struct pointHelper {
	int number;
	int count;
};

info isValidSelection(info& info_in);

int getPoints(info& info_in);

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


		cout << endl << currentTurn->getName() << "'s roll:" << endl;

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
		int numOnes = 0;
		int numFives = 0;
		for (size_t i = 0; i < roll.size(); i++) {
			if (roll[i] == 1 || roll[i] == 5) {
				numOnes++;
			}
		}

		if (numOnes == 0) {
			return true;
		}
		return false;
	}

	void runTurn() {
		printScore();

		bool goodInput = false;
		bool bank = false;
		string input;

		info diceSelection{ "", 0, false };

		bool farkleRoll = false;

		do {
			rollDice();
			printRoll();

			diceSelection.dice = roll;

			farkleRoll = isFarkle();

			if (farkleRoll) {
				cout << "Farkle! You earn 0 points for this roll. " << endl;
				farkleRoll = true;
			}

			else {
				do {
					cout << "Current points this turn: " << diceSelection.numPoints << endl;

					cout << "Please enter the dice you wish to bank";
					if (diceSelection.numPoints > 300) {
						cout << "if you wish to bank " << diceSelection.numPoints << " points, type \"bank\"): ";
					}
					else {
						cout << " (you cannot bank until you have at least 300 points): ";
					}
					getline(cin, input);

					diceSelection.input = input;
				
					isValidSelection(diceSelection);

					goodInput = diceSelection.goodInput;

					if (!goodInput) {
						printRoll();

					} 
				} while (!goodInput);

				if (input == "Bank" || input == "bank") {
					one->addPoints(diceSelection.numPoints);
					cout << one->getName() << " banked " << diceSelection.numPoints << "!";
					break;
				}
			}

			if (roll.size() == 0) {
				roll.resize(6);
			}

		} while (!farkleRoll);

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
		farkle.runTurn();

	} while (playerVector[0]->getScore() < 10000 && playerVector[1]->getScore() < 10000);	


	return 0;
}


info isValidSelection(info& info_in) {

	string input = info_in.input;
	int numDice = int(info_in.dice.size());

	if (input == "Bank" || input == "bank") {
		if (info_in.numPoints > 300) {
			info_in.goodInput = true;
		}
		else {
			cout << "You do not have enough points to bank yet" << endl;
			info_in.goodInput = false;
		}

		return info_in;
	}

	if (input[0] == ' ') {
		cout << "Invalid input, please do not enter a space before your input" << endl;
		info_in.goodInput = false;
		return info_in;
	}


	else if (!isdigit(input[0])) {
		cout << "Error, cannot bank die # " << input[0] << endl;
		info_in.goodInput = false;
		return info_in;
	}

	size_t length = input.length();
	for (size_t i = 0; i < length; i++) {

		char current = input[i];

		if ((i + 1) % 3 == 0) {
			if (current != ' ') {
				cout << "Invalid input. Spacing error" << endl;
				info_in.goodInput = false;
				info_in.dice.clear();
				return info_in;
			}
		}
		else if (i % 3 == 0) {
			if (!isdigit(input[0])) {
				cout << "Error, invalid input" << endl;
				info_in.goodInput = false;
				info_in.dice.clear();
				return info_in;
			}
			else if ((current - '0' > numDice)) {
				cout << "Error, cannot bank die # " << current << endl;
				info_in.goodInput = false;
				info_in.dice.clear();
				return info_in;
			}
			else {
				size_t initialSize = info_in.selected.size();
				for (size_t j = 0; j < initialSize; j++) {
					if (current - '0' == info_in.selected[j]) {
						cout << "Error, you have selected the same die twice" << endl;
						info_in.goodInput = false;
						info_in.dice.clear();
						info_in.selected.clear();
						return info_in;
					}
				}
				info_in.selected.push_back(current - '0');
			}
		}

		else if ((i + 2) % 3 == 0) {
			if (current != ',') {
				cout << "Error, invalid input" << endl;
				info_in.goodInput = false;
				info_in.dice.clear();
				return info_in;
			}
		}
		else {
			cout << "Error, invalid input" << endl;
			info_in.goodInput = false;
			info_in.dice.clear();
			return info_in;
		}
	}

	int pointsFromRoll = getPoints(info_in);

	info_in.goodInput = true;

	return info_in;
}

int getPoints(info& info_in) {

	int points = 0;

	pointHelper p{ 0, 0 };

	vector<pointHelper> selectedDice;

	for (size_t i = 1; i < 7; i++) {
		for (size_t j = 0; j < info_in.selected.size(); j++) {
			int x = info_in.dice[info_in.selected[j] - 1];
			p.number = i;
			if (info_in.dice[info_in.selected[j] - 1] == i) {
				p.count++;
			}
		}
		if (p.count > 0) {
			selectedDice.push_back(p);
		}
		p.count = 0;
	}

	if (selectedDice.size() == 6) {
		points += 1250;
		return points;
	}

	else {

	}


	return -1;
}