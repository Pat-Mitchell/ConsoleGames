#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <stdio.h>

void gameBuffer(int, int);
void gameLoop(int, int, int[]);
void instantiateGrid(int&, int&);

// 2D grid that will function as the play area
char **grid;

HANDLE hWrite = GetStdHandle(STD_OUTPUT_HANDLE);

int Main() {
	int width, height;
	int playerPos[] = { 1,1 };

	instantiateGrid(width, height);
	system("cls");
	gameBuffer(width, height);

	while (1) {
		gameLoop(width, height, playerPos);
		gameBuffer(width, height);
	}
}

// Game Loop. Basically the controls
void gameLoop(int w, int h, int playerPos[]) {
	grid[playerPos[0]][playerPos[1]] = '1';
	if (kbhit()) {
		switch (getch()) {
		case 'w':
			if (playerPos[0] > 1) {
				grid[playerPos[0]][playerPos[1]] = ' ';
				playerPos[0]--;
			}
			break;
		case 's':
			if (playerPos[0] < h - 2) {
				grid[playerPos[0]][playerPos[1]] = ' ';
				playerPos[0]++;
			}
			break;
		case 'd':
			if (playerPos[1] < w - 2) {
				grid[playerPos[0]][playerPos[1]] = ' ';
				playerPos[1]++;
			}
			break;
		case 'a':
			if (playerPos[1] > 1) {
				grid[playerPos[0]][playerPos[1]] = ' ';
				playerPos[1]--;
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
			if (grid[i][j] == 'X') {
				consoleBuffer[j + w * i].Char.AsciiChar = 0xDB;
				consoleBuffer[j + w * i].Attributes = 15;
				continue;
			}
			if (grid[i][j] == '1') {
				consoleBuffer[j + w * i].Char.AsciiChar = 0x01;
				consoleBuffer[j + w * i].Attributes = 15;
				continue;
			}
			consoleBuffer[j + w * i].Char.AsciiChar = 0x00;
			consoleBuffer[j + w * i].Attributes = 15;
		}
	}

	// Write character buffer to the console buffer
	WriteConsoleOutputA(hWrite, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
	delete[] consoleBuffer;
}

// Create a user defined grid
void instantiateGrid(int& w, int& h) {
	/// This was cool at first, but got slowly more annoying
	/// as I kept adding to the program.
	/// Commenting it out and filling it with default values.
	// Get the width and the height from the user
	//std::cout << "Enter width x height of play area: " << std::endl;
	//std::cin >> w;
	//std::cin >> h;

	h = 30; w = 60;

	// Set the height
	grid = new char*[h];

	// Set the width of each row
	// Also creates a grid of ' ' surrounded by 'X'
	for (int i = 0; i < h; i++) {
		grid[i] = new char[w];
		for (int j = 0; j < w; j++) {
			if (j == 0 || j == w - 1 || i == 0 || i == h - 1)
				grid[i][j] = 'X';
			else
				grid[i][j] = ' ';
		}
	}
}