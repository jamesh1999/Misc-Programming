#include <iostream>
#include <algorithm>
#include <float.h>
#include <vector>
#include "Solitaire.h"
#include "Globals.h"



//Cracks solitaire cipher via simulated annealing algorithm
void solitaire(std::string text)
{
    auto decrypt = [](std::vector<unsigned> key, std::string text)
	{
		int stream;
		std::vector<unsigned> top;
		std::vector<unsigned> mid;
		std::vector<unsigned> bottom;

		top.reserve(54);
		mid.reserve(54);
		bottom.reserve(54);
		int ayy = 0;
        for(auto& c : text)
        {
			//std::cout << "\n" << ayy << std::endl;
			++ayy;
            do
            {
                //Move jokers
                for(int i = 0; i < 54; ++i)
                    if(key[i] == 53)
                    {
                        if (i == 53)
                        {
                            for(int j = 52; j > 0; --j)
                                key[j + 1] = key[j];
                            key[1] = 53;
                        }
                        else
                        {
                            key[i] = key[i + 1];
                            key[i + 1] = 53;
                        }
                        break;
                    }
                for(int i = 0; i < 54; ++i)
                    if(key[i] == 54)
                    {
                        if(i == 53)
                        {
                            for(int j = 52; j > 1; --j)
                                key[j + 1] = key[j];
                            key[2] = 54;
                        }
                        else if(i == 52)
                        {
                            for(int j = 51; j > 0; --j)
                                key[j + 1] = key[j];
                            key[1] = 54;
                        }
                        else
                        {
                            key[i] = key[i + 1];
                            key[i + 1] = key[i + 2];
                            key[i + 2] = 54;
                        }
                        break;
                    }

                //Triple cut
				top.clear();
				mid.clear();
				bottom.clear();
                int first = 0;
                bool t = false;
                for(int i = 0; i < 54; ++i)
                    if(key[i] > 52)
                    {
                        if(t)
                        {
                            mid.insert(mid.end(), key.begin() + first, key.begin() + i + 1);
                            bottom.insert(bottom.end(), key.begin() + 1 + i, key.end());
                            break;
                        }
                        else
                        {
                            top.insert(top.end(), key.begin(), key.begin() + i);
                            first = i;
                            t = true;
                        }
                    }
                key.clear();
                key.insert(key.end(), bottom.begin(), bottom.end());
                key.insert(key.end(), mid.begin(), mid.end());
                key.insert(key.end(), top.begin(), top.end());

                //Count cut
				top.clear();
				bottom.clear();
                int cnt = key[key.size() - 1];
				//std::cout << cnt << std::endl;
				int orig = cnt;
                if(cnt == 54)
                    cnt = 53;
                top.insert(top.end(), key.begin(), key.begin() + cnt);
                bottom.insert(bottom.end(), key.begin() + cnt, key.end() - 1);
                key.clear();
                key.insert(key.end(), bottom.begin(), bottom.end());
                key.insert(key.end(), top.begin(), top.end());
                key.push_back(orig);

                //Get value
				//std::cout << key[0] << std::endl;
				if (key[0] == 54)
					stream = key[53];
				else
					stream = key[key[0]];
				//std::cout << stream << std::endl;
            } while(stream > 52);

            //Decrypt with next value in stream
            c = (c - 'A' + 52 - stream) % 26 + 'A';
        }
        return text;
	};

    double parent_eval = -DBL_MAX;
    std::vector<unsigned> parent_key = {19, 45, 28, 11, 50 };

	auto mix = [](const std::vector<unsigned> in)
	{
		std::vector<unsigned> ret = { 39, 35, 47, 4, 5, 42,
			17, 52, 20, 13, 53, 16,
			30, 40, 38, 34, 43, 14,
			41, 7, 27, 44, 1, 6,
			33, 15, 54, 36, 18, 24,
			3, 9, 51, 37, 23, 46,
			21, 10,12,26,49,
			25,2,22,8,
			31,48, 32, 29 };
		ret.insert(ret.begin() + 39, in.begin(), in.begin() + 1);
		ret.insert(ret.begin() + 42, in.begin() + 1, in.begin() + 2);
		ret.insert(ret.begin() + 44, in.begin() + 2, in.begin() + 3);
		ret.insert(ret.begin() + 51, in.begin() + 3, in.begin() + 4);
		ret.push_back(in.back());
		return ret;
	};
    //for(unsigned i = 1; i < 55; ++i)
        //parent_key.push_back(i);

	//Implement simulated annealing
	std::vector<unsigned> child_key = parent_key;
	do
	{
		//std::random_shuffle(child_key.begin(), child_key.end());
        double eval = evaluate(decrypt(mix(child_key), text));

		//If key is better then always switch
		if (eval - parent_eval >= 0)
		{
			parent_eval = eval;
            parent_key = child_key;

			std::cout << "\nCurrent score: " << parent_eval << "\nBest key:  ";
			for (auto x : parent_key)
				std::cout << x << ", ";
			std::cout << std::endl;

			std::cout << decrypt(mix(parent_key), text) << std::endl;
			system("pause");
		}
	} while (true);

}
