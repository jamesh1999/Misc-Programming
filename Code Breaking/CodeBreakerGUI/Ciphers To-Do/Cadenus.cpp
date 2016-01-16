#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <float.h>
#include "Cadenus.h"
#include "Globals.h"
#include "decrypt.h"

//Decrypt text with key
std::string decryptCadenus(const std::vector<unsigned>& order_key, const std::vector<unsigned>& shift_key, const std::string& text)
{
	std::vector<std::string> columns(order_key.size(), "");
	for (unsigned i = 0; i < text.size(); ++i)
		columns[order_key[i % order_key.size()]] += text[i];

	std::string result;
	for (int i = 0; i < 25; ++i)
		for (unsigned j = 0; j < order_key.size(); ++j)
			result += columns[j][(shift_key[order_key[j]] + i) % 25];

	return result;
}

//Cracks cadenus cipher via simulated annealing algorithm
void cadenus(std::string text)
{
	std::vector<unsigned> top_key1;
	std::vector<unsigned> top_key2;
	double top_eval = -DBL_MAX;

	std::vector<unsigned> parent_key1;
	for (unsigned i = 0; i < text.size() / 25; ++i)
		parent_key1.push_back(i);
	std::random_shuffle(parent_key1.begin(), parent_key1.end());

	std::vector<unsigned> parent_key2;
	for (unsigned i = 0; i < text.size() / 25; ++i)
		parent_key2.push_back(rand() % 25);

	double parent_eval = -DBL_MAX;

	//Implement simulated annealing
	for (double T = TEMP; T >= 0; T -= STEP)
	{
		for (unsigned i = 0; i < COUNT; ++i)
		{
			std::vector<unsigned> child_key1 = parent_key1;
			std::vector<unsigned> child_key2 = parent_key2;

			//Swap columns or change shift in one column
			if (rand() % child_key1.size())
			{
				child_key2[rand() % child_key2.size()] = rand() % 25;
			}
			else
			{
				std::swap(child_key1[rand() % child_key1.size()], child_key1[rand() % child_key1.size()]);
			}

			double eval = evaluate(decryptCadenus(child_key1, child_key2, text));

			//If key is better then always switch
			if (eval > parent_eval)
			{
				parent_eval = eval;
				parent_key1 = child_key1;
				parent_key2 = child_key2;

				if (eval > top_eval)
				{
					top_eval = eval;
					top_key1 = child_key1;
					top_key2 = child_key2;
				}
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
        ss << "\nCurrent score: " << top_eval << "\nCurrent Temp.: " << T << "\nBest Key: ";
        for (auto v : top_key1)
        {
            ss << v << " ";
        }
        std::cout << ", ";
        for (auto v : top_key2)
        {
            ss << v << " ";
        }
        ss << std::endl;
        pWorker->appendToConsole(QString::fromStdString(ss.str()));

        pWorker->setPlainTextOutput(QString::fromStdString(decryptCadenus(top_key1, top_key2, text)));
    }
}
