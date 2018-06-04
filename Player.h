#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class Player {
protected:
	string name;
	int points;
	vector<int> roll;

public:

	Player()
		:name(""), points(0), roll(6, 0) {}

	Player(string name_in)
		:name(name_in), points(0), roll(6, 0) {}

	string getName() {
		return name;
	}

	int getScore() {
		return points;
	}

	void rollDice() {
		int x;
		for (int i = 0; i < 6; i++) {
			x = (rand() % 6) + 1;
			roll[i] = x;
		}
	}

	virtual void getDice() = 0;
};

class Human : public Player {

public:

	Human() : Player(){}

	Human(string name_in) : Player(name_in) {}

	virtual void getDice() {

	
	}

};

class Computer : public Player {
public:

	Computer() : Player("Computer") {}

	virtual void getDice(){}
};




#endif PLAYER_H

