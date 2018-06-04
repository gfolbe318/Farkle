#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

using namespace std;

class Player {
protected:
	string name;
	string style;
	int points;
	vector<int> roll;

public:
	Player()
		:name(""), style(""), points(0) {}

	Player(string name_in, string style_in)
		:name(name_in), style(style_in), points(0) {}

	string getName() {
		return name;
	}

	int getScore() {
		return points;
	}

	void roll() {
	}

	virtual void 


};


#endif PLAYER_H

