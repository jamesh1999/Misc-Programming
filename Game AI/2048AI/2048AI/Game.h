#ifndef __BOARD_INCLUDED__
#define __BOARD_INCLUDED__
#include <iostream>
#include <cstdint>
#include "AITree.h"

enum GameStates {Won, Lost, Ongoing};
enum MoveChoices {Up, Down, Left, Right};

typedef struct
{
	uint64_t board = 0;
	inline int GetVal(int x, int y) const
	{
		int shift = (y * 4 + x) * 4;
		return (board & 0xfULL << shift) >> shift;
	}
	inline void SetVal(int x, int y, unsigned long long val)
	{
		int shift = (y * 4 + x) * 4;
		board &= ~(0xfULL << shift);
		board |= (val & 0xfULL) << shift;
	}

} Board;

typedef struct
{
	MoveChoices move;
	Board new_board;
} Move;

class Game
{
	friend std::ostream& operator<<(std::ostream&, const Game&);
	friend Game;

public:
	Game() = default;
	explicit Game(const Board&);
	Game(const Game&);
	Game(bool);
	Board current;

	GameStates GetState() const;
	int NextMoves(Move*) const;
	int AINextMoves(Board*) const;

	bool MakeMove(MoveChoices);
	void AIMakeMove();
};

std::ostream& operator<<(std::ostream&, const Board&);
std::ostream& operator<<(std::ostream&, const Game&);

class AI2048 : public AI::MinMaxTree<Game>
{
	void RecalculateRecursive(Containers::DynamicTreeNode<AI::MinMaxData<Game>>*, int) override;
};

#endif