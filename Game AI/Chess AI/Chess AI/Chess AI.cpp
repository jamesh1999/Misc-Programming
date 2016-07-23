// Chess AI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AITree.h"
#include <iostream>

struct TicTacToeBoard
{
	char board[9] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
};

char getWinner(const char* board)
{
	if (board[0] == board[1] && board[1] == board[2] && board[0] != ' ')
		return board[0];
	if (board[3] == board[4] && board[4] == board[5] && board[3] != ' ')
		return board[3];
	if (board[6] == board[7] && board[7] == board[8] && board[6] != ' ')
		return board[6];

	if (board[0] == board[3] && board[3] == board[6] && board[0] != ' ')
		return board[0];
	if (board[1] == board[4] && board[4] == board[7] && board[1] != ' ')
		return board[1];
	if (board[2] == board[5] && board[5] == board[8] && board[2] != ' ')
		return board[2];

	if (board[0] == board[4] && board[4] == board[8] && board[0] != ' ')
		return board[0];
	if (board[2] == board[4] && board[4] == board[6] && board[6] != ' ')
		return board[2];

	return ' ';
}

void printBoard(char* board)
{
	std::cout << board[0] << board[1] << board[2] << std::endl
		<< board[3] << board[4] << board[5] << std::endl
		<< board[6] << board[7] << board[8] << std::endl;
}

int main()
{
	AI::MinMaxTree<TicTacToeBoard> ai_tree_a;
	AI::MinMaxTree<TicTacToeBoard> ai_tree_b;

	ai_tree_a.evaluationFunction = [](const TicTacToeBoard& board)
	{
		if (getWinner(board.board) == 'X')
			return 1.0;

		if (getWinner(board.board) == 'O')
			return -1.0;

		return 0.0;
	};

	ai_tree_a.getPlayerMovesFunction = [](const TicTacToeBoard& current, TicTacToeBoard*& next)
	{
		//Count empty squares
		int cnt = 0;
		for (int i = 0; i < 9; ++i)
			if (current.board[i] == ' ')
				++cnt;

		next = new TicTacToeBoard[cnt];

		//Construct boards
		int last = -1;
		for (int i = 0; i < cnt; ++i)
		{
			memcpy(next[i].board, current.board, 9);

			for (int j = last + 1; j < 9; ++j)
				if(next[i].board[j] == ' ')
				{
					last = j;
					next[i].board[j] = 'X';
				}
		}

		return cnt;
	};

	ai_tree_a.getOpponentMovesFunction = [](const TicTacToeBoard& current, TicTacToeBoard*& next)
	{
		//Count empty squares
		int cnt = 0;
		for (int i = 0; i < 9; ++i)
			if (current.board[i] == ' ')
				++cnt;

		next = new TicTacToeBoard[cnt];

		//Construct boards
		int last = -1;
		for (int i = 0; i < cnt; ++i)
		{
			memcpy(next[i].board, current.board, 9);

			for (int j = last + 1; j < 9; ++j)
				if (next[i].board[j] == ' ')
				{
					last = j;
					next[i].board[j] = 'O';
				}
		}

		return cnt;
	};

	ai_tree_b.evaluationFunction = [](const TicTacToeBoard& board)
	{
		if (getWinner(board.board) == 'X')
			return -1.0;

		if (getWinner(board.board) == 'O')
			return 1.0;

		return 0.0;
	};

	ai_tree_b.getPlayerMovesFunction = [](const TicTacToeBoard& current, TicTacToeBoard*& next)
	{
		//Count empty squares
		int cnt = 0;
		for (int i = 0; i < 9; ++i)
			if (current.board[i] == ' ')
				++cnt;

		next = new TicTacToeBoard[cnt];

		//Construct boards
		int last = -1;
		for (int i = 0; i < cnt; ++i)
		{
			memcpy(next[i].board, current.board, 9);

			for (int j = last + 1; j < 9; ++j)
				if (next[i].board[j] == ' ')
				{
					last = j;
					next[i].board[j] = 'O';
				}
		}

		return cnt;
	};

	ai_tree_b.getOpponentMovesFunction = [](const TicTacToeBoard& current, TicTacToeBoard*& next)
	{
		//Count empty squares
		int cnt = 0;
		for (int i = 0; i < 9; ++i)
			if (current.board[i] == ' ')
				++cnt;

		next = new TicTacToeBoard[cnt];

		//Construct boards
		int last = -1;
		for (int i = 0; i < cnt; ++i)
		{
			memcpy(next[i].board, current.board, 9);

			for (int j = last + 1; j < 9; ++j)
				if (next[i].board[j] == ' ')
				{
					last = j;
					next[i].board[j] = 'X';
				}
		}

		return cnt;
	};

	TicTacToeBoard game;
	while(getWinner(game.board) == ' ')
	{
		game = ai_tree_a.Recalculate(game);
		printBoard(game.board);
		system("pause");

		if (getWinner(game.board) != ' ')
			break;

		game = ai_tree_a.Recalculate(game);
		printBoard(game.board);
		system("pause");
	}

	std::cout << getWinner(game.board) << " won!" << std::endl;
	system("pause");

    return 0;
}

