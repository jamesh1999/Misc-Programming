#ifndef __RAM_INCLUDED__
#define __RAM_INCLUDED__

#include <vector>

const int AMNT = 131072; //Amount of RAM to use in bytes



//RAM is written and read as in a big-endian system



class RAM
{
private:
	unsigned char* m_data;
public:
	RAM() : m_data(new unsigned char[AMNT]) //Allocate memory
	{
		//Initialize RAM to 0
		for (int i = 0; i < AMNT; ++i)
			m_data[i] = 0;
	}

	~RAM(){ delete[] m_data; }

	//Takes a program and loads it into RAM from 'start' onwards
	void loadProgram(std::vector<unsigned char> data, unsigned start)
	{
		for (auto b = data.cbegin(); b != data.cend(); ++b)
			m_data[(b - data.cbegin()) + start] = *b;
	}

	//Writes 'data' to 'len' bytes of RAM
	void set(const unsigned& len, unsigned data, const unsigned& addr)
	{
		if (addr + len >= AMNT)
		{
			std::cerr << "Attempted to access a non-existant RAM address: " << addr << ", " << len << std::endl;
			system("pause");
			exit(-1);
		}

		data <<= 8 * (4 - len);
		for (unsigned cnt = 0; cnt < len; ++cnt)
		{
			m_data[addr + cnt] = data / 16777216; //Write highest byte of data
			data <<= 8;
		}
	}

	//Read 'len' bytes of RAM
	unsigned read(const unsigned& len, const unsigned& addr)
	{
		unsigned data = 0;
		for (unsigned i = 0; i < len; ++i)
			data = (data << 8) + m_data[i + addr];

		return data;
	}
};

#endif