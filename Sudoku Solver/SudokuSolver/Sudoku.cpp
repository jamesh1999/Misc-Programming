#include "stdafx.h"
#include <iostream>
#include "Sudoku.h"

#ifdef DEBUG
#include <chrono>
std::chrono::high_resolution_clock::time_point start;
#endif

unsigned DEPTH = 0;
unsigned MAX_DEPTH = 0;



Sudoku& Sudoku::operator=(const Sudoku& other)
{
	for (unsigned i = 0; i < 9; ++i)
	{
		for (unsigned j = 0; j < 9; ++j)
		{
			for (unsigned k = 0; k <= 9; ++k)
			{
				this->values[i][j][k] = other.values[i][j][k];
			}
		}
	}

	return *this;
}

bool Sudoku::isDone()
{
	for (unsigned i = 0; i < 9; ++i)
	{
		for (unsigned j = 0; j < 9; ++j)
		{
			if (values[i][j][0] == ' ')
				return false;
		}
	}

	return true;
}

void Sudoku::createFromString(std::string in)
{
#ifdef DEBUG
	start = std::chrono::high_resolution_clock::now();
#endif

	for (unsigned i = 0; i < 9; ++i)
	{
		for (unsigned j = 0; j < 9; ++j)
		{
			values[i][j][0] = in[i * 9 + j];
		}
	}
}

//Print a sudoku (and clear screen)
void Sudoku::printSelf()
{
#ifndef DEBUG
	system("cls");
#endif
	for (unsigned i = 0; i < 9; ++i)
	{
		if (i && !(i % 3))
			std::cout << "-----------" << std::endl; //Print horizontal divider

		for (unsigned j = 0; j < 9; ++j)
		{
			if (j && !(j % 3))
				std::cout << "|"; //Print vertical divider

			std::cout << values[i][j][0];
		}

		std::cout << std::endl;
	}

#ifdef DEBUG
	std::cout << (isValid() ? "Valid" : "Invalid") << std::endl;
	std::cout << "Current depth: " << DEPTH << std::endl;
	std::cout << "Max depth: " << MAX_DEPTH << std::endl;
	std::cout << std::endl;
#endif
}

//Resets all possibility flags
void Sudoku::resetPossible()
{
	for (unsigned i = 0; i < 9; ++i)
	{
		for (unsigned j = 0; j < 9; ++j)
		{
			for (unsigned k = 1; k <= 9; ++k)
			{
				if (values[i][j][0] == ' ')
					values[i][j][k] = 't';
				else
					values[i][j][k] = ' ';
			}
		}
	}
}

//Sets all flags at the start of a solve
void Sudoku::setPossible()
{
	//Rows/cols
	for (unsigned i = 0; i < 9; ++i)
	{
		for (unsigned j = 0; j < 9; ++j)
		{
			if (values[i][j][0] != ' ')
				continue;

			for (unsigned k = 0; k < 9; ++k)
			{
				if (values[k][j][0] != ' ') //Columns
					values[i][j][values[k][j][0] - '0'] = ' ';

				if (values[i][k][0] != ' ') //Rows
					values[i][j][values[i][k][0] - '0'] = ' ';
			}
		}
	}

	//Blocks
	for (unsigned i = 0; i < 3; ++i)
	{
		for (unsigned j = 0; j < 3; ++j)
		{
			for (unsigned k = 0; k < 9; ++k)
			{
				if (values[i * 3 + k / 3][j * 3 + (k % 3)][0] != ' ')
					continue;

				for (unsigned l = 0; l < 9; ++l)
				{
					if (values[i * 3 + l / 3][j * 3 + (l % 3)][0] != ' ')
						values[i * 3 + k / 3][j * 3 + (k % 3)][values[i * 3 + l / 3][j * 3 + (l % 3)][0] - '0'] = ' ';
				}
			}
		}
	}
}

//If number must be in row/column in a square, remove option from entire row/column
void Sudoku::eliminateFurtherPossible()
{
	for (unsigned i = 0; i < 3; ++i)
	{
		for (unsigned j = 0; j < 3; ++j)
		{
			for (unsigned k = 1; k <= 9; ++k)
			{
				unsigned coords[3][2] = { { 0, 0 }, { 0, 0 }, { 0, 0 } }; //Col, row pairs
				int cnt = 0;

				for (unsigned l = 0; l < 9; ++l)
				{
					if (values[i * 3 + l / 3][j * 3 + (l % 3)][k] == 't')
					{
						if (cnt < 3)
						{
							coords[cnt][0] = l % 3;
							coords[cnt][1] = l / 3;
							++cnt;
						}
						else
						{
							cnt = -1; //Too many values. Can't all be in one row/col
							break;
						}
					}
				}

				if (cnt > 1) //At least 2 possible spaces
				{
					bool row = true;
					bool col = true;

					for (unsigned l = 0; l < cnt; ++l)
					{
						if (coords[l][0] != coords[0][0])
							col = false;
						if (coords[l][1] != coords[0][1])
							row = false;
					}

					if (row) //No other square in row can hold value
					{
						for (unsigned l = 0; l < 9; ++l)
						{
							if (l / 3 != j) //Value must be in other block
								values[i * 3 + coords[0][1]][l][k] = ' ';
						}
					}

					else if (col) //No other square in col can hold value
					{
						for (unsigned l = 0; l < 9; ++l)
						{
							if (l / 3 != i) //Value must be in other block
								values[l][j * 3 + coords[0][0]][k] = ' ';
						}
					}
				}
			}
		}
	}
}

bool Sudoku::fillKnown()
{
	bool changed = false;
	char buffer[9] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }; //Test whether a num can only go in one block
	char buffer_only_option[9] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }; //To test whether one block has only one possible num

	//Rows
	for (unsigned i = 0; i < 9; ++i)
	{
		//Set buffers
		for (unsigned j = 0; j < 9; ++j)
		{
			for (unsigned k = 0; k < 9; ++k)
			{
				if (values[i][j][k + 1] == 't')
				{
					if (buffer_only_option[j] == ' ')
						buffer_only_option[j] = 't';
					else if (buffer_only_option[j] == 't')
						buffer_only_option[j] = 'f';

					if (buffer[k] == ' ')
						buffer[k] = 't';
					else if (buffer[k] == 't')
						buffer[k] = 'f';
				}
			}
		}

		//Apply changes
		for (unsigned j = 0; j < 9; ++j)
		{
			if (values[i][j][0] != ' ')
				continue;

			for (unsigned k = 0; k < 9; ++k)
			{
				if (buffer[k] == 't' && values[i][j][k + 1] == 't')
				{
					setValue(i, j, k + 1);
					changed = true;
				}

				if (buffer_only_option[j] == 't' && values[i][j][k + 1] == 't')
				{
					setValue(i, j, k + 1);
					changed = true;
				}
			}
		}

		//Clear buffers
		for (unsigned j = 0; j < 9; ++j)
		{
			buffer[j] = ' ';
			buffer_only_option[j] = ' ';
		}
	}

	//Cols
	for (unsigned j = 0; j < 9; ++j)
	{
		//Set buffer
		for (unsigned i = 0; i < 9; ++i)
		{
			for (unsigned k = 0; k < 9; ++k)
			{
				if (values[i][j][k + 1] == 't')
				{
					if (buffer[k] == ' ')
						buffer[k] = 't';
					else if (buffer[k] == 't')
						buffer[k] = 'f';
				}
			}
		}

		//Apply changes
		for (unsigned i = 0; i < 9; ++i)
		{
			if (values[i][j][0] != ' ')
				continue;

			for (unsigned k = 0; k < 9; ++k)
			{
				if (buffer[k] == 't' && values[i][j][k + 1] == 't')
				{
					setValue(i, j, k + 1);
					changed = true;
				}
			}
		}

		//Clear buffer
		for (unsigned i = 0; i < 9; ++i)
		{
			buffer[i] = ' ';
		}
	}

	//Blocks
	for (unsigned i = 0; i < 3; ++i)
	{
		for (unsigned j = 0; j < 3; ++j)
		{
			//Set buffer
			for (unsigned k = 0; k < 9; ++k)
			{
				for (unsigned l = 0; l < 9; ++l)
				{
					if (values[i * 3 + k / 3][j * 3 + (k % 3)][l + 1] == 't')
					{
						if (buffer[l] == ' ')
							buffer[l] = 't';
						else if (buffer[l] == 't')
							buffer[l] = 'f';
					}
				}
			}

			//Apply changes
			for (unsigned k = 0; k < 9; ++k)
			{
				for (unsigned l = 0; l < 9; ++l)
				{
					if (values[i * 3 + k / 3][j * 3 + (k % 3)][l + 1] == 't' && buffer[l] == 't')
					{
						setValue(i * 3 + k / 3, j * 3 + (k % 3), l + 1);
						changed = true;
					}
				}
			}

			//Clear buffer
			for (unsigned k = 0; k < 9; ++k)
			{
				buffer[k] = ' ';
			}
		}
	}

	return changed;
}

//Tests to see whether a sudoku is valid or not
bool Sudoku::isValid()
{
	bool occurred[9] = { false, false, false, false, false, false, false, false, false }; //Stores whether a number has already been seen

	//Test rows
	for (unsigned y = 0; y < 9; ++y)
	{
		for (unsigned x = 0; x < 9; ++x)
		{
			if (values[y][x][0] == ' ')
				continue;

			if (occurred[values[y][x][0] - '1'])
				return false;
			else
				occurred[values[y][x][0] - '1'] = true;
		}

		//Reset buffer
		for (unsigned i = 0; i < 9; ++i)
		{
			occurred[i] = false;
		}
	}

	//Test columns
	for (unsigned x = 0; x < 9; ++x)
	{
		for (unsigned y = 0; y < 9; ++y)
		{
			if (values[y][x][0] == ' ')
				continue;

			if (occurred[values[y][x][0] - '1'])
				return false;
			else
				occurred[values[y][x][0] - '1'] = true;
		}

		//Reset buffer
		for (unsigned i = 0; i < 9; ++i)
		{
			occurred[i] = false;
		}
	}

	//Test blocks
	for (unsigned block_y = 0; block_y < 3; ++block_y)
	{
		for (unsigned block_x = 0; block_x < 3; ++block_x)
		{
			for (unsigned i = 0; i < 9; ++i)
			{
				if (values[block_y * 3 + (i / 3)][block_x * 3 + (i % 3)][0] == ' ')
					continue;

				if (occurred[values[block_y * 3 + (i / 3)][block_x * 3 + (i % 3)][0] - '1'])
					return false;
				else
					occurred[values[block_y * 3 + (i / 3)][block_x * 3 + (i % 3)][0] - '1'] = true;
			}

			//Reset buffer
			for (unsigned i = 0; i < 9; ++i)
			{
				occurred[i] = false;
			}
		}
	}

	return true;
}

//Set a value & affected flags
void Sudoku::setValue(unsigned y, unsigned x, unsigned val)
{
	values[y][x][0] = '0' + val;

	for (int i = 0; i < 9; ++i)
	{
		values[y][i][val] = ' '; //Column
		values[i][x][val] = ' '; //Row

		values[y][x][i + 1] = ' '; //Same square
	}

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			values[(y / 3) * 3 + j][(x / 3) * 3 + i][val] = ' '; //Same block
		}
	}
}

//Fill the square with the fewest options with a number
bool Sudoku::fillRandom(unsigned time)
{
	int min_choices = INT_MAX;
	int coords[2] = { 0, 0 };

	//Identify first square with least options
	for (int y = 0; y < 9; ++y)
	{
		for (int x = 0; x < 9; ++x)
		{
			if (values[y][x][0] == ' ')
			{
				int cnt = 0;
				for (int i = 1; i <= 9; ++i)
				{
					if (values[y][x][i] == 't')
						++cnt;
				}

				if (cnt < min_choices)
				{
					min_choices = cnt;
					coords[0] = x;
					coords[1] = y;
				}
			}
		}
	}

	int cnt = 0;
	for (int i = 1; i <= 9; ++i)
	{
		if (values[coords[1]][coords[0]][i] == 't')
		{
			if (cnt == time)
			{
				setValue(coords[1], coords[0], i);
				return true;
			}
			++cnt;
		}
	}

	return false;
}

bool Sudoku::formBranch()
{
#ifdef DEBUG
	++DEPTH;
	if (DEPTH > MAX_DEPTH)
	{
		MAX_DEPTH = DEPTH;
	}
#endif

	Sudoku nPuzzle;
	int time = 0;
	do
	{
		nPuzzle = *this; //Clone parent
		if (!nPuzzle.fillRandom(time)) //Fill child square with fewest options
		{

#ifdef DEBUG
			--DEPTH;
#endif

			return false;
		}
		++time;
	} while (!nPuzzle.solvePuzzle()); //Test branch

	*this = nPuzzle; //Set parent to child if successful

#ifdef DEBUG
	--DEPTH;
#endif

	return true;
}


bool Sudoku::solvePass()
{
	eliminateFurtherPossible();
	if (!fillKnown()) //Attempt to fill blanks
	{
		if (!formBranch()) //Form a new branch if necessary
			return false; //Branch failed
	}

	return isValid();
}

bool Sudoku::solvePuzzle()
{
	resetPossible(); //Clear board
	setPossible(); //Initialise flags

	while (!isDone())
	{
		if (!solvePass())
			return false;
#ifdef DEBUG
		printSelf();
#endif
	}

#ifdef DEBUG
	auto end = std::chrono::high_resolution_clock::now();
	double seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;
	std::cout << "Completed in: " << seconds << " seconds" << std::endl;
#endif

	return true;
}

std::ostream& operator<<(std::ostream& os, const Sudoku& puzzle)
{
	for (int y = 0; y < 9; ++y)
	{
		for (int x = 0; x < 9; ++x)
		{
			os << puzzle.values[y][x][0];
		}
		os << std::endl;
	}
	return os;
}