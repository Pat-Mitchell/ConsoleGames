#include <Windows.h>
#include <iostream>
#include <conio.h>
#include "Tile.h"

void gameLoop(int, int, Player&);
void gameBuffer(int, int);
void instantiateGrid(int&, int&);

Tile **grid;
Tile tFloor, tWall;
HANDLE hWrite = GetStdHandle(STD_OUTPUT_HANDLE);

int main() {
	tFloor.setName("Floor");
	tFloor.setSymbol(0);
	tWall.setName("Wall");
	tWall.setSymbol(219);

	int width = 60, height = 30;
	instantiateGrid(width, height);
	Player player(1, 1);

	grid[player.getY()][player.getX()] = player;
	while (1) {
		gameBuffer(width, height);
		gameLoop(width, height, player);
	}
}

// Game Loop. Basically the controls
void gameLoop(int w, int h, Player& p) {
	if (kbhit()) {
		switch (getch()) {
		case 'w':
			if (p.getY() > 1) {
				grid[p.getY()][p.getX()] = tFloor;
				p.setY(p.getY() - 1);
				grid[p.getY()][p.getX()] = p;
			}
			break;
		case 's':
			if (p.getY() < h - 2) {
				grid[p.getY()][p.getX()] = tFloor;
				p.setY(p.getY() + 1);
				grid[p.getY()][p.getX()] = p;
			}
			break;
		case 'd':
			if (p.getX() < w - 2) {
				grid[p.getY()][p.getX()] = tFloor;
				p.setX(p.getX() + 1);
				grid[p.getY()][p.getX()] = p;
			}
			break;
		case 'a':
			if (p.getX() > 1) {
				grid[p.getY()][p.getX()] = tFloor;
				p.setX(p.getX() - 1);
				grid[p.getY()][p.getX()] = p;
			}
			break;
		}
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
				grid[i][j] = tWall;
			}
			else {
				grid[i][j] = tFloor;
			}
		}
	}
}