#include <iostream>
#include <bitset>
#include "BinaryConvert.h"

enum BinaryMethod { ASCII, ZERO, ONE };
//Decrypt a text using a particular method
std::string binaryDecrypt(int length, BinaryMethod method, const std::string& text)
{
	std::string result;
	for (unsigned i = 0; i < text.size(); i += length)
	{
		std::string next = text.substr(i, length);
		std::bitset<8> val(next);
		
		switch(method)
		{
		case ASCII:
			result += static_cast<char>(val.to_ulong());
			break;
		case ZERO:
			result += static_cast<char>(val.to_ulong() + 'A');
			break;
		case ONE:
			result += static_cast<char>(val.to_ulong() + 'A' - 1);
			break;
		}
	}

	return result;
}


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
                binaryDecrypt(length, ZERO, text);
				break;
			case '1':
                binaryDecrypt(length, ONE, text);
				break;
			case '2':
                binaryDecrypt(length, ASCII, text);
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
