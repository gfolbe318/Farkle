#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <algorithm>
#include <deque>

using namespace std;

struct info {
	vector<int> dice;
	vector<int> selected;
	string input;
	int numPoints;
	bool goodInput;

	info()
		:dice(6, 0), numPoints(0) {}

	info(string input_in, int numPoints_in, bool goodInput_in)
		:dice(6, 0), input(input_in), numPoints(numPoints_in), goodInput(goodInput_in) {}
};

struct pointHelper {
	int number;
	int count;
};


class Player {
protected:
	string name;
	int points;

public:

	Player()
		:name(""), points(0) {}

	Player(string name_in)
		:name(name_in), points(0) {}

	string getName() {
		return name;
	}

	int getScore() {
		return points;
	}

	void addPoints(int points_in) {
		points += points_in;
	}

	void printRoll(vector<int>& roll) {


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

	void rollDice(info& info_in) {

		for (size_t i = 0; i < info_in.dice.size(); i++) {
			info_in.dice[i] = (rand() % 6) + 1;
		}
	}

	bool isFarkle(vector<int> &roll) {

		pointHelper p{ 0, 0 };
		vector<pointHelper> vec;

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

		if (vec[0].count > 0 || vec[4].count > 0) {
			return false;
		}


		int numFiveOfKinds = 0;
		int numFourOfKinds = 0;
		int numThreeOfKinds = 0;
		int numPairs = 0;
		int numOnDie = 0;

		int points = 0;

		for (int i = 0; i < vec.size(); i++) {

			if (vec[i].count == 5) {
				return false;
			}

			if (vec[i].count == 4) {
				return false;
			}

			if (vec[i].count == 3) {
				return false;
			}

			if (vec[i].count == 2) {
				numPairs++;
			}
		}

		if (numPairs == 3) {
			return false;
		}

		return true;

	}

	virtual void getDice() = 0;
};

class Human : public Player {

public:

	Human() : Player(){}

	Human(string name_in) : Player(name_in) {}
	
	int getPoints(info& info_in) {

		int numDiceRemoved = 0;

		int points = 0;
		int tempPoints = 0;

		pointHelper p{ 0, 0 };

		vector<pointHelper> selectedDice;

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

		int numFiveOfKinds = 0;
		int numFourOfKinds = 0;
		int numThreeOfKinds = 0;
		int numPairs = 0;
		int numOnDie = 0;

		int numOnes = 0;
		int numFives = 0;

		bool threePair = false;

		for (int i = 0; i < selectedDice.size(); i++) {

			if (selectedDice[i].count == 5) {
				numFiveOfKinds++;
				numOnDie = selectedDice[i].number;
				cout << "Five of a kind!" << endl;
				points += 2000;
				numDiceRemoved = 5;
			}

			if (selectedDice[i].count == 4) {
				numFourOfKinds++;
				numOnDie = selectedDice[i].number;
				cout << "Four of a kind!" << endl;
				points += 1500;
				numDiceRemoved = 4;
			}

			if (selectedDice[i].count == 3) {
				numThreeOfKinds++;
				numOnDie = selectedDice[i].number;
			}

			if (selectedDice[i].count == 2) {
				numPairs++;
			}
		}

		if (numThreeOfKinds == 2) {
			cout << "Double triples!" << endl;
			numDiceRemoved = 6;
			info_in.selected.resize(0);
			return 1500;
		}

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

		else if (numPairs == 3) {
			cout << "Three pairs!" << endl;
			points += 1500;
			numDiceRemoved = 6;
			threePair = true;
		}

		if (selectedDice[0].count < 3 && !threePair) {
			numOnes += selectedDice[0].count;
			numDiceRemoved += numOnes;
			points += (100 * numOnes);
		}

		if (selectedDice[4].count < 3 && !threePair) {
			numFives += selectedDice[4].count;
			numDiceRemoved += numFives;
			points += (50 * numFives);
		}

		int initialDice = info_in.dice.size();
		info_in.dice.resize(initialDice - numDiceRemoved);

		info_in.selected.resize(0);

		return points;
	}

	void isValidSelection(info& info_in) {

		string input = info_in.input;
		int numDice = int(info_in.dice.size());


		if (input[0] == ' ') {
			cout << "Invalid input, please do not enter a space before your input" << endl;
			info_in.goodInput = false;
			return;
		}


		else if (!isdigit(input[0])) {
			cout << "Error, invalid input " << input[0] << endl;
			info_in.goodInput = false;
			return;
		}

		size_t length = input.length();
		for (size_t i = 0; i < length; i++) {

			char current = input[i];

			if ((i + 1) % 3 == 0) {
				if (current != ' ') {
					cout << "Invalid input. Spacing error" << endl;
					info_in.goodInput = false;
					info_in.selected.clear();
					return;
				}
			}
			else if (i % 3 == 0) {
				if (!isdigit(input[0])) {
					cout << "Error, invalid input" << endl;
					info_in.goodInput = false;
					info_in.selected.clear();
					return;
				}
				else if ((current - '0' > numDice)) {
					cout << "Error, cannot bank die # " << current << endl;
					info_in.goodInput = false;
					info_in.selected.clear();
					return;
				}
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
					info_in.selected.push_back(current - '0');
				}
			}

			else if ((i + 2) % 3 == 0) {
				if (current != ',') {
					cout << "Error, invalid input" << endl;
					info_in.goodInput = false;
					info_in.selected.clear();
					return;
				}
			}
			else {
				cout << "Error, invalid input" << endl;
				info_in.goodInput = false;
				info_in.selected.clear();
				return;
			}
		}

		int pointsScored = getPoints(info_in);

		if (pointsScored == 0) {
			cout << "The selected dice do not generate any points! Please select new dice." << endl;
			info_in.goodInput = false;
			info_in.selected.clear();
			return;
		}

		info_in.goodInput = true;
		info_in.numPoints = pointsScored;
	}

	virtual void getDice() {
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

					isValidSelection(diceSelection);
					goodInput = diceSelection.goodInput;

					if (!goodInput) {
						printRoll(diceSelection.dice);

					}
				} while (!goodInput);

				cout << diceSelection.numPoints << " points scored." << endl;
				currentTurnPoints += diceSelection.numPoints;

				string bankInput;

				if (currentTurnPoints >= 300) {

					do {

						cout << "Would you like to bank? Type \"Bank\" to bank " << currentTurnPoints << " points, or \"No\" to continue: ";
						getline(cin, bankInput);

						if (bankInput != "Bank" && bankInput != "bank" && bankInput != "No" && bankInput != "no") {
							cout << "Error, invalid input" << endl;
						}
					} while (bankInput != "Bank" && bankInput != "bank" && bankInput != "No" && bankInput != "no");

					if (bankInput == "Bank" || bankInput == "bank") {
						addPoints(currentTurnPoints);
						cout << getName() << " banked " << currentTurnPoints << "!" << endl << endl;
						stop = true;
					}
				}
			}

			if (diceSelection.dice.size() == 0) {
				diceSelection.dice.resize(6);
			}

			if (stop == false) {
				cout << "Rolling again..." << endl;
			}

		} while (!stop);
	
	}

};

class Computer : public Player {

private:

public:

	Computer() : Player("Computer") {}

	virtual void getDice() {
		info info_in;

		bool stop = false;

		int currentTurnPoints = 0;

		do {

			if ((currentTurnPoints > 1000 && info_in.dice.size() <= 3) ||
				(currentTurnPoints > 300 && info_in.dice.size() <= 2)) {

				cout << getName() << " banked " << currentTurnPoints << "!" << endl << endl;
				addPoints(currentTurnPoints);
				break;
			}

			rollDice(info_in);
			printRoll(info_in.dice);

			cout << endl << "Current points this turn: " << currentTurnPoints << endl;

			stop = isFarkle(info_in.dice);

			if (stop) {
				cout << "Farkle! Computer earns 0 points for this roll. " << endl << endl;
				stop = true;
			}

			else {
				makeDecision(info_in);

				currentTurnPoints += info_in.numPoints;
			}

			if (info_in.dice.size() == 0) {
				info_in.dice.resize(6);
			}

			info_in.numPoints = 0;

		} while (!stop);
	}

	void printSelected(vector<int>& vec) {
		cout << "Computer selected dice # ";
		sort(vec.begin(), vec.end());
		for (auto i : vec) {
			cout << i << " ";
		}
		cout << endl;
	}

	void makeDecision(info& info_in) {

		vector<int> selectedDice;
		vector<int> store;

		pointHelper p{ 0, 0 };

		vector<pointHelper> buckets;

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

		string scoreMessage = "";

		bool straight = true;
		for (int i = 0; i < 6; i++) {
			if (buckets[i].count != 1) {
				straight = false;
			}
		}
		if (straight) {
			info_in.numPoints = 1250;
			scoreMessage = "Straight!";
			store = { 1,2,3,4,5,6 };
		}

		int numFiveOfKinds = 0;
		int numFourOfKinds = 0;
		int numThreeOfKinds = 0;
		int numPairs = 0;
		int numOnDie = 0;

		int numOnes = 0;
		int numFives = 0;

		bool threePair = false;

		if (!straight) {
			for (int i = 0; i < buckets.size(); i++) {

				if (buckets[i].count == 5) {
					numFiveOfKinds++;
					numOnDie = buckets[i].number;
					scoreMessage = "Five of a kind!";
					info_in.numPoints += 2000;

					for (int i = 0; i < 5; i++) {
						selectedDice.push_back(numOnDie);
					}

				}

				if (buckets[i].count == 4) {
					numFourOfKinds++;
					numOnDie = buckets[i].number;
					scoreMessage = "Four of a kind!";
					info_in.numPoints += 1500;

					for (int i = 0; i < 4; i++) {
						selectedDice.push_back(numOnDie);
					}
				}

				if (buckets[i].count == 3) {
					numThreeOfKinds++;
					numOnDie = buckets[i].number;
				}

				if (buckets[i].count == 2) {
					numPairs++;
				}
			}

			if (numThreeOfKinds == 2) {
				scoreMessage = "Double Triples!";
				store = { 1,2,3,4,5,6 };
				info_in.numPoints += 1500;
			}

			else if (numThreeOfKinds == 1) {
				scoreMessage = "Three of a kind!";
				if (numOnDie == 1) {
					info_in.numPoints += 1000;
				}
				else {
					info_in.numPoints += (numOnDie * 100);
				}

				for (int i = 0; i < 3; i++) {
					selectedDice.push_back(numOnDie);
				}
			}

			else if (numPairs == 3) {
				scoreMessage = "Three pairs!";
				info_in.numPoints += 1500;
				store = { 1,2,3,4,5,6 };
				threePair = true;
			}

			if (buckets[0].count < 3 && !threePair) {
				numOnes += buckets[0].count;
				info_in.numPoints += (100 * numOnes);

				for (int i = 0; i < buckets[0].count; i++) {
					selectedDice.push_back(1);
				}
			}

			if (buckets[4].count < 3 && !threePair) {
				numFives += buckets[4].count;
				info_in.numPoints += (50 * numFives);

				for (int i = 0; i < buckets[4].count; i++) {
					selectedDice.push_back(5);
				}
			}

			bool hasBeenSeen = false;

			for (int i = 0; i < selectedDice.size(); i++) {
				for (int j = 0; j < info_in.dice.size(); j++) {
					if (selectedDice[i] == info_in.dice[j]) {
						if (find(store.begin(), store.end(), j + 1) == store.end() || store.empty()) {
							store.push_back(j + 1);
							break;
						}
					}
				}
			}
		}


		size_t initial_size = info_in.dice.size();

		printSelected(store);

		if (scoreMessage != "") {
			cout << scoreMessage << endl;
		}

		info_in.dice.resize(initial_size - store.size());
	}

};
#endif PLAYER_H