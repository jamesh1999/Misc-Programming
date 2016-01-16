#include <iostream>
#include <algorithm>
#include <sstream>
#include <float.h>
#include "FourSquare.h"
#include "Globals.h"
#include "decrypt.h"

//Decrypt text with key
std::string decryptFoursquare(const std::string& key1, const std::string& key2, std::string text)
{
	for (unsigned i = 0; i < text.size() - 1; i += 2)
	{
		int a_ind = key1.find_first_of(text[i]);
		int b_ind = key2.find_first_of(text[i + 1]);
		text[i]     = 5 * (a_ind / 5) + (b_ind % 5) + 'A'; //Work out row and height from keys then decrypt
		text[i + 1] = 5 * (b_ind / 5) + (a_ind % 5) + 'A'; //"
	}

	return text;
}

//Cracks foursquare cipher via simulated annealing algorithm
void fourSquare(std::string text)
{
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

			double eval = evaluate(decryptFoursquare(child_key1, child_key2, text));

			//If key is better then always switch
			if (eval - parent_eval >= 0)
			{
				parent_eval = eval;
				parent_key1 = child_key1;
				parent_key2 = child_key2;
			}
            //Otherwise switch if temperature is high enough
            else if (T>0 && exp((eval - parent_eval) / T) > 1.0*rand() / (double)RAND_MAX)
            {
                parent_eval = eval;
                parent_key1 = child_key1;
                parent_key2 = child_key2;
            }
		}
        std::stringstream ss;
        ss << "\nCurrent score: " << parent_eval << "\nCurrent Temp.: " << T << "\nBest key:  " << parent_key1 << ", " << parent_key2 << std::endl;
        pWorker->appendToConsole(QString::fromStdString(ss.str()));

        pWorker->setPlainTextOutput(QString::fromStdString(decryptFoursquare(parent_key1, parent_key2, text)));
	}

}
