// 2048AI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <ctime>
#include "Game.h"

int main()
{
	srand(time(nullptr));

	AI2048 ai_tree;
	ai_tree.maxDepth = 7;
	
	Game game(true);
	std::cout << game;
	while(game.GetState() == Ongoing)
	{
		game = ai_tree.Recalculate(game);
		system("cls");
		std::cout << game << std::endl;
		game.AIMakeMove();
		system("cls");
		std::cout << game << std::endl;
	}

	system("pause");
    return 0;
}

