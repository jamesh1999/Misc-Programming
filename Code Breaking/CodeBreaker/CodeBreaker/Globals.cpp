#include "stdafx.h"
#include <string>
#include <iostream>
#include "Globals.h"
#include "Bigrams.h"
#include "Trigrams.h"
#include "Quadgrams.h"



//Calculates IC for string text
double indexCoincidence(std::string text)
{
	double kp = 0;

	//Sum of f(f-1) for each letter
	for(unsigned i=0; i<26; ++i)
	{
		char letter = 'A'+i;
		long cnt = 0;

		//Count frequency of letter
		for(auto l : text)
		{
			if(l == letter)
			{
				++cnt;
			}
		}

		kp += cnt*(cnt-1);
	}

	return kp/(text.length()*(text.length()-1));
}



//Score text based on ngram frequencies
double evaluate(std::string text)
{
	double score = 0;

	if(LENGTH == 2)
	{
		for(unsigned i = 0; i<text.length()-LENGTH+1; ++i)
		{
			if (text[i] == '#' || text[i + 1] == '#')
			{
				score -= 9;
			}
			else
			{
				score += bigram_freq[(text[i] - 'A') * 26 + (text[i + 1] - 'A')];
			}
		}
	}
	else if(LENGTH == 3)
	{
		for(unsigned i = 0; i<text.length()-LENGTH+1; ++i)
		{
			if (text[i] == '#' || text[i + 1] == '#' || text[i + 2] == '#')
			{
				score -= 10;
			}
			else
			{
				score += trigram_freq[(text[i] - 'A') * 676 + (text[i + 1] - 'A') * 26 + (text[i + 2] - 'A')];
			}
		}
	}
	else if(LENGTH == 4)
	{
		for(unsigned i = 0; i<text.length()-LENGTH+1; ++i)
		{
			if (text[i] == '#' || text[i + 1] == '#' || text[i + 2] == '#' || text[i + 3] == '#')
			{
				score -= 13;
			}
			else
			{
				score += quadgram_freq[(text[i] - 'A') * 17576 + (text[i + 1] - 'A') * 676 + (text[i + 2] - 'A') * 26 + (text[i + 3] - 'A')];
			}
		}
	}

	return score;
}