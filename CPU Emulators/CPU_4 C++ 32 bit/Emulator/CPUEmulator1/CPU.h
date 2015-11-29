#ifndef __CPU_INCLUDED__
#define __CPU_INCLUDED__

#include "RAM.h"
#include "ALU.h"
#include "register.h"
//#include "output.h"
#include <string>

class CPU
{
private:
	RAM m_ram;
	ALU m_alu;
	Register m_ir;
	Register m_iar;
	Register m_registers[256];
	//OutputBus m_out_bus;
	bool m_done;
	int m_cpu_flag;

	void loop();
	//void addDevice(Device, unsigned);
	void operation();
	void read(Register, Register);
	void data(Register);
	void write(Register, Register);
	void writeTo(Register);
	void clone(Register, Register);
	void jump(Register);
	void jumpTo();
	//void getIn(Register);
	//void setOut(Register);
	//void dataOut(Register);
	//void outFrom(Register);
	//void address(Register);
	void outputD(Register);
	void outputB(Register);
	void setFlag();
	void clearFlags();
	//void clearInterrupt();
	void halt();
public:
	CPU();

	void start(std::string);
};

#endif