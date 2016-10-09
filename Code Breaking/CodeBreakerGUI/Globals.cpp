#include <string>
#include "Globals.h"
#include "Bigrams.h"
#include "Trigrams.h"
#include "Quadgrams.h"

float ENGLISH_IC = 0.067f;
float ENGLISH_IC_DIFFERENCE = 0.005f;
float TEMP = 20.0f;
float STEP = 0.1f;
unsigned COUNT = 10000;
int LENGTH = 4;
unsigned MAX_RAILS = 99;



//Calculates IC for string text
double indexCoincidence(const std::string& text)
{
    double ic = 0;

	//Sum of f(f-1) for each letter
    for(unsigned i = 0; i < 27; ++i)
	{
		char letter = 'A' + i;
		long cnt = 0;

		//Count frequency of letter
		for(auto l : text)
		{
			if(l == letter)
				++cnt;
		}

        ic += cnt * (cnt - 1);
	}

    return ic / (text.length() * (text.length() - 1));
}

//Score text based on ngram frequencies
double evaluate(const std::string& text)
{
	double score = 0;

    switch(LENGTH)
	{
    case 2:
        for(unsigned i = 0; i < text.length() - 1; ++i)
        {
			if (text[i] == '#' || text[i + 1] == '#')
				score -= 9;
			else
                score += bigram_freq[TO_INT(text[i]) * 26 +
										TO_INT(text[i + 1])];
		}
        break;

    case 3:
        for(unsigned i = 0; i < text.length() - 2; ++i)
        {
            if (text[i] == '#' || text[i + 1] == '#' || text[i + 2] == '#')
                score -= 10;
            else
                score += trigram_freq[TO_INT(text[i]) * 676 +
										TO_INT(text[i + 1]) * 26 +
										TO_INT(text[i + 2])];
        }
        break;

    case 4:
        for(unsigned i = 0; i < text.length() - 3; ++i)
        {
            if (text[i] == '#' || text[i + 1] == '#' || text[i + 2] == '#' || text[i + 3] == '#')
                score -= 13;
            else
                score += quadgram_freq[TO_INT(text[i]) * 17576 +
										TO_INT(text[i + 1]) * 676 +
										TO_INT(text[i + 2]) * 26 +
										TO_INT(text[i + 3])];
        }
        break;
	}
	return score;
}
