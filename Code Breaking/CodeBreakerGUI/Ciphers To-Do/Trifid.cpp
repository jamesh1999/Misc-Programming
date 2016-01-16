#include <iostream>
#include <algorithm>
#include <sstream>
#include <float.h>
#include "Trifid.h"
#include "Globals.h"
#include "decrypt.h"

//Decrypt text with key
std::string decryptTrifid(int period, const std::string& key, const std::string& text)
{

	std::string result = text;

	for (unsigned i = 0; i < text.length(); i += period)
	{
		if (i + period > text.length())
			period = text.length() - i;

		for (int j = 0; j < period; ++j)
		{
			//Letters that were encoded by the letter i+j
			char a = text[i + (j / 3)];
			char b = text[i + ((j + period) / 3)];
			char c = text[i + ((j + (2 * period)) / 3)];

			//Get the coordinates of the letters that came from letter i+j
			int a_part = key.find_first_of(a) / (int)std::pow(3, (j % 3)) % 3;
			int b_part = key.find_first_of(b) / (int)std::pow(3, ((j + period) % 3)) % 3;
			int c_part = key.find_first_of(c) / (int)std::pow(3, ((j + (2 * period)) % 3)) % 3;

			result[i + j] = key[a_part + b_part * 3 + c_part * 9];
		}
	}

	return result;
}

//Crack simple substitution ciphers
void trifid(std::string text)
{

	int period;
	std::cout << "What period do you want to use? " << std::endl;
	std::cin >> period;

	if (!period)
	{
		period = text.length();
	}

	//Overall max key
	std::string parent_key = "ABCDEFGHIJKLMNOPQRSTUVWXYZ#";
	double parent_eval = -DBL_MAX;

	std::random_shuffle(parent_key.begin(), parent_key.end());

	//Implement simulated annealing
	for (double T = TEMP; T >= 0; T -= STEP)
	{
		for (unsigned cnt = 0; cnt < 728; ++cnt)
		{
			std::string child_key = parent_key;

			//Swap letters
			std::swap(child_key[cnt / 27], child_key[cnt % 27]);

			double eval = evaluate(decryptTrifid(period, child_key, text));

			//If key is better then always switch
			if (eval - parent_eval >= 0)
			{
				parent_eval = eval;
				parent_key = child_key;
			}
            //Otherwise switch if temperature is high enough
            else if (T>0 && exp((eval - parent_eval) / T) > 1.0*rand() / (double)RAND_MAX)
            {
                parent_eval = eval;
                parent_key = child_key;
            }
		}
        std::stringstream ss;
        ss << "\nCurrent score: " << parent_eval << "\nCurrent Temp.: " << T << "\nBest key:  " << parent_key << std::endl;
        pWorker->appendToConsole(QString::fromStdString(ss.str()));

        pWorker->setPlainTextOutput(QString::fromStdString(decryptTrifid(period, parent_key, text)));
	}
}
