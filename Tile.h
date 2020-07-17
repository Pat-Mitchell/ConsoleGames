#pragma once

#include <string>
#include <vector>

class Tile {
private:
	int position[2];
	int symbol = 0;
	std::string name = "<undefined>";
public:
	void setX(int x) {
		this->position[0] = x;
	}
	void setY(int y) {
		this->position[1] = y;
	}
	int getX() {
		return this->position[0];
	}
	int getY() {
		return this->position[1];
	}
	void setSymbol(char s) {
		this->symbol = s;
	}
	int getSymbol() {
		return this->symbol;
	}
	void setName(std::string name) {
		if (name == "")
			this->name = "IMError";
		else
			this->name = name;
	}
	std::string getName() {
		return this->name;
	}
};

class Wall : public Tile {
public:
	Wall() {
		setName("Wall");
		setSymbol(219);
	}
};

class Floor : public Tile {
public:
	Floor() {
		setName("Floor");
		setSymbol(00);
	}
};

class Tail : public Tile {
public:
	Tail() {
		setName("Tail");
		setSymbol(02);
	}
};

class Player : public Tile {
public:
	std::vector<Tail> tail;
	Player(int x, int y) {
		setName("Player");
		setSymbol(01);
		setX(x);
		setY(y);
	}

	void insertTail(Tail tail) {
		this->tail.push_back(tail);
	}

	void follow() {
		int prevX = tail.at(0).getX(), prevY = tail.at(0).getY();
		int prev2X, prev2Y;

		tail.at(0).setX(getX()); tail.at(0).setY(getY());


		for (int i = 1; i < tail.size(); i++) {
			prev2X = tail.at(i).getX();
			prev2Y = tail.at(i).getY();
			tail.at(i).setX(prevX);
			tail.at(i).setY(prevY);
			prevX = prev2X;
			prevY = prev2Y;
		}
	}
	
};