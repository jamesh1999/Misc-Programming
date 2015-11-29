#include "stdafx.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "ColumnarTransposition.h"
#include "Globals.h"

//Crack columnar transposition ciphers
void columnarTransposition(std::string text)
{

	//Decrypt text with key
	auto decrypt = [](unsigned keylen, std::vector<unsigned> key, std::string text)
	{

		std::string decoded = text;

		unsigned val = text.size() / keylen;

		for (unsigned i = 0; i < val; ++i)
		{
			for (unsigned j = 0; j < keylen; ++j)
			{
				decoded[i*keylen+j] = text[i + key[j]*val];
			}
		}

		return decoded;
	};

	//Overall max score
	std::vector<unsigned> top_key;
	double top_score = -DBL_MAX;
	unsigned top_len = 0;

	for (unsigned i = 2; i < text.size(); ++i)
	{
		if (text.size() % i == 0)
		{
			//Key for iteration
			std::vector<unsigned> parent_key;
			for (unsigned j = 0; j < i; ++j)
			{
				parent_key.push_back(j);
			}
			double parent_score = -DBL_MAX;

			for (unsigned iter = 0; iter < 10; ++iter)
			{
				//Generate and evaluate random parent
				std::random_shuffle(parent_key.begin(), parent_key.end());
				parent_score = evaluate(decrypt(i, parent_key, text));

				for (unsigned cnt = 0; cnt < std::min(i*i, 40 * i); ++cnt)
				{
					//Clone the parent key
					std::vector<unsigned> child_key = parent_key;

					//Swap two numbers
					std::swap(child_key[rand() % i], child_key[rand() % i]);

					//Evaluate new key
					double child_score = evaluate(decrypt(i, child_key, text));

					//Compare scores changing parent if child is better
					if (child_score>parent_score)
					{
						parent_key = child_key;
						parent_score = child_score;
						cnt = 0;
					}
				}

				//Print results if better
				if (parent_score>top_score)
				{
					top_score = parent_score;
					top_key = parent_key;
					top_len = i;

					system("cls");
					std::cout << "\n\nCurrent key length: " << i << std::endl;
					std::cout << "Best key length: " << top_len << std::endl;
					std::cout << "Best score: " << top_score << std::endl;
					std::cout << "Iterations: " << iter << std::endl;
					std::cout << "Best key:  ";
					for (unsigned num : top_key)
					{
						std::cout << num << ", ";
					}
					std::cout << std::endl;
					std::cout << "Plaintext:\n\n    " << decrypt(top_len, top_key, text) << std::endl;
				}
			}

			if (top_len == i)
			{
				system("cls");
				std::cout << "\n\nBest key length: " << top_len << std::endl;
				std::cout << "Best score: " << top_score << std::endl;
				std::cout << "Best key:  ";
				for (unsigned num : top_key)
				{
					std::cout << num << ", ";
				}
				std::cout << std::endl;
				std::cout << "Plaintext:\n\n    " << decrypt(top_len, top_key, text) << std::endl;

				system("pause");
			}
		}
	}
}