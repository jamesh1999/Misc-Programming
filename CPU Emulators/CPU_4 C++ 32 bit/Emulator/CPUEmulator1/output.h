#ifndef __OUTPUT_INCLUDED__
#define __OUTPUT_INCLUDED__

#include <vector>
#include "device.h"

class OutputBus
{
private:
	std::vector<Device> m_devices;
	Device m_cur_device;

public:
	int m_interrupt;

	OutputBus();

	unsigned set(unsigned,bool,bool);
	void update();
	void addDevice(Device);
};

#endif