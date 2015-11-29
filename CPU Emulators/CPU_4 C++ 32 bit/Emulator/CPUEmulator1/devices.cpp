#include "stdafx.h"
#include "device.h"
#include <windows.h>
#include <iostream>

Keyboard::Keyboard()
{
	m_interrupt = 0;
}

void Keyboard::update()
{
	char pressed;
	while(std::cin >> pressed)
	{
		m_log.push_back((unsigned)pressed);
	}
}

unsigned Keyboard::getInput()
{
	unsigned val = m_log[0];
	m_log.pop_front();
	return val;
}

void Keyboard::getOutput(unsigned data)
{}