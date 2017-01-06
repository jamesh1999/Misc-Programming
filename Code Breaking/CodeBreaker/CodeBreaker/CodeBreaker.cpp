// CodeBreaker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <vector>
#include <thread>
#include "Globals.h"
#include "SimpleSubstitution.h"
#include "Vigenere.h"
#include "FourSquare.h"
#include "Bifid.h"
#include "Trifid.h"
#include "ColumnarTransposition.h"
#include "RailFence.h"
#include "PolybiusSquare.h"
#include "Playfair.h"
#include "Amsco.h"
#include "BinaryConvert.h"
#include "Cadenus.h"
#include "Hill.h"


typedef std::basic_string<TCHAR> tstring;


int _tmain(int argc, _TCHAR* argv[])
{
	
	srand(time(NULL)); //Set the seed for the RNG

	//Print the correct usage if wrong
	if (argc != 2)
	{
		std::cout << "Usage: CodeBreaker.exe filename.txt" << std::endl;
		return 0;
	}


	//Attempt to read file
	std::string text;

	try
	{
		tstring filename = argv[1];

		std::ifstream ifile;
		ifile.open(filename);
	
		std::string line;

		while(std::getline(ifile,line))
		{
			text+=line+'\n';
		}

		ifile.close();

	}
	catch(int)
	{
		std::cout << "That is not a valid filename!" << std::endl;
		return 0;
	}

	//Remove all punctuation (except #s and numbers) and convert to upper case
	std::regex e("[^A-Z0-9#]");
	std::transform(text.begin(), text.end(),text.begin(), ::toupper);
	text = std::regex_replace(text, e,"");

	std::cout << "What method of decryption would you like to use?" << std::endl;

	//Identify most likely ciphers
	std::vector<char> identified;
	unsigned lcnt = 0; //Number of different characters
	unsigned etacnt = 0; //Total frequency of ETA
	for (auto c : text)
	{
		if (std::find(identified.begin(), identified.end(), c) == identified.end())
		{
			++lcnt;
			identified.push_back(c);
		}

		if (c == 'E' || c == 'T' || c == 'A')
		{
			++etacnt;
		}
	}

	//Index of coincidence
	double ic = indexCoincidence(text);

	//Output most likely ciphers
	if (lcnt == 5)
	{
		std::cout << "p - Polybius Square" << std::endl; //Codes made of 5 different letters
	}
	else
	{
		if (std::abs(ENGLISH_IC - ic) < ENGLISH_IC_DIFFERENCE && etacnt > text.size() / 20)
		{
			std::cout << "a - Amsco\nc - Columnar Transposition\nd - Cadenus\nr - Rail Fence" << std::endl; //Have same letter frequency as english
		}
		else if (std::abs(ENGLISH_IC - ic) < ENGLISH_IC_DIFFERENCE)
		{
			std::cout << "s - Simple Substitution" << std::endl; //Have same letter spread as english
		}
		else if (lcnt <= 25)
		{
			std::cout << "b - Bifid\nf - Foursquare\nl - Playfair" << std::endl; //Maximum of 25 letters
		}
		else if (lcnt == 27)
		{
			std::cout << "t - Trifid" << std::endl; //Uses 27 letter alphabet
		}
		else
		{
			std::cout << "v - Vigenere" << std::endl; //Uses 26 letter alphabet
		}

	}

	std::cout << "\n\na - Amsco\nb - Bifid\nc - Columnar Transposition\nd - Cadenus\nf - Foursquare\nl - Playfair\np - Polybius Square" <<
		"\nr - Rail Fence\ns - Simple Substitution\nt - Trifid\nv - Vigenere\nz - Binary Conversion\n\n" << std::endl; //Display entire list of codes

	bool done = false;
	char type;

	while(!done)
	{
		try
		{
			std::cin >> type;
			done = true;

			//Insert other cipher keys here
			switch(type)
			{
			case 'a':
				amsco(text);
				break;
			case 'b':
				bifid(text);
				break;
			case 'c':
				columnarTransposition(text);
				break;
			case 'd':
				cadenus(text);
				break;
			case 'f':
				fourSquare(text);
				break;
			case 'h':
				hill(text);
				break;
			case 'l':
				playfair(text);
				break;
			case 'p':
				polybiusSquare(text);
				break;
			case 'r':
				railFence(text);
				break;
			case 's':
				simpleSubstitution(text);
				break;
			case 't':
				trifid(text);
				break;
			case 'v':
				vigenere(text);
				break;
			case 'z':
				binary(text);
				break;
			default:
				done = false;
				std::cout << "That is not a valid answer!" << std::endl;
				break;
			}

		}
		catch(int)
		{
			std::cout << "That is not a valid answer!" << std::endl;
		}
	}

	std::cout << "The program has finished. ";
	system("pause");

	return 0;
}

