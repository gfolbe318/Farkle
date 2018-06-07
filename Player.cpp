#include <vector>
#include <string>
#include <iostream>

#include "Player.h"

using namespace std;

Player::Player()
	:name(""), points(0) {}

Player::Player(string name_in)
	: name(name_in), points(0) {}

string Player::getName() {
	return name;
}

int Player::getScore() {
	return points;
}

void Player::addPoints(int points_in) {
	points += points_in;
}

void Player::printRoll(vector<int>& roll) {

	cout << endl << getName() << "'s roll:" << endl;

	for (size_t i = 0; i < roll.size(); i++) {
		cout << "[" << i + 1 << "]  ";
	}
	cout << endl;
	for (size_t i = 0; i < roll.size(); i++) {
		cout << " " << roll[i] << "   ";
	}
	cout << endl;
}

void Player::rollDice(info& info_in) {

	//Pause for better user experience
	chrono::seconds dura(1);
	this_thread::sleep_for(dura);

	//Generate random numbers for roll
	for (size_t i = 0; i < info_in.dice.size(); i++) {
		info_in.dice[i] = (rand() % 6) + 1;
	}
}

bool Player::isFarkle(vector<int> &roll) {

	pointHelper p{ 0, 0 };
	vector<pointHelper> vec;

	//Bucket sort dice
	for (size_t i = 1; i < 7; i++) {
		for (size_t j = 0; j < roll.size(); j++) {
			p.number = i;
			if (roll[j] == i) {
				p.count++;
			}
		}
		vec.push_back(p);
		p.count = 0;
	}

	//If there are any 1's or 5's, return false
	if (vec[0].count > 0 || vec[4].count > 0) {
		return false;
	}

	int numPairs = 0;

	for (int i = 0; i < vec.size(); i++) {

		//Check for 3 of a kind(any die)
		if (vec[i].count >= 3) {
			return false;
		}

		if (vec[i].count == 2) {
			numPairs++;
		}
	}

	//Check for three pairs
	if (numPairs == 3) {
		return false;
	}

	return true;

}
	

Human::Human() : Player() {}

Human::Human(string name_in) : Player(name_in) {}

int Human::getPoints(info& info_in) {

	int numDiceRemoved = 0;

	int points = 0;
	int tempPoints = 0;

	pointHelper p{ 0, 0 };

	vector<pointHelper> selectedDice;

	//Bucket sort numbers on dice given the roll
	for (size_t i = 1; i < 7; i++) {
		for (size_t j = 0; j < info_in.selected.size(); j++) {
			p.number = i;
			if (info_in.dice[info_in.selected[j] - 1] == i) {
				p.count++;
			}
		}
		selectedDice.push_back(p);
		p.count = 0;
	}

	//Check for straight before anything else
	bool straight = true;
	for (int i = 0; i < 6; i++) {
		if (selectedDice[i].count != 1) {
			straight = false;
		}
	}
	if (straight) {
		points += 1250;
		cout << "Straight!" << endl;
		info_in.selected.resize(0);
		return points;
	}

	int numSixOfKinds = 0;
	int numFiveOfKinds = 0;
	int numFourOfKinds = 0;
	int numThreeOfKinds = 0;
	int numPairs = 0;
	int numOnDie = 0;

	int numOnes = 0;
	int numFives = 0;

	bool threePair = false;

	//Loop through dice
	for (int i = 0; i < selectedDice.size(); i++) {

		//Check for 6 of a kind
		if (selectedDice[i].count == 6) {
			numSixOfKinds++;
			cout << "Six of a kind!" << endl;
			points += 3000;
			numDiceRemoved = 6;
			break;
		}

		//Check for 5 of a kind
		if (selectedDice[i].count == 5) {
			numFiveOfKinds++;
			cout << "Five of a kind!" << endl;
			points += 2000;
			numDiceRemoved = 5;
		}

		//Check for 4 of a kind
		if (selectedDice[i].count == 4) {
			numFourOfKinds++;
			cout << "Four of a kind!" << endl;
			points += 1500;
			numDiceRemoved = 4;
		}

		//Check for 3 of a kind
		if (selectedDice[i].count == 3) {

			//Keep track of number on die, and keep open possibility of double triples
			numThreeOfKinds++;
			numOnDie = selectedDice[i].number;
		}

		//Track 3 pairs
		if (selectedDice[i].count == 2) {
			numPairs++;
		}
	}

	//Double triples check
	if (numThreeOfKinds == 2) {
		cout << "Double triples!" << endl;
		numDiceRemoved = 6;
		info_in.selected.resize(0);
		return 1500;
	}

	//Single three of kind
	else if (numThreeOfKinds == 1) {
		cout << "Three of a kind!" << endl;
		if (numOnDie == 1) {
			points += 1000;
		}
		else {
			points += (numOnDie * 100);
		}

		numDiceRemoved = 3;
	}

	//Three pairs check
	else if (numPairs == 3) {
		cout << "Three pairs!" << endl;
		points += 1500;
		numDiceRemoved = 6;
		threePair = true;
	}

	//Add 1's only if they dont contribute to anything else
	if (selectedDice[0].count < 3 && !threePair) {
		numOnes += selectedDice[0].count;
		numDiceRemoved += numOnes;
		points += (100 * numOnes);
	}

	//Add 5's only if they dont contribute to anything else
	if (selectedDice[4].count < 3 && !threePair) {
		numFives += selectedDice[4].count;
		numDiceRemoved += numFives;
		points += (50 * numFives);
	}

	int initialDice = info_in.dice.size();

	//Resize dice for next roll
	info_in.dice.resize(initialDice - numDiceRemoved);

	//Clear selection
	info_in.selected.resize(0);

	return points;
}

void Human::isValidSelection(info& info_in) {

	string input = info_in.input;
	int numDice = int(info_in.dice.size());
	
	//Sanity check (saves headache)
	if (input[0] == ' ') {
		cout << "Invalid input, please do not enter a space before your input" << endl;
		info_in.goodInput = false;
		return;
	}
	
	//Letter inputted first. Probably excessive but still checking
	else if (!isdigit(input[0])) {
		cout << "Error, invalid input ";
		info_in.goodInput = false;
		return;
	}

	size_t length = input.length();
	for (size_t i = 0; i < length; i++) {

		char current = input[i];

		//Spaces in spots before numbers
		if ((i + 1) % 3 == 0) {
			if (current != ' ') {
				cout << "Invalid input. Spacing error" << endl;
				info_in.goodInput = false;
				info_in.selected.clear();
				return;
			}
		}

		//Check for numbers in the right spot
		else if (i % 3 == 0) {
			if (!isdigit(input[0])) {
				cout << "Error, invalid input" << endl;
				info_in.goodInput = false;
				info_in.selected.clear();
				return;
			}

			//User enters dice #6 when roll is only 5 dice
			else if ((current - '0' > numDice)) {
				cout << "Error, cannot bank die # " << current << endl;
				info_in.goodInput = false;
				info_in.selected.clear();
				return;
			}

			//Double dice check 
			else {
				size_t initialSize = info_in.selected.size();
				for (size_t j = 0; j < initialSize; j++) {
					if (current - '0' == info_in.selected[j]) {
						cout << "Error, you have selected the same die twice" << endl;
						info_in.goodInput = false;
						info_in.selected.clear();
						return;
					}
				}

				//If everything passes, push the dice into our selected vector
				info_in.selected.push_back(current - '0');
			}
		}

		//Check for commas after numbers
		else if ((i + 2) % 3 == 0) {
			if (current != ',') {
				cout << "Error, invalid input" << endl;
				info_in.goodInput = false;
				info_in.selected.clear();
				return;
			}
		}

		//Double check!
		else {
			cout << "Error, invalid input" << endl;
			info_in.goodInput = false;
			info_in.selected.clear();
			return;
		}
	}

	//Given our valid, selected dice, check how many points they are worth
	int pointsScored = getPoints(info_in);

	//Selecting dice worth 0 points is useless, and the user will need to reselect
	if (pointsScored == 0) {
		cout << "The selected dice do not generate any points! Please select new dice." << endl;
		info_in.goodInput = false;
		info_in.selected.clear();
		return;
	}

	//If everything passes, we're good to go
	info_in.goodInput = true;
	info_in.numPoints = pointsScored;
}

void Human::getDice(int leadingScore) {
	bool goodInput = false;
	bool bank = false;
	string input;
	bool stop = false;

	int currentTurnPoints = 0;

	info diceSelection{ "", 0, false };

	do {
		rollDice(diceSelection);
		printRoll(diceSelection.dice);

		stop = isFarkle(diceSelection.dice);

		//Check for farkle
		if (stop) {
			cout << "Farkle! You earn 0 points for this roll. " << endl << endl;
			stop = true;
		}

		else {
			do {
				cout << endl << "Current points this turn: " << currentTurnPoints << endl;

				cout << "Please enter the dice you wish to select: ";

				getline(cin, input);
				diceSelection.input = input;

				cout << endl;

				//Check selection
				isValidSelection(diceSelection);
				goodInput = diceSelection.goodInput;

				//Reprompt if bad
				if (!goodInput) {
					printRoll(diceSelection.dice);

				}
			} while (!goodInput);

			//Let user know how many current points their roll is
			cout << diceSelection.numPoints << " points scored." << endl;

			currentTurnPoints += diceSelection.numPoints;

			string bankInput;

			//If able to bank...
			if (currentTurnPoints >= 300) {

				//Check that the opponent has less than 10000, or that their roll surpasses the winner's score
				if (leadingScore < POINTSFORWIN || getScore() + currentTurnPoints > leadingScore) {

					do {

						cout << "Would you like to bank? Type \"Bank\" to bank " << currentTurnPoints << " points, or \"No\" to continue: ";
						getline(cin, bankInput);

						if (bankInput != "Bank" && bankInput != "bank" && bankInput != "No" && bankInput != "no") {
							cout << "Error, invalid input" << endl;
						}
					} while (bankInput != "Bank" && bankInput != "bank" && bankInput != "No" && bankInput != "no");

					//Bank points!
					if (bankInput == "Bank" || bankInput == "bank") {
						addPoints(currentTurnPoints);
						cout << getName() << " banked " << currentTurnPoints << "!" << endl << endl;
						stop = true;
					}
				}
			}
		}

		//Resize if empty
		if (diceSelection.dice.size() == 0) {
			diceSelection.dice.resize(6);
		}

		//Roll again if no farkle
		if (stop == false) {
			cout << "Rolling again..." << endl;
		}

	} while (!stop);

}


Computer::Computer() : Player("Computer") {}

void Computer::getDice(int leadingScore) {
	info info_in;

	bool stop = false;

	int currentTurnPoints = 0;

	do {

		/*
		**AI Thought process:
		**First line: Large amount of points in roll, minimize risk
		**Second line: Medium amount of points in roll, probably won't farkle with 3+ dice
		**Third line: bankable points, small dice. Minimize risk
		**Fourth line: Bank as soon as exceeding 10000
		*/
		if ((currentTurnPoints >= 2000 && info_in.dice.size() <= 4) ||
			(currentTurnPoints >= 1000 && info_in.dice.size() <= 3) ||
			(currentTurnPoints >= 300 && info_in.dice.size() <= 2) ||
			(getScore() + currentTurnPoints > POINTSFORWIN)) {

			//If opponent has 10000+, keep going until you exceed their value
			if (leadingScore < POINTSFORWIN || getScore() + currentTurnPoints > leadingScore) {

				cout << getName() << " banked " << currentTurnPoints << "!" << endl << endl;
				addPoints(currentTurnPoints);
				break;
			}
		}

		rollDice(info_in);
		printRoll(info_in.dice);

		cout << endl << "Current points this turn: " << currentTurnPoints << endl;

		//check for farkle
		stop = isFarkle(info_in.dice);

		if (stop) {
			cout << "Farkle! Computer earns 0 points for this roll. " << endl << endl;
			stop = true;
		}

		else {

			//Pause for better user experience
			cout << "Computer is thinking..." << endl << endl;
			chrono::seconds dura(2);
			this_thread::sleep_for(dura);

			makeDecision(info_in);

			currentTurnPoints += info_in.numPoints;
		}

		//Resize if 0 dice left
		if (info_in.dice.size() == 0) {
			info_in.dice.resize(6);
		}

		info_in.numPoints = 0;

	} while (!stop);
}

void Computer::printSelected(vector<int>& vec) {

	//Print the dice the computer selected in order
	cout << "Computer selected dice # ";
	sort(vec.begin(), vec.end());
	for (auto i : vec) {
		cout << i << " ";
	}
	cout << endl;
}

void Computer::makeDecision(info& info_in) {

	//Dice chosen based on number
	vector<int> selectedDice;

	//Dice chosen based on position of roll
	vector<int> store;

	pointHelper p{ 0, 0 };

	vector<pointHelper> buckets;

	//Bucket sort dice to make everything easier
	for (size_t i = 1; i < 7; i++) {
		for (size_t j = 0; j < info_in.dice.size(); j++) {
			p.number = i;
			if (info_in.dice[j] == i) {
				p.count++;
			}
		}
		buckets.push_back(p);
		p.count = 0;
	}

	//Keep track of what to print after selected dice
	string scoreMessage = "";

	//Check for straight immediately
	bool straight = true;
	for (int i = 0; i < 6; i++) {
		if (buckets[i].count != 1) {
			straight = false;
		}
	}
	if (straight) {
		info_in.numPoints = 1250;
		scoreMessage = "Straight!";
		
		//We can simply store all 6 dice in order because all 6 dice are kept.
		//This is done on a few occasions in this function. Pay close attention
		//To the vector that dice are stored in
		store = { 1,2,3,4,5,6 };
	}

	int numSixOfKinds = 0;
	int numFiveOfKinds = 0;
	int numFourOfKinds = 0;
	int numThreeOfKinds = 0;
	int numPairs = 0;
	int numOnDie = 0;

	int numOnes = 0;
	int numFives = 0;

	bool threePair = false;

	//A majority of this code is the same as the player decisions, 
	//Except this time, all 6 dice are analyzed instead of just the selected one
	//The computer collects all points possible and then banks their maximum score
	if (!straight) {
		for (int i = 0; i < buckets.size(); i++) {

			//Check for 6 of a kind
			if (buckets[i].count == 6) {
				numSixOfKinds++;
				scoreMessage = "Six of a kind!";
				info_in.numPoints += 3000;
				numOnDie = buckets[i].number;
				store = { 1,2,3,4,5,6 };
				break;
			}

			//Check for 5 of a kind
			if (buckets[i].count == 5) {
				numFiveOfKinds++;
				scoreMessage = "Five of a kind!";
				info_in.numPoints += 2000;

				//What number? Push it back 5 times
				numOnDie = buckets[i].number;
				for (int i = 0; i < 5; i++) {
					selectedDice.push_back(numOnDie);
				}

			}

			//Check for 4 of a kind, use same process as 5 of a kind
			if (buckets[i].count == 4) {
				numFourOfKinds++;
				scoreMessage = "Four of a kind!";
				info_in.numPoints += 1500;
				numOnDie = buckets[i].number;
				for (int i = 0; i < 4; i++) {
					selectedDice.push_back(numOnDie);
				}
			}

			//Keep track of 3 of a kinds
			if (buckets[i].count == 3) {
				numThreeOfKinds++;
				numOnDie = buckets[i].number;
			}

			//Watch out for 3 pair
			if (buckets[i].count == 2) {
				numPairs++;
			}
		}

		//Check for double triples
		if (numThreeOfKinds == 2) {
			scoreMessage = "Double Triples!";
			store = { 1,2,3,4,5,6 };
			info_in.numPoints += 1500;
		}

		//If only one type of 3 of a kind
		else if (numThreeOfKinds == 1) {
			scoreMessage = "Three of a kind!";
			if (numOnDie == 1) {
				info_in.numPoints += 1000;
			}
			else {
				info_in.numPoints += (numOnDie * 100);
			}

			//Push back the number on the die 3 times
			for (int i = 0; i < 3; i++) {
				selectedDice.push_back(numOnDie);
			}
		}

		//Three pairs check
		else if (numPairs == 3) {
			scoreMessage = "Three pairs!";
			info_in.numPoints += 1500;
			store = { 1,2,3,4,5,6 };
			threePair = true;
		}

		//Add 1's only if unused anywhere else
		if (buckets[0].count < 3 && !threePair) {
			numOnes += buckets[0].count;
			info_in.numPoints += (100 * numOnes);

			for (int i = 0; i < buckets[0].count; i++) {
				selectedDice.push_back(1);
			}
		}

		//Add 4's only if unused anywhere else
		if (buckets[4].count < 3 && !threePair) {
			numFives += buckets[4].count;
			info_in.numPoints += (50 * numFives);

			for (int i = 0; i < buckets[4].count; i++) {
				selectedDice.push_back(5);
			}
		}


		/*
		**This was by far the hardest function to logically visualize and to write.
		**The purpose of this function is to inform the human player
		**what dice were chosen by the computer.

		**Recall that selected dice has the numbers on the dice,
		**store has the position of the dice, and info_in.dice is the 
		**original roll itself
		*/

		//Loop through selected dice
		for (int i = 0; i < selectedDice.size(); i++) {

			//Loop through the original roll
			for (int j = 0; j < info_in.dice.size(); j++) {

				//If the number selected matches the number on the original roll...
				if (selectedDice[i] == info_in.dice[j]) {

					//Check to make sure it hasn't been added already
					if (find(store.begin(), store.end(), j + 1) == store.end() || store.empty()) {

						//Push back the dice position into store (add 1 for user experience)
						store.push_back(j + 1);

						//break the loop to save time
						break;
					}//if
				}//if
			}//for
		}//for
	}

	size_t initial_size = info_in.dice.size();

	//Print out the dice
	printSelected(store);

	//Print out the score message
	if (scoreMessage != "") {
		cout << scoreMessage << endl;
	}

	//Resize the roll for next turn
	info_in.dice.resize(initial_size - store.size());
}