#ifndef __DEVICE_INCLUDED__
#define __DEVICE_INCLUDED__

#include <deque>

class Device
{
public:
	unsigned m_addr;
	int m_interrupt;

	unsigned getInput();
	void getOutput(unsigned);
	void update();
};

class Keyboard : public Device
{
private:
	std::deque<unsigned> m_log;
public:
	Keyboard();
	unsigned getInput();
	void getOutput(unsigned);
	void update();
};

#endif