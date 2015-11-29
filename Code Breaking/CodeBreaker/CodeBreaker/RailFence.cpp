#include "stdafx.h"
#include <vector>
#include <iostream>
#include "RailFence.h"
#include "Globals.h"

//Crack columnar transposition ciphers
void railFence(std::string text)
{

	//Decrypt text with key
	auto decrypt = [](unsigned rails, unsigned start, std::string text)
	{

		std::vector<std::vector<unsigned>> rail_list(2*rails, std::vector<unsigned>(0));

		unsigned current = start/2;
		bool plus = true;
		if (start / 2 == 1)
		{
			plus = false;
		}
		for (unsigned i = 0; i < text.size(); ++i)
		{
			rail_list[current].push_back(i);

			if (plus&&current + 1 == rails)
			{
				plus = false;
			}
			else if ((!plus) && current == 0)
			{
				plus = true;
			}

			if (plus)
			{
				++current;
			}
			else
			{
				--current;
			}
		}

		std::vector<unsigned> order;
		for (auto v : rail_list)
		{
			order.insert(order.end(), v.begin(), v.end());
		}

		std::string decoded(text.size(), 'a');

		for (unsigned i = 0; i < text.size(); ++i)
		{
			decoded[order[i]] = text[i];
		}

		return decoded;
	};

	//Overall max score
	unsigned max_cnt;
	double max_score = -DBL_MAX;
	unsigned max_start = 0;

	for (unsigned i = 2; i < text.size(); ++i)
	{
		for (unsigned j = 0; j < 2*i-1; ++j)
		{
			if (j == i / 2 - 1)
			{
				continue;
			}

			double score = evaluate(decrypt(i, j, text));

			//Print results if better
			if (score>max_score)
			{
				max_score = score;
				max_start = j;
				max_cnt = i;

				system("cls");
				std::cout << "\n\nBest rail number: " << max_cnt << std::endl;
				std::cout << "Best starting rail: " << max_start << std::endl;
				std::cout << "Best score: " << max_score << std::endl;
				std::cout << "Plaintext:\n\n    " << decrypt(max_cnt, max_start, text) << std::endl;
			}
		}
	}
}