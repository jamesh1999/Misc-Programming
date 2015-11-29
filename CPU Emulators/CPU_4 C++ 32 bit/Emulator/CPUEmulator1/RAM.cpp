#include "stdafx.h"
#include "RAM.h"
#include <iostream>

RAM::RAM()
{
	for(auto &val : m_data)
	{
		val = 0;
	}
}

void RAM::loadProgram(std::vector<unsigned> data, unsigned start)
{
	for(auto b = data.cbegin(); b!=data.cend();b++)
	{
		unsigned cnt = b-data.cbegin();
		m_data[cnt+start] = *b;
	}
}

void RAM::set(unsigned data, unsigned addr)
{
	m_data[addr] = data;
}

unsigned RAM::read(unsigned addr)
{
	std::cout << addr << std::endl;
	return m_data[addr];
}