#include "space.h"
#include "hashtable.h"
#include <fstream>
#include <cstring>
#include <vector>

#define MAX_LOOKAHEAD 4

Containers::HashTable<char*, bool> table(131072);

bool comp(char* const& x, char* const& y)
{
    return std::strcmp(x, y) == 0;
}

unsigned hash(char* const& key)
{
    unsigned index = 0;
    unsigned total = 0;
    unsigned buff = 0;
    while(key[index] != '\0')
    {
        buff <<= 4;
        buff += key[index];

        if(index % 5 == 0)
        {
            total += buff;
            buff = 0;
        }

        ++index;
    }
    return total + buff;
}

void initTable()
{

    table.comparisonFunction = comp;
    table.hashFunction = hash;
    table.defaultValue = false;

    //Load wordlists
    std::ifstream stream("words.txt");
    std::string buff;
    while(std::getline(stream, buff))
    {
        char* word = new char[buff.size() + 1];
        std::strcpy(word, buff.c_str());
        table[word] = true;
    }
    stream.close();
}

void closeTable()
{
    std::vector<char*> keys = table.GetKeys();
    for(char* key : keys)
    {
        table.RemoveKey(key);
        delete[] key;
    }
}

bool isWord(const char* test)
{
    return table.DoesContain(const_cast<char*>(test));
}

int getLookaheadCount(const std::string& text, unsigned start, int depth)
{
    if(depth == MAX_LOOKAHEAD)
        return 0;

    int max_depth = 0;
    for (unsigned i = 15; i > 0; --i)
    {
        if(i + start > text.size()) continue;

        if(isWord(text.substr(start, i).c_str()))
        {
            int test = getLookaheadCount(text, start + i, depth + 1);
            if(test > max_depth)
                max_depth = test;
        }
    }

    return max_depth + 1;
}

std::string addSpaces(std::string text)
{
	std::string spaced = "";
	std::string buffer = "";
    unsigned processed = 0;
    while (processed < text.size())
	{
		int max = 0;
		int word_len = 0;
        for (unsigned i = 15; i > 0; --i)
		{
            if(i + processed > text.size()) continue;
            if (isWord(text.substr(processed, i).c_str()))
			{
                int lookaheadCount = getLookaheadCount(text, i + processed, 0);
				if (lookaheadCount == MAX_LOOKAHEAD)
				{
					word_len = i;
					break;
				}
				else if(lookaheadCount > max)
				{
					word_len = i;
					max = lookaheadCount;
				}
			}
		}

		if(word_len == 0)
		{
            buffer += text[processed];
			word_len = 1;
		}
		else
		{
			if(buffer.size() > 0)
			{
				spaced += buffer + " ";
				buffer = "";
			}

            spaced += text.substr(processed, word_len) + " ";
		}
        processed += word_len;
	}
    spaced += buffer;
	return spaced;
}
