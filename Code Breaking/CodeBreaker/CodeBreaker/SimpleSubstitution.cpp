#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include "SimpleSubstitution.h"
#include "Globals.h"

//Crack simple substitution ciphers
void simpleSubstitution(std::string text)
{

	//Decrypt text with key
	auto decrypt = [](std::string key, std::string text)
	{
		for(auto& letter : text)
		{
			letter = key[letter - 'A'];
		}
		return text;
	};

	//Overall max key
	std::string max_key = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	double max_score = -DBL_MAX;

	//Key for iteration
	std::string parent_key = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	double parent_score = -DBL_MAX;

	for (unsigned iter = 0; iter < 50; ++iter)
	{
		//Generate and evaluate random parent
		std::random_shuffle(parent_key.begin(),parent_key.end());
		parent_score = evaluate(decrypt(parent_key,text));

		for (unsigned cnt = 0; cnt < 675; ++cnt)
		{
			//Clone the parent key
			std::string child_key = parent_key;

			//Swap next two characters
			std::swap(child_key[cnt/26],child_key[cnt%26]);

			//Evaluate new key
			double child_score = evaluate(decrypt(child_key,text));

			//Compare scores changing parent if child is better
			if(child_score>parent_score)
			{
				parent_key = child_key;
				parent_score = child_score;
				cnt = 0;
			}
		}

		//Print results if better
		if(parent_score>max_score)
		{
			max_score = parent_score;
			max_key = parent_key;

			system("cls");
			std::cout << "\n\nBest score: " << max_score << std::endl;
			std::cout << "Iterations: " << iter << std::endl;
			std::cout << "Best key:  " << max_key << std::endl;
			std::cout << "Plaintext:\n\n    " << decrypt(max_key,text) << std::endl;
		}

		//Check for more iterations specified
		if(iter==50)
		{
			std::cout << "Do you want to do another 50 iterations? Y/N\n" << std::endl;
			char result;
			std::cin >> result;
			if(tolower(result)=='y')
			{
				iter = 0;
			}
		}
	}
}