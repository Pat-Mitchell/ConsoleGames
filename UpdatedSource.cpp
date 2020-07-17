/*
* ASCII Showdown
* Found this game as an example of using windows.h
* There's some compatability issue with my computer.
* So I'm altering it.
* Patrick Mitchell. July 2020
*/

#include<iostream>
#include<windows.h>
#include<time.h>
#include<conio.h>

#define WIDTH 40
#define HEIGHT 20

char grid[20][41] =                           // arena
{ "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX", // X: wall
  "X          c                           X", // c: cpu
  "X                                      X", // u: user
  "X                                      X", // b: user's bullet
  "X                                      X", // B: cpu's bullet
  "X                                      X", // boomerang(array): boomerang
  "X                                      X", // wall(array): user's wall
  "X                                      X",
  "X                                      X",
  "X                                      X",
  "X                                      X",
  "X                                      X",
  "X                                      X",
  "X                                      X",
  "X                                      X",
  "X                                      X",
  "X                                      X",
  "X                                      X",
  "X           u                          X",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" };

HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE); /* write (output) handle */
HANDLE rHnd = GetStdHandle(STD_INPUT_HANDLE); /* read (input) handle */

// function prototyping
void controls();
void printBuffer();
void win(int);
void printborder(int, int, COORD, int);
void score(int, int, int, int, int, int, int);
void printScoreBoard();
void screenflash();

// main function
int main() {
	int upos = 11, cpos = 11, cmove, uHealth = 10, cHealth = 40;
	int bullets = 500, nBoomerang = 20, nWall = 50, nBoost = 1;
	int boomerang[4] = { 188,187,201,200 }, wall[4] = { 219,178,177,176 };
	int winner = 0, boost = 0;
	clock_t start = -1, reloadtime = -1;

	SetConsoleTitle(L"ASCII Showdown");
	controls();
	system("cls");

	// Prints the area and actors
	printBuffer();
	printScoreBoard();

	// Read from the keyboard
	// Bad practice, but it's a part of the tutorial
	// Better to have a second thread handling the buffer.
	// That way inputs aren't ignored when more complex processes
	// Eat CPU 
	getch();

	srand(time(NULL));  //seeds random numbers using system time

	//Game loop
	while (1)
	{
		///user's turn
		if (kbhit()) ///listens if user pressed a key
		{
			switch (getch()) ///takes the key input
			{
			case 'w':
				if (bullets > 0)
				{
					if (boost == 0)
					{
						grid[17][upos] = 'b';           ///fires a bullet
						bullets--;
					}
					else
					{
						if (bullets > 2)
						{
							for (int i = -1; i <= 1; i++)
							{
								if (upos + i > 0 && upos + i < 39)
								{
									grid[17][upos + i] = 'b';          ///fires 3 bullets via weapon boost
									bullets--;
								}
							}
						}
					}
				}
				break;
			case 'a':              ///moves user left
				if (upos > 1)
				{
					grid[18][upos] = ' ';
					upos = upos - 1;
					grid[18][upos] = 'u';
				}
				break;
			case 'd':             ///moves user right
				if (upos < 38)
				{
					grid[18][upos] = ' ';
					upos = upos + 1;
					grid[18][upos] = 'u';
				}
				break;
			case 'o':
				if (nBoomerang > 0)
				{
					grid[17][upos] = boomerang[0];      ///fires homing boomerang
					nBoomerang--;
				}
				break;
			case 'p':
				if (nWall > 0)
				{
					grid[16][upos] = 219;                 ///sets up a wall
					nWall--;
				}
				break;
			case 'i':
				if (nBoost > 0)
				{
					boost = 1;                              ///starts weapon boost
					start = clock();
					nBoost--;
				}
				break;
			}
		}
		///cpu's turn
		cmove = rand() % 4;        ///randomizing cpu's move
		switch (cmove)
		{
		case 0:
			if (cHealth > 10)
				grid[2][cpos] = 'B';    ///fires a bullet
			else
			{
				for (int i = -1; i <= 1; i++)
				{
					if (cpos + i > 0 && cpos + i < 39)
						grid[2][cpos + i] = 'B';          ///fires 3 bullets when health goes 10 or below 10
				}
			}
			break;
		case 1:
			if (cpos > 1)            ///moves left
			{
				grid[1][cpos] = ' ';
				cpos = cpos - 1;
				grid[1][cpos] = 'c';
			}
			break;
		case 2:                 ///moves right
			if (cpos < 38)
			{
				grid[1][cpos] = ' ';
				cpos = cpos + 1;
				grid[1][cpos] = 'c';
			}
			break;
		case 3:                 ///does nothing
			break;
		}
		///prints the arena
		printBuffer();
		///prints score
		score(uHealth, cHealth, bullets, nBoomerang, nWall, boost, nBoost);
		///time delay
		Sleep(70);
		///user bullets
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 40; j++)
			{
				if (grid[i][j] == 'b' && i > 1 && grid[i - 1][j] == ' ')
				{
					grid[i][j] = ' ';
					grid[i - 1][j] = 'b';             ///moves the bullets forward
				}
				if (grid[i][j] == 'b' && i == 1)
					grid[i][j] = ' ';   ///bullets collision with wall
				if (grid[i][j] == 'b' && grid[i - 1][j] == 'B')
				{
					grid[i][j] = ' ';                      ///bullets collision
					grid[i - 1][j] = ' ';
				}
				if (grid[i][j] == 'b' && grid[i - 1][j] == 'c')
				{
					grid[i][j] = ' ';              ///bullet hits cpu
					cHealth--;
					screenflash();
				}
				for (int k = 0; k < 4; k++)
				{
					if (grid[i][j] == boomerang[k] - 256 && (grid[i - 1][j] == 'c' || grid[i - 1][j - 1] == 'c' || grid[i - 1][j + 1] == 'c'))
					{
						grid[i][j] = ' ';              ///boomerang hits cpu
						cHealth -= 1;
						screenflash();
						screenflash();
					}
					if (grid[i][j] == boomerang[k] - 256 && i > 1 && grid[i - 1][j] == ' ')
					{
						grid[i][j] = ' ';
						if (j > cpos)
							grid[i - 1][j - 1] = boomerang[(k + 1) % 4];         /// boomerang moves forward
						if (j < cpos)
							grid[i - 1][j + 1] = boomerang[(k + 1) % 4];           ///homing boomerang
						if (j == cpos)
							grid[i - 1][j] = boomerang[(k + 1) % 4];
						break;
					}
					if (grid[i][j] == boomerang[k] - 256 && grid[i - 1][j] == 'B')
					{
						grid[i][j] = ' ';                      ///bullet and boomerang collision
						grid[i - 1][j] = boomerang[(k + 1) % 4];
					}
					if (grid[i][j] == boomerang[k] - 256 && i == 1)
						grid[i][j] = ' ';                       ///boomerang hits wall
				}
				for (int k = 0; k < 4; k++)
				{
					if (grid[i][j] == 'b' && grid[i - 1][j] == wall[k] - 256)
					{
						grid[i][j] = ' ';
						grid[i - 2][j] = 'b';                                           ///bullets collision with user's wall
					}
					if (grid[i][j] == boomerang[0] - 256 && grid[i - 1][j] == wall[k] - 256)
					{
						grid[i][j] = ' ';
						grid[i - 1][j] = ' ';                                       ///boomerang collision with user's wall
						grid[i - 2][j] = boomerang[2];
					}
				}
			}
		}
		///cpu bullets
		for (int i = 19; i > -1; i--)
		{
			for (int j = 0; j < 40; j++)
			{
				if (grid[i][j] == 'B' && i < 18 && grid[i + 1][j] == ' ')
				{
					grid[i][j] = ' ';                ///moves the bullets forward
					grid[i + 1][j] = 'B';
				}
				if (grid[i][j] == 'B' && i == 18)
					grid[i][j] = ' ';                 ///bullets collision with wall
				if (grid[i][j] == 'B' && grid[i + 1][j] == 'u')
				{
					grid[i][j] = ' ';           ///bullet hits user
					uHealth--;
					screenflash();
				}
				for (int k = 0; k < 4; k++)
				{
					if (grid[i][j] == 'B' && grid[i + 1][j] == wall[k] - 256)
					{
						grid[i][j] = ' ';                             ///cpu's bullets collision with user's wall
						if (k < 3)
							grid[i + 1][j] = wall[k + 1];
						else
							grid[i + 1][j] = ' ';
					}
				}
			}
		}
		if (clock() - start > 5000 && start > 0)
		{
			boost = 0;                                 ///stops the weapon boost
			start = -1;
			reloadtime = clock();
		}
		if (clock() - reloadtime > 10000 && reloadtime > 0)
		{
			if (nBoost == 0)
				nBoost++;                              ///reloads the weapon boost
			reloadtime = -1;
		}
		if (cHealth <= 0)
		{
			winner = 1;                               ///user wins
			break;
		}
		if (uHealth <= 0 || (bullets <= 0 && nBoomerang == 0))
		{
			winner = 2;                                            ///cpu wins
			break;
		}
	}
	win(winner);   ///selects the winner. 0 : no winner, 1 : user wins, 2 : cpu wins


	std::cout << std::endl << std::endl << std::endl << std::endl << std::endl;
}

void win(int winner) ///prints winning message
{
	COORD wintextpos = { 37,12 };
	switch (winner)
	{
	case 1:
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), wintextpos);
		printf("You win!\n");
		Sleep(10000);
		break;
	case 2:
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), wintextpos);
		printf("You lose!\n");
		Sleep(10000);
		break;
	}
}

// Print the scoreboard
void printScoreBoard() {
	COORD cpuscoresz = { 55,3 }, userscoresz = { 55,10 }, ctextpos = { 56,4 }, utextpos = { 56,11 };
	printborder(18, 3, cpuscoresz, 15);
	printborder(18, 7, userscoresz, 15);
	SetConsoleCursorPosition(wHnd, ctextpos);
	std::cout << "CPU health: ";
	SetConsoleCursorPosition(wHnd, utextpos);
	std::cout << "USER health: ";
	utextpos.Y++;
	SetConsoleCursorPosition(wHnd, utextpos);
	std::cout << "Bullets: ";
	utextpos.Y++;
	SetConsoleCursorPosition(wHnd, utextpos);
	std::cout << "Boomerang: ";
	utextpos.Y++;
	SetConsoleCursorPosition(wHnd, utextpos);
	std::cout << "Wall: ";
	utextpos.Y++;
	SetConsoleCursorPosition(wHnd, utextpos);
	std::cout << "Boost: ";
}

void score(int uhealth, int chealth, int ubullets, int uboomerang, int uwall, int boost, int nofboost)   ///prints score
{
	HANDLE out;
	out = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD ctext = { 68,4 }, utext = { 68,16 }, utext1 = { 68,17 }, utext2 = { 68,18 }, utext3 = { 68,19 }, utext4 = { 68,20 };
	SetConsoleCursorPosition(out, ctext);
	printf("   ");
	SetConsoleCursorPosition(out, utext);
	printf("   ");
	SetConsoleCursorPosition(out, utext1);
	printf("   ");
	SetConsoleCursorPosition(out, utext2);
	printf("  ");
	SetConsoleCursorPosition(out, utext3);
	printf("  ");
	SetConsoleCursorPosition(out, utext4);
	printf("   ");
	SetConsoleCursorPosition(out, ctext);
	printf("%d", chealth);
	SetConsoleCursorPosition(out, utext);
	printf("%d", uhealth);
	SetConsoleCursorPosition(out, utext1);
	printf("%d", ubullets);
	SetConsoleCursorPosition(out, utext2);
	printf("%d", uboomerang);
	SetConsoleCursorPosition(out, utext3);
	printf("%d", uwall);
	if (nofboost == 1 && boost == 0)
	{
		SetConsoleCursorPosition(out, utext4);
		SetConsoleTextAttribute(out, 12);
		printf("OFF");
		SetConsoleTextAttribute(out, 7);
	}
	else if (nofboost == 0 && boost == 1)
	{
		SetConsoleCursorPosition(out, utext4);
		SetConsoleTextAttribute(out, 10);
		printf("ON");
		SetConsoleTextAttribute(out, 7);
	}
	else if (nofboost == 0 && boost == 0)
	{
		SetConsoleCursorPosition(out, utext4);
		SetConsoleTextAttribute(out, 14);
		printf("...");
		SetConsoleTextAttribute(out, 7);
	}
}

// utility for printing the border of the scoreboard
void printborder(int w, int h, COORD coord, int color) 
{
	COORD bufferSize = { w, h };
	COORD characterBufferSize = { w, h };
	COORD characterPosition = { 0,0 };
	SMALL_RECT consoleWriteArea;
	consoleWriteArea.Left = coord.X;
	consoleWriteArea.Top = coord.Y;
	consoleWriteArea.Right = consoleWriteArea.Left + w;
	consoleWriteArea.Bottom = consoleWriteArea.Top + h;
	CHAR_INFO * consoleBuffer = new CHAR_INFO[w * h];
	SetConsoleScreenBufferSize(wHnd, bufferSize);


	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (i == 0 || i == h - 1)
			{
				consoleBuffer[j + w * i].Char.AsciiChar = (unsigned char)205;
				consoleBuffer[j + w * i].Attributes = color;
				continue;
			}
			if (j == 0 || j == w - 1)
			{
				consoleBuffer[j + w * i].Char.AsciiChar = (unsigned char)186;
				consoleBuffer[j + w * i].Attributes = color;
				continue;
			}
			consoleBuffer[j + w * i].Char.AsciiChar = (unsigned char)0;
			consoleBuffer[j + w * i].Attributes = color;
		}
	}
	consoleBuffer[0].Char.AsciiChar = (unsigned char)201;
	consoleBuffer[w - 1].Char.AsciiChar = (unsigned char)187;
	consoleBuffer[h*w - 1].Char.AsciiChar = (unsigned char)188;
	consoleBuffer[w*(h - 1)].Char.AsciiChar = (unsigned char)200;
	consoleBuffer[0].Attributes = color;
	consoleBuffer[w - 1].Attributes = color;
	consoleBuffer[h*w - 1].Attributes = color;
	consoleBuffer[w*(h - 1)].Attributes = color;

	WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
	delete[] consoleBuffer;
}

// Prints the Arena
void printBuffer() {
	int userColor = 10, computerColor = 12, color = 15;
	COORD bufferSize = { WIDTH, HEIGHT };
	COORD characterBufferSize = { WIDTH, HEIGHT };
	COORD characterPosition = { 0, 0 };
	SMALL_RECT consoleWriteArea = { 10, 1, WIDTH + 9, HEIGHT };
	CHAR_INFO * consoleBuffer= new CHAR_INFO[WIDTH * HEIGHT];
	SetConsoleScreenBufferSize(wHnd, bufferSize);

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			consoleBuffer[j + 40 * i].Attributes = color;
			if (grid[i][j] == 'X')
				consoleBuffer[j + 40 * i].Char.AsciiChar = (unsigned char)219; // Filled block
			else if (grid[i][j] == 'u')
			{
				consoleBuffer[j + 40 * i].Char.AsciiChar = (unsigned char)1; // Dwarf
				consoleBuffer[j + 40 * i].Attributes = userColor;
			}
			else if (grid[i][j] == ' ')
				consoleBuffer[j + 40 * i].Char.AsciiChar = (unsigned char)0; // Null space
			else if (grid[i][j] == 'b')
				consoleBuffer[j + 40 * i].Char.AsciiChar = (unsigned char)30; // Up arrow
			else if (grid[i][j] == 'B')
				consoleBuffer[j + 40 * i].Char.AsciiChar = (unsigned char)31; // Down arrow
			else if (grid[i][j] == 'c')
			{
				consoleBuffer[j + 40 * i].Char.AsciiChar = (unsigned char)2; // Dark Dwarf
				consoleBuffer[j + 40 * i].Attributes = computerColor;
			}
			else
			{
				consoleBuffer[j + 40 * i].Char.AsciiChar = grid[i][j];
			}
		}
	}

	WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
	delete[] consoleBuffer;
}

void screenflash()  //flashes the screen
{
	char str[9] = "color   ";
	char sym[17] = "0123456789ABCDEF";
	str[6] = sym[rand() % 17];
	str[7] = sym[rand() % 17];
	system(str);
	Sleep(3);
	str[6] = sym[rand() % 17];
	str[7] = sym[rand() % 17];
	system(str);
	Sleep(3);
	str[6] = sym[rand() % 17];
	str[7] = sym[rand() % 17];
	system(str);
	Sleep(3);
	system("color 07");
}

// Display the controls
void controls() {
	std::cout <<
		"CONTROLS:\n" <<
		"'a' = left\n" <<
		"'d' = right\n" <<
		"'w' = fire\n" <<
		"'o' = homing boomerang\n" <<
		"'p' = wall\n" <<
		"'i' = boost" << std::endl;
	system("pause");
}

