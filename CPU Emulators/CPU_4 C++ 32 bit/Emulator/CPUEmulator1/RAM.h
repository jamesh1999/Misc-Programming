#ifndef __RAM_INCLUDED__
#define __RAM_INCLUDED__

#include <vector>

class RAM
{
private:
	unsigned m_data[1000];
public:
	RAM();

	void loadProgram(std::vector<unsigned>, unsigned);
	void set(unsigned,unsigned);
	unsigned read(unsigned);
};

#endif