#include "stdafx.h"
#include "register.h"

Register::Register()
{
	m_value = 0;
}

void Register::clear()
{
	m_value = 0;
}

unsigned Register::getValue()
{
	return m_value;
}

void Register::setValue(unsigned val)
{
	m_value = val;
}

void Register::increment()
{
	m_value++;
}