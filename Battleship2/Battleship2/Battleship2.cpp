// Battleship2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "pch.h"
#include <iostream>
#include "time.h"

using namespace std;

const int rows = 8;
const int columns = 8;

const char hit = 'X';
const char nearMiss = '+';
const char miss = '0';
const char blank = '#';

void Print2DArray(char a[rows][columns])
{
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			cout << a[r][c] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

char** AddNearMiss(char a[rows][columns])
{
	char** nearMissArray;
	nearMissArray = new char*[rows];

	for (int r = 0; r < rows; r++)
	{
		nearMissArray[r] = new char[columns];

		for (int c = 0; c < columns; c++)
		{
			nearMissArray[r][c] = a[r][c];
		}
	}

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			if (nearMissArray[r][c] == hit)
			{
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -1; j <= 1; j++)
					{
						if (r + i >= 0 && r + i < rows && c + j >= 0 && c + j < columns)
							if (nearMissArray[r + i][c + j] != hit)
								nearMissArray[r + i][c + j] = nearMiss;
					}
				}
			}
		}
	}

	return nearMissArray;
}

//probably could've made into a class
void PlayerGuess(char a[rows][columns], char aUnknown[rows][columns], int numofships)
{
	char board[rows][columns];
	char cheatsheet[rows][columns];

	int rowGuess;
	int columnGuess;
	bool playing = true;
	int numOfCorrectGuesses = 0;

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			board[r][c] = aUnknown[r][c];
		}
	}

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			cheatsheet[r][c] = AddNearMiss(a)[r][c];
		}
	}

	do
	{
		cout << "Number of ships left: " << numofships - numOfCorrectGuesses << "\n" << endl;

		//Print2DArray(cheatsheet); //only for testing
		Print2DArray(board);

		cout << "Enter your guess for the row position: \n";
		cin >> rowGuess;

		cout << "Enter your guess for the column position: \n";
		cin >> columnGuess;

		cout << "\n" << "-------------------------" << endl;
		cout << rowGuess << "," << " " << columnGuess << endl;
		rowGuess -= 1;
		columnGuess -= 1;

		if (cheatsheet[rowGuess][columnGuess] == hit && board[rowGuess][columnGuess] == blank) //hit
		{
			cout << "Hit!" << endl;
			cout << "-------------------------" << "\n" << endl;
			numOfCorrectGuesses++;
		}
		else if (cheatsheet[rowGuess][columnGuess] == nearMiss && board[rowGuess][columnGuess] == blank) //near miss
		{
			cout << "Near Miss!" << endl;
			cout << "-------------------------" << "\n" << endl;
		}
		else if(board[rowGuess][columnGuess] == blank) //miss
		{
			cout << "Miss!" << endl;
			cout << "-------------------------" << "\n" << endl;
		}
		else
		{
			cout << "That spot has already been hit! Quit wasting ammo!" << endl;
			cout << "-------------------------" << "\n" << endl;
		}

		if (numOfCorrectGuesses == numofships)
		{
			cout << "You win!" << endl;
			playing = false;
		}

		//show where hit
		if (rowGuess >= 0 && rowGuess <= rows - 1 && columnGuess >= 0 && columnGuess <= columns - 1)
			board[rowGuess][columnGuess] = cheatsheet[rowGuess][columnGuess];

	} while (playing);
}

void AIGuess(char a[rows][columns], char aUnknown[rows][columns], int numofships)
{
	char board[rows][columns];
	char cheatsheet[rows][columns];

	int rowGuess;
	int columnGuess;
	bool playing = true;
	int numOfCorrectGuesses = 0;

	bool gotANearMiss = false;

	int nearMissRow;
	int nearMissColumn;

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			board[r][c] = aUnknown[r][c];
		}
	}

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			cheatsheet[r][c] = AddNearMiss(a)[r][c];
		}
	}

	do
	{
		cout << "Number of ships left: " << numofships - numOfCorrectGuesses << "\n" << endl;

		

		bool validChoice = false;
		
		if (gotANearMiss)
		{
			int blankCounter = 0;

			for (int r = nearMissRow - 1; r < nearMissRow + 1; r++)
			{
				for (int c = nearMissColumn - 1; c < nearMissColumn + 1; c++)
				{
					if (r >= 0 && r <= rows - 1 && c >= 0 && c <= columns - 1)
					if (board[r][c] == blank)
					{
						blankCounter++;
					}
				}
			}

			if (blankCounter == 0)
			{
				gotANearMiss = false;
			}
		}

		if (!gotANearMiss)
		{
			do
			{
				if (!validChoice)
				{
					rowGuess = rand() % (rows - 1);
					columnGuess = rand() % (columns - 1);
				}

				if (board[rowGuess][columnGuess] == blank && rowGuess >= 0 && columnGuess >= 0)
				{
					validChoice = true;
				}


			} while (!validChoice);
		}
		else
		{
			do
			{
				if (!validChoice)
				{
					rowGuess = rand() % 3 - 1 + nearMissRow;
					columnGuess = rand() % 3 - 1 + nearMissColumn;
				}

				if (board[rowGuess][columnGuess] == blank && rowGuess >= 0 && columnGuess >= 0)
				{
					validChoice = true;
				}


			} while (!validChoice);
		}
		

		cout << "\n" << "-------------------------" << endl;
		cout << rowGuess + 1 << "," << " " << columnGuess + 1<< endl;
		rowGuess -= 1;
		columnGuess -= 1;

		if (cheatsheet[rowGuess][columnGuess] == hit) //hit
		{
			cout << "Hit!" << endl;
			cout << "-------------------------" << "\n" << endl;
			numOfCorrectGuesses++;
			gotANearMiss = false;
		}
		else if (cheatsheet[rowGuess][columnGuess] == nearMiss) //near miss
		{
			cout << "Near Miss!" << endl;
			cout << "-------------------------" << "\n" << endl;

			nearMissRow = rowGuess;
			nearMissColumn = columnGuess;

			gotANearMiss = true;
		}
		else //miss
		{
			cout << "Miss!" << endl;
			cout << "-------------------------" << "\n" << endl;
		}

		if (numOfCorrectGuesses == numofships)
		{
			cout << "Victory!" << endl;
			playing = false;
		}

		//show where hit
		if (rowGuess >= 0 && rowGuess <= rows - 1 && columnGuess >= 0 && columnGuess <= columns - 1)
			board[rowGuess][columnGuess] = cheatsheet[rowGuess][columnGuess];

		//Print2DArray(cheatsheet); //only for testing
		Print2DArray(board);

		char c;
		cin >> c;

	} while (playing);
}

int main()
{
	char arr[rows][columns];
	char arrUnknown[rows][columns];

	int columntest = -1;
	int shipcount = 0;

	srand((unsigned)time(NULL));

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			arrUnknown[r][c] = blank;
		}
	}

	for (int r = 0; r < rows; r++)
	{
		bool rowtest = rand() % 2;

		for (int c = 0; c < columns; c++)
		{
			if (rowtest)
			{
				columntest = rand() % columns;
			}

			if (columntest == c)
			{
				arr[r][c] = hit;
				shipcount++;
			}
			else if (r == rows - 1 && c == columns - 1 && shipcount == 0)
			{
				arr[r][c] = hit;
				shipcount++;
			}
			else
			{
				arr[r][c] = miss;
			}
		}
		columntest = -1;
	}


	//PlayerGuess(arr, arrUnknown, shipcount);
	AIGuess(arr, arrUnknown, shipcount);
}