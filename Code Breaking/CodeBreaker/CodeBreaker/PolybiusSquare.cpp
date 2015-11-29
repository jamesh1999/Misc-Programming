#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include "PolybiusSquare.h"
#include "Globals.h"



//Crack simple substitution ciphers
void polybiusSquare(std::string text)
{
	//Replace text letters with ABCDE
	char current = 'A';
	for (char t = 'A'; t <= 'Z'; ++t)
	{
		if (text.find(t) != std::string::npos)
		{
			std::size_t found = text.find_first_of(t);
			while (found != std::string::npos)
			{
				text[found] = current;
				found = text.find_first_of(t, found + 1);
			}

			++current;
		}
	}

	//Decrypt text with key
	auto decrypt = [](std::string key, std::string text)
	{

		std::string result;

		for (unsigned i = 0; i < text.size(); i+=2)
		{
			//Letters that represent coordinates of letter i
			char a = text[i];
			char b = text[i + 1];

			result += key[5 * (a - 'A') + (b - 'A')];
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

			double eval = evaluate(decrypt(child_key, text));

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
		std::cout << "Current score: " << parent_eval << std::endl;
		std::cout << "Current Temp.: " << T << std::endl;
		std::cout << "Best key:  " << parent_key << std::endl;
		std::cout << "Plaintext:\n\n    " << decrypt(parent_key, text) << std::endl;
	}
}