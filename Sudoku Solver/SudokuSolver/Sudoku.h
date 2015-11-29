#ifndef __SUDOKU_INCLUDED__
#define __SUDOKU_INCLUDED__

#include <string>

//#define DEBUG

typedef char board[9][9][10];

class Sudoku{
	friend std::ostream& operator<<(std::ostream&, const Sudoku&);
private:
	board values; //[x][y][0] = Value in Sudoku

public:
	Sudoku& Sudoku::operator=(const Sudoku& other);

	bool isDone();
	void createFromString(std::string);
	void printSelf();
	void resetPossible();
	void setPossible();
	void eliminateFurtherPossible();
	bool fillKnown();
	bool isValid();
	void setValue(unsigned, unsigned, unsigned);
	bool fillRandom(unsigned);
	bool formBranch();
	bool solvePass();
	bool solvePuzzle();

};

std::ostream& operator<<(std::ostream&, const Sudoku&);

#endif