#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <algorithm>
#include <deque>
#include <chrono>
#include <thread>
#include <vector>

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

using namespace std;

const int POINTSFORWIN = 10000;

class Player {
private:
	string name;
	int points;

public:

	Player();

	Player(string name_in);

	string getName();

	int getScore();

	void addPoints(int points_in);

	void printRoll(vector<int>& roll);

	void rollDice(info& info_in);

	bool isFarkle(vector<int> &roll);

	virtual void getDice(int leadingScore) = 0;
};

class Human : public Player {

public:

	Human();

	Human(string name_in);
	
	int getPoints(info& info_in);

	void isValidSelection(info& info_in);

	virtual void getDice(int leadingScore);

};

class Computer : public Player {

public:

	Computer();

	virtual void getDice(int leadingScore);

	void printSelected(vector<int>& vec);

	void makeDecision(info& info_in);

};
#endif PLAYER_H