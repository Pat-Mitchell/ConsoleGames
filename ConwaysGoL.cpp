/*
Conway's game of life
Expressed in the text console
Wanted practice using Windows.h

The rules state that the board is infinite, but I don't have infinite memory
or a cpu fast enough to calculate infinity in a reasonable amount of time.
Forgive me for not using an infinite plain or simulating an infinite plain
by wrapping the game board.

Probably not the most efficient, but that was not the point
It displays right. That's all I wanted to accomplish.

Created and finished on July 2, 2020
Patrick Mitchell
*/

#include <Windows.h>
#include <stdio.h>

#define WIDTH 40
#define HEIGHT 20

void gameBuffer();
void gameLoop();
void checkLife(int, int);

// Initialize the play area
// Requires two grids
// since I need a place to store the update for the next frame
char game[20][41] =                           
{ "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX", 
  "X......................................X", 
  "X......................................X", 
  "X......................................X", 
  "X......................................X", 
  "X......................................X", 
  "X......................................X", 
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" };

char gameNew[20][41] =
{ "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "X......................................X",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" };

// Write output handle
HANDLE hWrite = GetStdHandle(STD_OUTPUT_HANDLE);

int main() {
	// Window size coordinates
	SMALL_RECT windowSize = { 0, 0, WIDTH - 1, HEIGHT - 1 };
	//Cute Title
	SetConsoleTitle(L"Conway's Game of Life!");

	// Set the window size
	SetConsoleWindowInfo(hWrite, TRUE, &windowSize);

	// Initiate the first blank frame
	gameBuffer();

	// Fill the grid randomly
	srand(0);
	for (int i = 1; i < HEIGHT - 1; i++)
	{
		for (int j = 1; j < WIDTH - 1; j++)
		{
			if (rand() % 100 >= 70)
				game[i][j] = 'o';
		}
	}

	//Commence the game loop!
	while (1){
		Sleep(100);
		gameLoop();
		gameBuffer();

		// Copy the new board onto the old one
		for (int i = 1; i < HEIGHT - 1; i++)
		{
			for (int j = 1; j < WIDTH - 1; j++)
			{
				game[i][j] = gameNew[i][j];
			}
		}
	}
	getchar();
}

void gameLoop() {
	// Rule 0: Every cell has 8 neighbors
	// Rule 1: Any live cell with fewer than two live neighbors dies
	// Rule 2: Any live cell with two or three live neighbors lives
	// Rule 3: Any live cell with more than three live neighbors dies
	// Rule 4: Any dead cell with exactly 3 neighbors becomes a live cell
	for (int i = 1; i < HEIGHT - 1; i++) {
		for (int j = 1; j < WIDTH - 1; j++) {
			checkLife(i, j);
		}
	}
}

// Holy O(n^2). Not O(n^4)
// Reasoning: checklife has a guaranteed finite length of 9 checks
//		gameloop's nested for loops is dependant on the size of the board
// Function just for checking the cell's life status.
void checkLife(int a, int b) {
	char check = ' ';
	if (game[a][b] == 'o') {
		check = 'o';
	}
	int neighbors = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) continue;
			if (game[a + i][b + j] == 'o') neighbors++;
		}
	}
	if (game[a][b] == '.' && neighbors == 3) gameNew[a][b] = 'o';
	else if (game[a][b] == 'o' && (neighbors == 3 || neighbors ==2)) gameNew[a][b] = 'o';
	else gameNew[a][b] = '.';
}

void gameBuffer() {

	// Screen buffer dimensions
	COORD bufferSize = { WIDTH, HEIGHT };

	// Setting up different variables for passing to WriteConsoleOutput
	COORD characterBufferSize = { WIDTH, HEIGHT };
	COORD characterPosition = { 0, 0 };
	SMALL_RECT consoleWriteArea = { 0, 0, WIDTH - 1, HEIGHT - 1 };

	// A CHAR_INFO structure containing dara about a single character
	CHAR_INFO consoleBuffer[WIDTH * HEIGHT];

	// Set the screen buffer size
	SetConsoleScreenBufferSize(hWrite, bufferSize);

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (game[i][j] == 'X') {
				consoleBuffer[j + WIDTH * i].Char.AsciiChar = 0xDB;
				consoleBuffer[j + WIDTH * i].Attributes = 15;
				continue;
			}
			if (game[i][j] == 'o') {
				consoleBuffer[j + WIDTH * i].Char.AsciiChar = 0x2B;
				consoleBuffer[j + WIDTH * i].Attributes = 15;
				continue;
			}
			consoleBuffer[j + WIDTH * i].Char.AsciiChar = 0x2E;
			consoleBuffer[j + WIDTH * i].Attributes = 15;
		}
	}

	// Write character buffer to the console buffer
	WriteConsoleOutputA(hWrite, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);

}