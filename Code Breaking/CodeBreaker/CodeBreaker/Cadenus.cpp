#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include "Cadenus.h"
#include "Globals.h"



//Cracks cadenus cipher via simulated annealing algorithm
void cadenus(std::string text)
{
	auto decrypt = [](std::vector<unsigned> order_key, std::vector<unsigned> shift_key, std::string text)
	{
		std::vector<std::string> columns(order_key.size(), "");

		for (unsigned i = 0; i < text.size(); ++i)
		{
			columns[order_key[i % order_key.size()]] += text[i];
		}

		std::string result;

		for (int i = 0; i < 25; ++i)
		{
			for (unsigned j = 0; j < order_key.size(); ++j)
			{
				result += columns[j][(shift_key[order_key[j]] + i) % 25];
			}
		}

		return result;
	};

	std::vector<unsigned> top_key1;
	std::vector<unsigned> top_key2;
	double top_eval = -DBL_MAX;

	std::vector<unsigned> parent_key1;
	for (unsigned i = 0; i < text.size() / 25; ++i)
	{
		parent_key1.push_back(i);
	}
	std::vector<unsigned> parent_key2;
	for (unsigned i = 0; i < text.size() / 25; ++i)
	{
		parent_key2.push_back(rand() % 25);
	}

	double parent_eval = -DBL_MAX;

	std::random_shuffle(parent_key1.begin(), parent_key1.end());

	//Implement simulated annealing
	for (double T = TEMP; T >= 0; T -= STEP)
	{
		for (unsigned i = 0; i < COUNT; ++i)
		{
			std::vector<unsigned> child_key1 = parent_key1;
			std::vector<unsigned> child_key2 = parent_key2;

			//Swap columns or change shift in one column
			if (rand() % child_key1.size())
			{
				child_key2[rand() % child_key2.size()] = rand() % 25;
			}
			else
			{
				std::swap(child_key1[rand() % child_key1.size()], child_key1[rand() % child_key1.size()]);
			}

			double eval = evaluate(decrypt(child_key1, child_key2, text));

			//If key is better then always switch
			if (eval > parent_eval)
			{
				parent_eval = eval;
				parent_key1 = child_key1;
				parent_key2 = child_key2;

				if (eval > top_eval)
				{
					top_eval = eval;
					top_key1 = child_key1;
					top_key2 = child_key2;
				}
			}
			else if (T > 0)
			{
				//Otherwise switch if temperature is high enough
				if (exp((eval - parent_eval) / T) > 1.0*rand() / static_cast<double>(RAND_MAX))
				{
					parent_eval = eval;
					parent_key1 = child_key1;
					parent_key2 = child_key2;
				}
			}
		}
		system("cls");
		std::cout << "\n\nCurrent score: " << top_eval << std::endl;
		std::cout << "Current Temp.: " << T << std::endl;
		std::cout << "Best Key: ";
		for (auto v : top_key1)
		{
			std::cout << v << " ";
		}
		std::cout << ", ";
		for (auto v : top_key2)
		{
			std::cout << v << " ";
		}
		std::cout << std::endl;
		std::cout << "Plaintext:\n\n    " << decrypt(top_key1, top_key2, text) << std::endl;
	}
}