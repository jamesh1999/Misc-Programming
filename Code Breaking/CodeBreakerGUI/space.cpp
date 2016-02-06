#include "space.h"
#include "smallwords.h"
#include <QtSql/QtSql>
#include <iostream>

#define MAX_LOOKAHEAD 3
#define TO_INT(chr) (chr - 'A')

 QSqlDatabase db;
 int sizes[MAX_LOOKAHEAD] = {0};

void initDb()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("words.db");
    db.open();
}

void closeDb()
{
    db.close();
}

bool isWord(const char* test)
{
    switch(strlen(test))
    {
    case 1:
        return words_one[TO_INT(test[0])];
    case 2:
        return words_two[TO_INT(test[0]) * 26 + TO_INT(test[1])];
    case 3:
        return words_three[TO_INT(test[0]) * 676 + TO_INT(test[1]) * 26 + TO_INT(test[2])];
    case 4:
        return words_four[TO_INT(test[0]) * 17576 + TO_INT(test[1]) * 676 + TO_INT(test[2]) * 26 + TO_INT(test[3])];
    default:
    {
        QString stmnt = "SELECT id FROM ";
        stmnt += test[0] + 32;
        stmnt += " WHERE word = '";
        stmnt += test;
        stmnt += "'";
        QSqlQuery query;
        query.exec(stmnt);
        while(query.next())
            return true;
        return false;
    }
    }
}

int getLookaheadCount(const std::string& text, int start, int depth)
{
    if(depth == MAX_LOOKAHEAD)
        return 0;

    int max_depth = 0;
    for(int i = 1; i <= 15; ++i)
    {
        if(i + start > text.size()) break;

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
    int processed = 0;
    while (processed < text.size())
	{
		int max = 0;
		int word_len = 0;
        for (int i = 1; i <= 15; ++i)
		{
            if(i + processed > text.size()) break;
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
