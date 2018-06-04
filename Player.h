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
	string style;
	int points;
	vector<int> roll;

public:

	Player()
		:name(""), style(""), points(0), roll(6, 0) {}

	Player(string name_in, string style_in)
		:name(name_in), style(style_in), points(0), roll(6, 0) {}

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

	virtual void getDice(string& input) = 0;
};

class Human : public Player {

public:

	Human() : Player() {}

	Human(string name_in) : Player(name_in, "human") {}

	virtual void getDice(string& input) {

		
		
	}

};




#endif PLAYER_H

