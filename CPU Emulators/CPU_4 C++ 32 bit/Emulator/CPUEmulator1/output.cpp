#include "stdafx.h"
#include "output.h"

OutputBus::OutputBus()
{
	m_interrupt = 0;
}

unsigned OutputBus::set(unsigned data, bool i, bool addr)
{
	if(addr)
	{
		for(auto &device : m_devices)
		{
			if(device.m_addr == data)
			{
				m_cur_device = device;
			}
		}
	}
	else if(i)
	{
		return m_cur_device.getInput();
	}
	else
	{
		m_cur_device.getOutput(data);
	}

	return 0;
}

void OutputBus::addDevice(Device device)
{
	m_devices.push_back(device);
}

void OutputBus::update()
{
	for(auto &device : m_devices)
	{
		device.update();
		if(device.m_interrupt)
		{
			m_interrupt = 1;
		}
	}
}