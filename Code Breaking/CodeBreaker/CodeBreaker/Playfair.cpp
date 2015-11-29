#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include "Playfair.h"
#include "Globals.h"



//Crack simple substitution ciphers
void playfair(std::string text)
{

	//Decrypt text with key
	auto decrypt = [](std::string key, std::string text)
	{
		for (unsigned i = 0; i < text.size(); i+=2)
		{
			int a_ind = key.find_first_of(text[i]);
			int b_ind = key.find_first_of(text[i + 1]);

			if (a_ind / 5 == b_ind / 5)
			{
				text[i] = key[(((a_ind % 5) + 4) % 5) + (5 * (a_ind / 5))];
				text[i + 1] = key[(((b_ind % 5) + 4) % 5) + (5 * (b_ind / 5))];
			}
			else if (a_ind % 5 == b_ind % 5)
			{
				text[i] = key[(a_ind % 5) + (5 * (((a_ind / 5) + 4) % 5))];
				text[i + 1] = key[(b_ind % 5) + (5 * (((b_ind / 5) + 4) % 5))];
			}
			else
			{
				text[i] = key[5 * (a_ind / 5) + (b_ind % 5)];
				text[i + 1] = key[5 * (b_ind / 5) + (a_ind % 5)];
			}
		}

		for (unsigned i = 0; i < text.size(); ++i)
		{
			if (text[i] == 'X' && i != 0)
			{
				text[i] = text[i - 1];
			}
		}
		
		return text;
	};

	//Overall max key
	std::string parent_key = "ABCDEFGHIKLMNOPQRSTUVWXYZ";

	double parent_eval = -DBL_MAX;

	std::random_shuffle(parent_key.begin(), parent_key.end());

	//Implement simulated annealing
	for (double T = TEMP; T >= 0; T -= STEP)
	{
		for (unsigned cnt = 0; cnt < 674; ++cnt)
		{
			std::string child_key = parent_key;

			//Swap letters
			if (cnt < 625)
			{
				std::swap(child_key[cnt / 25], child_key[cnt % 25]);
			}
			//Swap rows
			else if (cnt < 650)
			{
				int row1 = (cnt - 625) / 5;
				int row2 = cnt % 5;
				for (int i = 0; i < 5; ++i)
				{
					char temp = child_key[(5 * row1) + i];
					child_key[(5 * row1) + i] = child_key[(5 * row2) + i];
					child_key[(5 * row2) + i] = temp;
				}
			}
			//Swap columns
			else
			{
				int col1 = (cnt - 650) / 5;
				int col2 = cnt % 5;
				for (int i = 0; i < 5; ++i)
				{
					char temp = child_key[(5 * i) + col1];
					child_key[(5 * i) + col1] = child_key[(5 * i) + col2];
					child_key[(5 * i) + col2] = temp;
				}
			}

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