// CPUEmulator1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CPU.h"


#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

/*
#32 bit cpu with 255gp+ia registers (all addressable)

#--------------------INSTRUCTION SET--------------------#
#
#ALU instructions input ra and rb and output into ra (CMP has no output) (unary operations take rb as input)
#_______________________________________________________
#| OPCODE   |   Registers/Args   |   Flags   |  Assem.  |
#|----------|--------------------|-----------|----------|
# 00000000    ra        00000000    olcaezdi    ZERO     ||FLAGS-(o=On/Use flags, l=Logic 0:OR/1:AND, c=Carry, a=ra>rb, e=ra=rb, z=Output Zero, d=CPU Flag, i=Interrupt)
# 00000001    ra        00000000    olcaezdi    ONE
# 00000010    ra        rb          olcaezdi    ADD
# 00000011    ra        rb          olcaezdi    ADDC
# 00000100    ra        00000000    olcaezdi    INC
# 00000101    ra        rb          olcaezdi    SUB
# 00000110    ra        rb          olcaezdi    SUBC
# 00000111    ra        rb          olcaezdi    RSUB     ||Reverse subtract e.g. ra=rb-ra
# 00001000    ra        rb          olcaezdi    RSUBC
# 00001001    ra        00000000    olcaezdi    DEC
# 00001010    ra        rb          olcaezdi    MUL
# 00001011    ra        00000000    olcaezdi    RAND     ||Set ra to a random binary number
# 00010000    ra        00000000    olcaezdi    SHL
# 00010001    ra        00000000    olcaezdi    SHLC
# 00010010    ra        00000000    olcaezdi    SHR
# 00010011    ra        00000000    olcaezdi    SHRC
# 00010100    ra        00000000    olcaezdi    NOT
# 00010101    ra        rb          olcaezdi    AND
# 00010110    ra        rb          olcaezdi    NAND
# 00010111    ra        rb          olcaezdi    OR
# 00011000    ra        rb          olcaezdi    NOR
# 00011001    ra        rb          olcaezdi    XOR
# 00011010    ra        rb          olcaezdi    XNOR
# 00011011    ra        rb          olcaezdi    BCLR     ||Set ra to ra AND NOT rb
# 00011111    ra        rb          olcaezdi    CMP
#
#
#
#RAM/Register operations
# 00100000    ra        rb          olcaezdi    READ     ||Set ra to data at rb
# 00100001    ra        00000000    olcaezdi    DATA     ||Set ra to data in next byte
# 00100010    ra        rb          olcaezdi    WRITE    ||Write ra to rb
# 00100011    ra        00000000    olcaezdi    WRITETO  ||Write ra to address in next byte
# 00100100    ra        rb          olcaezdi    CLONE    ||Set ra to rb
#
#
#
#Jump operations
# 01000000    00000000  ra          olcaezdi    JUMP     ||Jump to ra
# 01000001    00000000  00000000    olcaezdi    JUMPTO   ||Jump to address in next byte
#
#
#
#Output operations
# 10000000    ra        00000000    olcaezdi    IN       ||Gets input from current device
# 10000001    ra        00000000    olcaezdi    OUT      ||Outputs to current device
# 10000010    00000000  00000000    olcaezdi    DATAO    ||Output data in next byte
# 10000011    00000000  00000000    olcaezdi    OUTFRM   ||Output data from address in next byte
# 10000100    ra        00000000    olcaezdi    ADR      ||Sets current device on output bus
# 10000101    ra        00000000    olcaezdi    OUTB     ||Outputs ra in binary to console
# 10000110    ra        00000000    olcaezdi    OUTD     ||Outputs ra as a decimal to colsole
#
#
#
#Other operations
# 11100000    00000000  00000000    olcaezdi    SETF     ||Set CPU flag
# 11100001    00000000  00000000    olcaezdi    CLF      ||Clear ALU flags
# 11100010    00000000  00000000    olcaezdi    CLI      ||Clear interrupt flag
# 11111111    00000000  00000000    olcaezdi    HALT     ||Halt system
#
#-------------------------------------------------------#
*/


CPU::CPU()
{
	m_ram = RAM();
	m_alu = ALU();
	m_ir = Register();
	m_iar = Register();
	m_registers[0] = m_iar;

	for(unsigned i = 1; i<256;i++)
	{
		m_registers[i] = Register();
	}

	//m_out_bus = OutputBus();
	m_done = false;
}

void CPU::start(std::string filename)
{
	std::ifstream program_file;
	program_file.open(filename);

	std::vector<unsigned> program;
	std::string line;
	char* ptr;
	unsigned val;

	while(program_file >> line)
	{
		val = strtol(line.c_str(), &ptr, 2);
		program.push_back(val);
	}

	program_file.close();

	m_ram.loadProgram(program,0);
	loop();
}

void CPU::loop()
{
	while(!m_done)
	{
		operation();
		m_registers[0].increment();
		//m_out_bus.update();
	}

	std::cout << "The program has finished. Press any key to continue... " << std::endl;
	system("pause");
}

/*void CPU::addDevice(Device device, unsigned addr)
{
	device.m_addr = addr;
	m_out_bus.addDevice(device);
}*/

void CPU::operation()
{
	m_ir.setValue(m_ram.read(m_registers[0].getValue()));
	std::string command = std::bitset<32>(m_ir.getValue()).to_string();

	//Check flags
	bool perform = false;
	if(command[24]=='0')
	{
		perform = true;
	}
	else
	{
		//l=AND
		if(command[25]=='0')
		{
			if((int)command[26]==m_alu.m_flags[0]
				&&(int)command[27]==m_alu.m_flags[1]
				&&(int)command[28]==m_alu.m_flags[2]
				&&(int)command[29]==m_alu.m_flags[3]
				&&(int)command[30]==m_cpu_flag)
			{
				perform = true;
			}
		}
		//l=OR
		else
		{
			if((int)command[26]==m_alu.m_flags[0]
				||(int)command[27]==m_alu.m_flags[1]
				||(int)command[28]==m_alu.m_flags[2]
				||(int)command[29]==m_alu.m_flags[3]
				||(int)command[30]==m_cpu_flag)
				//||(int)command[31]==m_out_bus.m_interrupt)
			{
				perform = true;
			}
		}
	}

	//Perform the operation
	if(perform)
	{

		Register a = m_registers[(m_ir.getValue()&16711680)>>16]; //Register at chars 9-16 in cmd
		Register b = m_registers[(m_ir.getValue()&65280)>>8]; //Register at chars 17-24 in cmd
		unsigned main = (m_ir.getValue()&3758096384)>>53; //Int representing chars 0-2 in cmd
		unsigned sub = (m_ir.getValue()&520093696)>>48; //Int representing chars 3-8 in cmd

		//ALU operation
		if(!main)
		{
			m_alu.operation(sub,a,b);
		}

		//Ram/Register operation
		else if(main==1)
		{
			if(!sub)
			{
				read(a,b);
			}
			else if(sub==1)
			{
				data(a);
			}
			else if(sub==2)
			{
				write(a,b);
			}
			else if(sub==3)
			{
				writeTo(a);
			}
			else if(sub==4)
			{
				clone(a,b);
			}
		}

		//Jump operations
		else if(main==2)
		{
			if(!sub)
			{
				jump(b);
			}
			else if(sub==1)
			{
				jumpTo();
			}
		}

		//IO operations
		else if(main==4)
		{
			if(!sub)
			{
				//getIn(a);
			}
			else if(sub==1)
			{
				//setOut(a);
			}
			else if(sub==2)
			{
				//dataOut(a);
			}
			else if(sub==3)
			{
				//outFrom(a);
			}
			else if(sub==4)
			{
				//address(a);
			}
			else if(sub==5)
			{
				outputB(a);
			}
			else if(sub==6)
			{
				outputD(a);
			}
		}

		//Other operations
		else if(main==7)
		{
			if(!sub)
			{
				setFlag();
			}
			else if(sub==1)
			{
				clearFlags();
			}
			else if(sub==2)
			{
				//clearInterrupt();
			}
			else if(sub==31)
			{
				halt();
			}
		}
	}
}

//Operations-----
void CPU::read(Register a, Register b)
{
	a.setValue(m_ram.read(b.getValue()));
}

void CPU::data(Register a)
{
	a.setValue(m_ram.read(m_registers[0].getValue()+1));
	m_registers[0].increment();
}

void CPU::write(Register a, Register b)
{
	m_ram.set(a.getValue(),b.getValue());
}

void CPU::writeTo(Register a)
{
	m_ram.set(a.getValue(),m_ram.read(m_registers[0].getValue()+1));
    m_registers[0].increment();
}

void CPU::clone(Register a, Register b)
{
	a.setValue(b.getValue());
}

void CPU::jump(Register a)
{
	m_registers[0].setValue(a.getValue()-1);
}

void CPU::jumpTo()
{
	m_registers[0].setValue(m_ram.read(m_registers[0].getValue()+1)-1);
}

/*void CPU::getIn(Register a)
{
	a.setValue(m_out_bus.set(0,true,false));
}

void CPU::setOut(Register a)
{
	m_out_bus.set(a.getValue(),false,false);
}

void CPU::dataOut(Register a)
{
	m_out_bus.set(m_ram.read(m_registers[0].getValue()+1),false,false);
	m_registers[0].increment();
}

void CPU::outFrom(Register a)
{
	m_out_bus.set(m_ram.read(m_ram.read(m_registers[0].getValue()+1)),false,false);
	m_registers[0].increment();
}

void CPU::address(Register a)
{
	m_out_bus.set(a.getValue(),false,true);
}*/

void CPU::outputD(Register a)
{
	std::cout << a.getValue() << std::endl;
}

void CPU::outputB(Register a)
{
	std::cout << std::bitset<32>(a.getValue()) << std::endl;
}

void CPU::setFlag()
{
	m_cpu_flag = 1;
}

void CPU::clearFlags()
{
	for(auto &flag : m_alu.m_flags)
	{
		flag = 0;
	}

	m_cpu_flag = 0;
}

/*void CPU::clearInterrupt()
{
	m_out_bus.m_interrupt = 0;
}*/

void CPU::halt()
{
	m_done = true;
}


int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "Boot: ";
	std::string filename;
	std::cin >> filename;

	CPU processor = CPU();


	processor.start(filename);

	system("pause");
}

