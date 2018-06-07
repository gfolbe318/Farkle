#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Player.h"
#include "Game.h"

using namespace std;

void printOpening();

void getPlayers(vector<Player*>& playerVector);

int main() {

	srand(time(NULL));

	vector<Player*> playerVector;
	
	printOpening();
	getPlayers(playerVector);

	Game farkle(playerVector);

	int winningScore = farkle.getWinningScore();

	do {	

		farkle.printScore();
		farkle.runTurn(winningScore);
		winningScore = farkle.getWinningScore();

	} while (playerVector[0]->getScore() < POINTSFORWIN && playerVector[1]->getScore() < POINTSFORWIN);

	cout << "LAST CHANCE!!!" << endl << endl;

	farkle.printScore();
	farkle.runTurn(winningScore);
	farkle.getWinner();
	farkle.printScore();

	cout << endl;
	
	system("pause");
	return 0;
}

void printOpening() {

	cout << "Welcome to Farkle! To select dice, separate your inputs by commas followed by a space.\n"
		"For example, to select dice #2, #4, #5, enter the following input when prompted: \"2, 4, 5\"\n"
		"Input is very sensitive to format. If formatted incorrectly, you will be reprompted.\n"
		"Scoring summaries are as follows:\n"
		"Six of a kind: 3000\n"
		"Five of a kind: 2000\n"
		"Four of a kind: 1500\n"
		"Double triples (Example: 1, 1, 1, 2, 2, 2): 1500\n"
		"Three pairs: 1500\n"
		"Straight (all 6 dice): 1250\n"
		"Three 1's: 1000\n"
		"Three 6's: 600\n"
		"Three 5's: 500\n"
		"Three 4's: 400\n"
		"Three 3's: 300\n"
		"Three 2's: 200\n"
		"1: 100\n"
		"5: 50\n"
		"Dice cannot be used twice. The roll \"1, 1, 2, 2, 4, 4\" will generate 1500 points for \n"
		"three pairs, but not an additional 200 points for the two 1's. At the end of each roll you \n"
		"will be prompted to \"select\" dice. The dice selected will count towards your score, and the\n"
		"remaining dice will be re-rolled. If your current turn generates more than 300 points, you will \n"
		"have the option to \"bank\" these points. A roll generating 0 points is called a FARKLE, and \n"
		"will cause you to lose all of your points gathered in that roll. The first to 10000 points is \n"
		"the winner. Have fun, and good luck!" << endl << endl;
}

void getPlayers(vector<Player*>& playerVector) {
	playerVector.resize(2);

	string playerNameOne;
	string playerNameTwo;

	cout << "Please enter the name of Player One: ";
	getline(cin, playerNameOne);
	Player* playerOne = new Human(playerNameOne);
	playerVector[0] = playerOne;

	string answer;
	do {
		cout << "Are you playing with a friend? Please enter \"Yes\" or \"No\" (enter \"No\" to play against the computer): ";
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
}