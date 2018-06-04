#include <iostream>
#include <vector>
#include "Player.h"

using namespace std;

int main() {

	srand(time(NULL));

	string playerNameOne;
	string playerNameTwo;

	cout << "Please enter the name of Player One: ";
	getline(cin, playerNameOne);
	Human playerOne(playerNameOne);

	string answer;
	do {
		cout << "Are you playing with a friend? Please enter \"Yes\" or \"No\" ";
		getline(cin, answer);

		if (answer != "Yes" && answer != "yes" && answer != "No" && answer != "no") {
			cout << "Invald input" << endl;
		}

	} while (answer != "Yes" && answer != "yes" && answer != "No" && answer != "no");

	if (answer == "No" || answer == "no") {
		//Computer* playerTwo = new Computer();
	}

	else {

		do {
			cout << "Please enter the name of Player Two: ";
			getline(cin, playerNameTwo);

			if (playerNameOne == playerNameTwo) {
				cout << "Please ensure that the names of the players are distinct. Try again." << endl;
			}

		} while (playerNameOne == playerNameTwo);

		Human playerTwo();
	}



	cout << playerOne.getName();

	return 0;
}
