#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include "Bifid.h"
#include "Globals.h"



//Crack simple substitution ciphers
void bifid(std::string text)
{

	unsigned period;
	std::cout << "What period do you want to use? " << std::endl;
	std::cin >> period;

	if (period == 0)
	{
		period = text.length();
	}

	//Decrypt text with key
	auto decrypt = [](int period, std::string key, std::string text)
	{

		std::string result = text;

		for (unsigned i = 0; i < text.length(); i += period)
		{
			if (i + period > text.length())
			{
				period = text.length() - i;
			}

			for (int j = 0; j < period; ++j)
			{
				//Letters that were encoded by the letter i+j
				char a = text[i + (j / 2)];
				char b = text[i + ((j + period) / 2)];

				//Get the coordinates of the letters that came from letter i+j
				int a_part = key.find_first_of(a) / (int)std::pow(5, (j % 2)) % 5;
				int b_part = key.find_first_of(b) / (int)std::pow(5, ((j + period) % 2)) % 5;

				result[i + j] = key[a_part + b_part * 5];
			}
		}
		return result;
	};

	//Overall max key
	std::string parent_key = "ABCDEFGHIKLMNOPQRSTUVWXYZ";

	double parent_eval = -DBL_MAX;

	std::random_shuffle(parent_key.begin(), parent_key.end());

	//Implement simulated annealing
	for (double T = TEMP; T >= 0; T -= STEP)
	{
		for (unsigned cnt = 0; cnt < 624; ++cnt)
		{
			std::string child_key = parent_key;

			//Swap letters
			std::swap(child_key[cnt / 25], child_key[cnt % 25]);

			double eval = evaluate(decrypt(period,child_key, text));

			//If key is better then always switch
			if (eval > parent_eval)
			{
				parent_eval = eval;
				parent_key = child_key;
			}
			else if (T>0)
			{
				//Otherwise switch if temperature is high enough
				if (exp((eval - parent_eval) / T) > 1.0*rand() / (double)RAND_MAX)
				{
					parent_eval = eval;
					parent_key = child_key;
				}
			}
		}
		system("cls");
		std::cout << "\n\nPeriod: " << period << std::endl;
		std::cout << "Current score: " << parent_eval << std::endl;
		std::cout << "Current Temp.: " << T << std::endl;
		std::cout << "Best key:  " << parent_key << std::endl;
		std::cout << "Plaintext:\n\n    " << decrypt(period,parent_key, text) << std::endl;
	}
}