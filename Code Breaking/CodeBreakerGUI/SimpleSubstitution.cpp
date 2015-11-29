#include <iostream>
#include <sstream>
#include <algorithm>
#include <float.h>
#include "SimpleSubstitution.h"
#include "Globals.h"
#include "decrypt.h"

//Crack simple substitution ciphers
void simpleSubstitution(std::string text)
{

	//Decrypt text with key
    auto decrypt = [](const std::string& key, const std::string& text)
	{
        std::string ret = "";
        for(auto letter : text)
            ret += key[letter - 'A'];

        return ret;
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

        for (int cnt = 0; cnt < 675; ++cnt)
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
                cnt = -1;
			}
		}

		//Print results if better
		if(parent_score>max_score)
		{
			max_score = parent_score;
			max_key = parent_key;

            std::stringstream ss;
            ss << "\nBest score: " << max_score << "\nBest key:  " << max_key << std::endl;
            pWorker->appendToConsole(QString::fromStdString(ss.str()));

            pWorker->setPlainTextOutput(QString::fromStdString(decrypt(max_key, text)));
		}
	}
}
