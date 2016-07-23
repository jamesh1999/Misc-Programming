#include "stdafx.h"
#include "Game.h"
#include <cstring>

std::ostream& operator<<(std::ostream& stream, const Board& board)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			int value = 1 << board.GetVal(j, i);

			if(value == 1)
				stream << "     ";
			else
			{
				stream << value;

				//Align with spaces
				char strbuff[16];
				_itoa_s(value, strbuff, 10);
				for (int k = 5 - strlen(strbuff); k > 0; --k)
					stream << " ";
			}
			
			if (j == 3) continue;
			stream << "| ";
		}
		stream << std::endl << "--------------------------" << std::endl;
	}

	return stream;
}

std::ostream& operator<<(std::ostream& stream, const Game& game)
{
	stream << game.current;
	return stream;
}

Game::Game(const Board& new_board)
{
	current.board = new_board.board;
}

Game::Game(const Game& new_game)
{
	current.board = new_game.current.board;
}

Game::Game(bool initialise_board)
{
	if(initialise_board)
	{
		AIMakeMove();
		AIMakeMove();
	}
}

GameStates Game::GetState() const
{
#ifndef DONT_STOP
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (current.GetVal(i, j) == 11) return Won;
#endif
	
	Move moves[4];
	if (NextMoves(moves) == 0)
		return Lost;

	return Ongoing;
}

int Game::NextMoves(Move* moves) const
{
	int index = 0;
	bool move_possible;

	for (int m = 0; m < 4; ++m)
		moves[m].new_board.board = current.board;

	//Up
	move_possible = false;

	for (int x = 0; x < 4; ++x)
		for (int y = 0; y < 4; ++y)
		{
			if (moves[index].new_board.GetVal(x, y) == 0) continue;

			//Collapse identical tiles
			for (int y_search = y + 1; y_search < 4; ++y_search)
				if(moves[index].new_board.GetVal(x, y_search) != 0)
				{
					if(moves[index].new_board.GetVal(x, y_search) == moves[index].new_board.GetVal(x, y))
					{
						moves[index].new_board.SetVal(x, y_search, 0);
						moves[index].new_board.SetVal(x, y, moves[index].new_board.GetVal(x, y) + 1);
						move_possible = true;
					}
					break;
				}

			//Move into free spaces
			int y_search;
			for (y_search = y - 1; y_search >= 0; --y_search)
				if (moves[index].new_board.GetVal(x, y_search) != 0)
				{
					++y_search;
					break;
				}

			if (y_search == -1) y_search = 0;

			if(y_search < y)
			{
				moves[index].new_board.SetVal(x, y_search, moves[index].new_board.GetVal(x, y));
				moves[index].new_board.SetVal(x, y, 0);
				move_possible = true;
			}
		}

	if(move_possible)
	{
		moves[index].move = Up;
		++index;
	}

	//Down
	move_possible = false;

	for (int x = 0; x < 4; ++x)
		for (int y = 3; y >= 0; --y)
		{
			if (moves[index].new_board.GetVal(x, y) == 0) continue;

			//Collapse identical tiles
			for (int y_search = y - 1; y_search >= 0; --y_search)
				if (moves[index].new_board.GetVal(x, y_search) != 0)
				{
					if (moves[index].new_board.GetVal(x, y_search) == moves[index].new_board.GetVal(x, y))
					{
						moves[index].new_board.SetVal(x, y_search, 0);
						moves[index].new_board.SetVal(x, y, moves[index].new_board.GetVal(x, y) + 1);
						move_possible = true;
					}
					break;
				}

			//Move into free spaces
			int y_search;
			for (y_search = y + 1; y_search < 4; ++y_search)
				if (moves[index].new_board.GetVal(x, y_search) != 0)
				{
					--y_search;
					break;
				}

			if (y_search == 4) y_search = 3;

			if (y_search > y)
			{
				moves[index].new_board.SetVal(x, y_search, moves[index].new_board.GetVal(x, y));
				moves[index].new_board.SetVal(x, y, 0);
				move_possible = true;
			}
		}

	if (move_possible)
	{
		moves[index].move = Down;
		++index;
	}

	//Left
	move_possible = false;

	for (int y = 0; y < 4; ++y)
		for (int x = 0; x < 4; ++x)
		{
			if (moves[index].new_board.GetVal(x, y) == 0) continue;

			//Collapse identical tiles
			for (int x_search = x + 1; x_search < 4; ++x_search)
				if (moves[index].new_board.GetVal(x_search, y) != 0)
				{
					if (moves[index].new_board.GetVal(x_search, y) == moves[index].new_board.GetVal(x, y))
					{
						moves[index].new_board.SetVal(x_search, y, 0);
						moves[index].new_board.SetVal(x, y, moves[index].new_board.GetVal(x, y) + 1);
						move_possible = true;
					}
					break;
				}

			//Move into free spaces
			int x_search;
			for (x_search = x - 1; x_search >= 0; --x_search)
				if (moves[index].new_board.GetVal(x_search, y) != 0)
				{
					++x_search;
					break;
				}

			if (x_search == -1) x_search = 0;

			if (x_search < x)
			{
				moves[index].new_board.SetVal(x_search, y, moves[index].new_board.GetVal(x, y));
				moves[index].new_board.SetVal(x, y, 0);
				move_possible = true;
			}
		}

	if (move_possible)
	{
		moves[index].move = Left;
		++index;
	}

	//Right
	move_possible = false;

	for (int y = 0; y < 4; ++y)
		for (int x = 3; x >= 0; --x)	
		{
			if (moves[index].new_board.GetVal(x, y) == 0) continue;

			//Collapse identical tiles
			for (int x_search = x - 1; x_search >= 0; --x_search)
				if (moves[index].new_board.GetVal(x_search, y) != 0)
				{
					if (moves[index].new_board.GetVal(x_search, y) == moves[index].new_board.GetVal(x, y))
					{
						moves[index].new_board.SetVal(x_search, y, 0);
						moves[index].new_board.SetVal(x, y, moves[index].new_board.GetVal(x, y) + 1);
						move_possible = true;
					}
					break;
				}

			//Move into free spaces
			int x_search;
			for (x_search = x + 1; x_search < 4; ++x_search)
				if (moves[index].new_board.GetVal(x_search, y) != 0)
				{
					--x_search;
					break;
				}

			if (x_search == 4) x_search = 3;

			if (x_search > x)
			{
				moves[index].new_board.SetVal(x_search, y, moves[index].new_board.GetVal(x, y));
				moves[index].new_board.SetVal(x, y, 0);
				move_possible = true;
			}
		}

	if (move_possible)
	{
		moves[index].move = Right;
		++index;
	}

	return index;
}

int Game::AINextMoves(Board* moves) const
{
	int index = 0;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (current.GetVal(i, j) == 0)
			{
				moves[index].board = current.board;
				moves[index + 1].board = current.board;

				moves[index].SetVal(i, j, 1);
				moves[index + 1].SetVal(i, j, 2);

				index += 2;
			}

	return index;
}

bool Game::MakeMove(MoveChoices choice)
{
	Move moves[4];
	int num_moves = NextMoves(moves);
	
	for (int n = 0; n < num_moves; ++n)
		if(choice == moves[n].move)
		{
			current.board = moves[n].new_board.board;
			return true;
		}

	return false;
}

void Game::AIMakeMove()
{
	Board boards[32];
	int num_moves = AINextMoves(boards);
	int choice = (rand() % (num_moves / 2)) * 2 + (rand() % 10 == 0 ? 1 : 0);

	current.board = boards[choice].board;
}

//2048 MinMax algorithm
void AI2048::RecalculateRecursive(Containers::DynamicTreeNode<AI::MinMaxData<Game>>* working_node, int depth)
{
	//Don't go over max depth: Evaluate & return
	if (depth >= maxDepth)
	{
		working_node->value.score = 0.0;

		switch (working_node->value.board.GetState())
		{
		case Won:
			working_node->value.score = 16.0;
			break;
		case Lost:
			working_node->value.score = -1000.0;
			break;
		case Ongoing:
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					if (working_node->value.board.current.GetVal(i, j) == 0)
						working_node->value.score += 1.0;
			break;
		}
		return;
	}

	//Create all children
	Game child_games[32];
	int total_boards = 0;

	if (depth % 2)
	{
		Board boards[32];
		total_boards = working_node->value.board.AINextMoves(boards);
		for (int i = 0; i < total_boards; ++i)
			child_games[i] = Game(boards[i]);
	}
	else
	{
		Move moves[4];
		total_boards = working_node->value.board.NextMoves(moves);
		for (int i = 0; i < total_boards; ++i)
			child_games[i] = Game(moves[i].new_board);
	}

	working_node->value.score = 0.0;

	//Calculate children
	for (int i = 0; i < total_boards; ++i)
	{
		//Create and Add child node
		AI::MinMaxData<Game> data;
		data.board = child_games[i];
		data.alpha = working_node->value.alpha;
		data.beta = working_node->value.beta;
		data.score = depth % 2 ? DBL_MIN : DBL_MAX; //Set starting score dependant on min/max node
		working_node->InsertChild(0, data);

		//Recalculate node
		RecalculateRecursive(&((*working_node)[0]), depth + 1);

		//Set score as required
		if (depth % 2)
			working_node->value.score += (*working_node)[0].value.score;
		else
		{
			//Update score
			if (working_node->value.score < (*working_node)[0].value.score)
				working_node->value.score = (*working_node)[0].value.score;
		}
	}

	//Get average
	if(depth % 2)
		working_node->value.score /= total_boards;

	//If leaf node: evaluate
	if (total_boards == 0)
	{
		working_node->value.score = 0.0;

		switch (working_node->value.board.GetState())
		{
		case Won:
			working_node->value.score = 16.0;
			break;
		case Lost:
			working_node->value.score = -1000.0;
			break;
		case Ongoing:
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					if (working_node->value.board.current.GetVal(i, j) == 0)
						working_node->value.score += 1.0;
			break;
		}
	}
}