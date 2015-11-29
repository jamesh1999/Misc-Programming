#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include "FourSquare.h"
#include "Globals.h"



//Cracks foursquare cipher via simulated annealing algorithm
void fourSquare(std::string text)
{
	auto decrypt = [](std::string key1, std::string key2, std::string text)
	{
		//Top-left and bottom-right squares
		std::string alph = "ABCDEFGHIKLMNOPQRSTUVWXYZ";

		for (unsigned i = 0; i < text.size() - 1; i += 2)
		{
			int a_ind = key1.find_first_of(text[i]);
			int b_ind = key2.find_first_of(text[i + 1]);
			text[i] = alph[5 * (a_ind / 5) + (b_ind % 5)];  //Work out row and height from keys then decrypt
			text[i + 1] = alph[5 * (b_ind / 5) + (a_ind % 5)];//"
		}

		return text;
	};

	std::string parent_key1 = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
	std::string parent_key2 = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
	double parent_eval = -DBL_MAX;

	std::random_shuffle(parent_key1.begin(), parent_key1.end());
	std::random_shuffle(parent_key2.begin(), parent_key2.end());

	//Implement simulated annealing
	for (double T = TEMP; T >= 0; T -= STEP)
	{
		for (unsigned i = 0; i<COUNT; ++i)
		{
			std::string child_key1 = parent_key1;
			std::string child_key2 = parent_key2;

			//Swap letters
			if (i % 2)
			{
				std::swap(child_key1[rand() % 25], child_key1[rand() % 25]);
			}
			else
			{
				std::swap(child_key2[rand() % 25], child_key2[rand() % 25]);
			}

			double eval = evaluate(decrypt(child_key1, child_key2, text));

			//If key is better then always switch
			if (eval - parent_eval >= 0)
			{
				parent_eval = eval;
				parent_key1 = child_key1;
				parent_key2 = child_key2;
			}
			else if (T>0)
			{
				//Otherwise switch if temperature is high enough
				if (exp((eval - parent_eval) / T) > 1.0*rand() / (double)RAND_MAX)
				{
					parent_eval = eval;
					parent_key1 = child_key1;
					parent_key2 = child_key2;
				}
			}
		}
		system("cls");
		std::cout << "\n\nCurrent score: " << parent_eval << std::endl;
		std::cout << "Current Temp.: " << T << std::endl;
		std::cout << "Best key:  " << parent_key1 << ", " << parent_key2 << std::endl;
		std::cout << "Plaintext:\n\n    " << decrypt(parent_key1, parent_key2, text) << std::endl;
	}

}