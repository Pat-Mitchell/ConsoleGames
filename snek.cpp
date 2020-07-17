#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <ctime>

#include "Tile.h"

class Fruit : public Tile {
public:
	Fruit() : Tile() {
		setName("Fruit");
		setSymbol(0x2B);
	}
};

void gameLoop(int, int, Player&, char&, bool&);
void gameBuffer(int, int);
void instantiateGrid(int&, int&);
void fruitSpawn(int, int);
bool ateFruit(Player&, int, int);

Tile **grid;
HANDLE hWrite = GetStdHandle(STD_OUTPUT_HANDLE);

int main() {
	std::srand(std::time(nullptr));
	int width = 60, height = 30;
	char dir = 'r';
	bool gameover = false;

	instantiateGrid(width, height);
	Player player(1, 1);
	grid[player.getY()][player.getX()] = player;
	fruitSpawn(width, height);
	while (!gameover) {
		Sleep(100);
		gameLoop(width, height, player, dir, gameover);
	}

	for (int i = 0; i < 30; i++) {
		std::cout << std::endl;
	}
}

// Put a fruit somewhere in the play area
void fruitSpawn(int width, int height)
{
	int fruitX = (rand() % (width - 2)) + 1;
	int fruitY = (rand() % (height - 2)) + 1;
	grid[fruitY][fruitX] = Fruit();
}

// See if the player ate a fruit
bool ateFruit(Player& p, int prevY, int prevX) {
	Tail newTail;
	if (grid[p.getY()][p.getX()].getName() == "Fruit") {
		newTail.setX(prevX);
		newTail.setY(prevY);
		p.insertTail(newTail);
		return true;
	}
	return false;
}

void gameLoop(int w, int h, Player& p, char& dir, bool& go) {
	Tile* nextTile = NULL;

	if (kbhit()) {
		switch (getch()) {
		case 'w':
			if(dir != 'd')
				dir = 'u';
			break;
		case 's':
			if (dir != 'u')
				dir = 'd';
			break;
		case 'd':
			if (dir != 'l')
				dir = 'r';
			break;
		case 'a':
			if (dir != 'r')
				dir = 'l';
			break;
		}
	}

	switch (dir) {
	case 'u':
		if (p.getY() > 0) {
			grid[p.getY()][p.getX()] = Floor();
			if (!p.tail.empty()) {
				if (grid[p.tail.at(p.tail.size() - 1).getY()][p.tail.at(p.tail.size() - 1).getX()].getName() != "Fruit")
					grid[p.tail.at(p.tail.size() - 1).getY()][p.tail.at(p.tail.size() - 1).getX()] = Floor();
				p.follow();
				for (Tile t : p.tail) {
					if (grid[t.getY()][t.getX()].getName() != "Fruit")
						grid[t.getY()][t.getX()] = t;
				}
			}
			p.setY(p.getY() - 1);
			if (ateFruit(p, p.getY() + 1, p.getX()))
				fruitSpawn(w, h);
			if (grid[p.getY()][p.getX()].getName() == "Wall" || grid[p.getY()][p.getX()].getName() == "Tail")
				go = true;
			grid[p.getY()][p.getX()] = p;
		}
		break;
	case 'd':
		if (p.getY() < h - 1) {
			grid[p.getY()][p.getX()] = Floor();
			if (!p.tail.empty()) {
				if (grid[p.tail.at(p.tail.size() - 1).getY()][p.tail.at(p.tail.size() - 1).getX()].getName() != "Fruit")
					grid[p.tail.at(p.tail.size() - 1).getY()][p.tail.at(p.tail.size() - 1).getX()] = Floor();
				p.follow();
				for (Tile t : p.tail) {
					if (grid[t.getY()][t.getX()].getName() != "Fruit")
						grid[t.getY()][t.getX()] = t;
				}
			}
			p.setY(p.getY() + 1);
			if (ateFruit(p, p.getY() - 1, p.getX())) fruitSpawn(w, h);
			if (grid[p.getY()][p.getX()].getName() == "Wall" || grid[p.getY()][p.getX()].getName() == "Tail")
				go = true;
			grid[p.getY()][p.getX()] = p;
		}
		break;
	case 'r':
		if (p.getX() < w - 1) {
			grid[p.getY()][p.getX()] = Floor();
			if (!p.tail.empty()) {
				if (grid[p.tail.at(p.tail.size() - 1).getY()][p.tail.at(p.tail.size() - 1).getX()].getName() != "Fruit")
					grid[p.tail.at(p.tail.size() - 1).getY()][p.tail.at(p.tail.size() - 1).getX()] = Floor();
				p.follow();
				for (Tile t : p.tail) {
					if (grid[t.getY()][t.getX()].getName() != "Fruit")
						grid[t.getY()][t.getX()] = t;
				}
			}
			p.setX(p.getX() + 1);
			if (ateFruit(p, p.getY(), p.getX() - 1)) fruitSpawn(w, h);
			if (grid[p.getY()][p.getX()].getName() == "Wall" || grid[p.getY()][p.getX()].getName() == "Tail")
				go = true;
			grid[p.getY()][p.getX()] = p;
		}
		break;
	case 'l':
		if (p.getX() > 0) {
			grid[p.getY()][p.getX()] = Floor();
			if (!p.tail.empty()) {
				if (grid[p.tail.at(p.tail.size() - 1).getY()][p.tail.at(p.tail.size() - 1).getX()].getName() != "Fruit")
					grid[p.tail.at(p.tail.size() - 1).getY()][p.tail.at(p.tail.size() - 1).getX()] = Floor();
				p.follow();
				for (Tile t : p.tail) {
					if (grid[t.getY()][t.getX()].getName() != "Fruit")
						grid[t.getY()][t.getX()] = t;
				}
			}
			p.setX(p.getX() - 1);
			if (ateFruit(p, p.getY(), p.getX() + 1)) fruitSpawn(w, h);
			if (grid[p.getY()][p.getX()].getName() == "Wall" || grid[p.getY()][p.getX()].getName() == "Tail")
				go = true;
			grid[p.getY()][p.getX()] = p;
		}
		break;
	}

	gameBuffer(w, h);
}

// the buffer
void gameBuffer(int w, int h) {
	// Screen buffer dimenstions
	COORD bufferSize = { w, h };

	// Setting up different variables for passing to WriteConsoleOutput
	COORD characterBufferSize = { w, h };
	COORD characterPosition = { 0,0 };
	SMALL_RECT consoleWriteArea = { 0, 0, w - 1, h - 1 };

	// A CHAR_INFO structure containing data about a single character
	CHAR_INFO* consoleBuffer = new CHAR_INFO[w * h];

	// Set the screen buffer size
	SetConsoleScreenBufferSize(hWrite, bufferSize);

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			consoleBuffer[j + w * i].Char.AsciiChar = grid[i][j].getSymbol();
			consoleBuffer[j + w * i].Attributes = 15;
		}
	}

	// Write character buffer to the console buffer
	WriteConsoleOutputA(hWrite, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
	delete[] consoleBuffer;
}

// Create a grid
void instantiateGrid(int& w, int& h) {

	h = 30; w = 60;

	// Set the height
	grid = new Tile*[h];

	// Set the width of each row
	// Also creates a grid of ' ' surrounded by 'X'
	for (int i = 0; i < h; i++) {
		grid[i] = new Tile[w];
		for (int j = 0; j < w; j++) {
			if (j == 0 || j == w - 1 || i == 0 || i == h - 1) {
				grid[i][j] = Wall();
			}
			else {
				grid[i][j] = Floor();
			}
		}
	}
}