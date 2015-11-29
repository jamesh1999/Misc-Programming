
// SudokuSolver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Sudoku.h"



int _tmain(int argc, _TCHAR* argv[])
{
	Sudoku n = Sudoku();

	//Read file from in.txt
	std::ifstream input;
	input.open("in.txt");

	std::string source;
	std::string line;

	while (std::getline(input, line))
		source += line;

	input.close();

	//Create sudoku
	n.createFromString(source);

	//Solve puzzle
	n.printSelf();
	n.solvePuzzle();
	n.printSelf();

	if (n.isValid())
		std::cout << "\n\nSolution is valid     :)" << std::endl;
	else
		std::cout << "\n\nSolution isn't valid     ;(" << std::endl;

	//Write to out.txt
	std::ofstream output;
	output.open("out.txt", std::ios_base::trunc);
	output << n;
	output.close();

	system("pause");
	return 0;
}