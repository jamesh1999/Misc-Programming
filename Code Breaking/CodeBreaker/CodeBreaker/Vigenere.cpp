#include "stdafx.h"
#include <vector>
#include <iostream>
#include "Vigenere.h"
#include "Globals.h"



void vigenere(std::string text)
{

	//Calculate IC for each key length < 50
	std::vector<double> rates;
	for(int i = 2; i<50; ++i)
	{
		std::vector<std::string> lists;

		unsigned long cnt = 0;
		for(auto letter : text)
		{
			//Add new list if required
			if(lists.size()<(cnt%i)+1)
			{
				lists.push_back("");
			}
			
			lists[cnt%i]+=letter; //Add to list[key index used to encrypt]

			++cnt;
		}

		//Calculate ave IC
		double ave_index_coincidence = 0;
		for(auto list : lists)
		{
			ave_index_coincidence+=indexCoincidence(list);
		}
		ave_index_coincidence/=i;

		rates.push_back(ave_index_coincidence);
	}

	//Find the first key whose value is very good
	double max = 0;
	unsigned k_len = 0;

	unsigned cnt = 2;
	for(auto val : rates)
	{
		if (std::abs(ENGLISH_IC - val)<std::abs(ENGLISH_IC - max))
		{
			max = val;
			k_len = cnt;
		}

		++cnt;
	}

	//Decrypt text with key
	auto decrypt = [](std::string key, std::string text)
	{
		unsigned cnt = 0;
		for(auto& letter : text)
		{
			letter = letter - key[cnt] + 'A';

			if(letter<'A')
			{
				letter+=26;
			}

			++cnt;

			if(cnt==key.size())
			{
				cnt = 0;
			}
		}
		return text;
	};

	std::string prev_key(k_len, 'A');
	std::string cur_key(k_len, 'A');

	//Keep making changes until no more improvements can be made
	bool same = false;
	while(!same)
	{
		//Loop through letters in key
		for(unsigned i = 0; i < k_len; ++i)
		{
			//Find letter that creates best value
			char max_char = 'A';
			double max_char_val = -DBL_MAX;
			for(int j = 0; j<26; ++j)
			{
				cur_key[i] = 'A'+j;
				double eval = evaluate(decrypt(cur_key,text));

				if(eval>max_char_val)
				{
					max_char = 'A' + j;
					max_char_val = eval;
				}
			}

			cur_key[i] = max_char;
		}

		if(cur_key==prev_key)
		{
			same = true;
		}
		else
		{
			prev_key = cur_key;
		}
	}

	std::string decrypted = decrypt(cur_key, text);
	system("cls");
	std::cout << "\n\nBest score: " << evaluate(decrypted) << std::endl;
	std::cout << "Best key:  " << cur_key << std::endl;
	std::cout << "Key length used:  " << k_len << std::endl;
	std::cout << "Plaintext:\n\n    " << decrypted << std::endl;
}