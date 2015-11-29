#include "stdafx.h"
#include <iostream>
#include <bitset>
#include "BinaryConvert.h"

//Convert from binary into ciphertext/plaintext
void binary(std::string text)
{
	system("cls");

	std::cout << "Length per letter (1-8): " << std::endl;

	int length;

	std::cin >> length;

	system("cls");
	
	std::cout << "Which method should be used to decode the text?\n0 - A=0\n1 - A=1\n2 - ASCII\n\n" << std::endl;

	bool done = false;
	char type;

	while (!done)
	{
		try
		{
			std::cin >> type;
			done = true;

			//Select conversion to use
			switch (type)
			{
			case '0':
				a0(length, text);
				break;
			case '1':
				a1(length, text);
				break;
			case '2':
				ascii(length, text);
				break;
			default:
				done = false;
				std::cout << "That is not a valid answer!" << std::endl;
				break;
			}

		}
		catch (int)
		{
			std::cout << "That is not a valid answer!" << std::endl;
		}
	}

};

void a0(int length, std::string text)
{
	std::string result;

	for (unsigned i = 0; i < text.size(); i += length)
	{
		std::string next = text.substr(i, length);
		std::bitset<8> val(next);
		unsigned long lval = val.to_ulong();

		result += 'A' + lval;
	}

	system("cls");

	std::cout << "Result:\n" << result << std::endl;
}

void a1(int length, std::string text)
{
	std::string result;

	for (unsigned i = 0; i < text.size(); i += length)
	{
		std::string next = text.substr(i, length);
		std::bitset<8> val(next);
		unsigned long lval = val.to_ulong();

		result += 'A' + lval - 1;
	}

	system("cls");

	std::cout << "Result:\n" << result << std::endl;
}

void ascii(int length, std::string text)
{
	std::string result;

	for (unsigned i = 0; i < text.size(); i += length)
	{
		std::string next = text.substr(i, length);
		std::bitset<8> val(next);
		unsigned long lval = val.to_ulong();

		result += static_cast<char>(lval);
	}

	system("cls");

	std::cout << "Result:\n" << result << std::endl;
}