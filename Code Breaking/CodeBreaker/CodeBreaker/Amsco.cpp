#include "stdafx.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "Amsco.h"
#include "Globals.h"

//Crack amsco ciphers
void amsco(std::string text)
{
	//Decrypt text with key
	auto decrypt = [](int start, unsigned keylen, std::vector<unsigned> key, std::string text)
	{
		start += 1;
		std::vector<unsigned> col_lengths(keylen, 0);


		unsigned cur_col = 0;
		unsigned index = 0;
		unsigned size = start;

		while (index < text.size())
		{
			col_lengths[cur_col] += std::min(size, text.size() - index);

			index += size;

			if (size == 2)
			{
				size = 1;
			}
			else
			{
				size = 2;
			}

			++cur_col;
			cur_col %= keylen;
		}

		std::vector<std::string> cols(keylen, "");
		unsigned total = 0;

		for (unsigned i = 0; i < keylen; ++i)
		{
			cols[key[i]] = text.substr(total, col_lengths[key[i]]);
			total += col_lengths[key[i]];
		}

		std::string decoded;
		size = start;
		cur_col = 0;

		while (decoded.size() < text.size())
		{
			decoded += cols[cur_col].substr(0, size);
			cols[cur_col].erase(0, size);

			++cur_col;
			cur_col %= keylen;

			if (size == 2)
			{
				size = 1;
			}
			else
			{
				size = 2;
			}
		}

		return decoded;
	};

	//Overall max score
	int top_i = 0;
	std::vector<unsigned> top_key;
	double top_score = -DBL_MAX;
	unsigned top_len = 0;

	for (unsigned i = 2; i < text.size(); ++i)
	{
		//Key for iteration
		std::vector<unsigned> parent_key;
		for (unsigned j = 0; j < i; ++j)
		{
			parent_key.push_back(j);
		}
		double child_score;

		while (std::next_permutation(parent_key.begin(), parent_key.end()))
		{
			//Evaluate new key
			child_score = evaluate(decrypt(0, i, parent_key, text));

			if (child_score > top_score)
			{
				top_i = 0;
				top_key = parent_key;
				top_score = child_score;
				top_len = i;
			}

			//Evaluate new key
			child_score = evaluate(decrypt(1, i, parent_key, text));

			if (child_score > top_score)
			{
				top_i = 1;
				top_key = parent_key;
				top_score = child_score;
				top_len = i;
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
			std::cout << "Best start: " << top_i + 1 << std::endl;
			std::cout << "Plaintext:\n\n    " << decrypt(top_i, top_len, top_key, text) << std::endl;
		}
	}
};