#ifndef PLAYER_H
#define PLAYER_H

#include <string>

using namespace std;

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

	virtual void getDice(vector<int>& roll_in) = 0;
};

class Human : public Player {

public:

	Human() : Player(){}

	Human(string name_in) : Player(name_in) {}

	virtual void getDice(vector<int>& roll_in) {

	
	}

};

class Computer : public Player {
public:

	Computer() : Player("Computer") {}

	virtual void getDice(vector<int>& roll_in){}
};




#endif PLAYER_H

