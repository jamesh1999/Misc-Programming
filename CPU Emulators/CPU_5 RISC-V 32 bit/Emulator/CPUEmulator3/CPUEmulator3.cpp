// CPUEmulator2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

#include "CPU.h"


//Takes an unsigned and sign-extends it before returning a signed int
inline int convertSigned(unsigned val, int signBit)
{
	val <<= signBit; //Shift sign bit into most significant bit

	if (val <= INT_MAX)
		return static_cast<int>(val) >> signBit; //Positive value returned
	return (static_cast<int>(val - INT_MIN) + INT_MIN) >> signBit; //Negative value returned
}


CPU::CPU()
{
	imm = 0;
	rs1 = nullptr;
	rs2 = nullptr;
	rd = nullptr;
	funct7 = 0;
	funct3 = 0;

	m_done = false;
	instructions_completed = 0;
}

//Read file and write data to RAM
void CPU::start(std::string filename)
{
	std::ifstream program_file(filename);

	std::vector<unsigned char> data;
	unsigned byte_val = 0;

	while (program_file >> byte_val)
		data.push_back(byte_val);

	program_file.close();

	m_ram.loadProgram(data, 0);

	loop(); //Start main loop
}

void CPU::loop()
{
	auto start_time = std::chrono::high_resolution_clock::now();

	while (!m_done)
	{
		operation();
		m_pc.setValue(m_pc.getValue() + 4); //Move the program counter to the next instruction
		m_registers[0].clear(); //Ensure that x0 (ZERO) remains at 0
		++instructions_completed;
	}

	auto delta_time = std::chrono::high_resolution_clock::now() - start_time; //Calculate time taken
	double seconds = std::chrono::duration_cast<std::chrono::milliseconds>(delta_time).count() / 1000.0;

	std::cout << "Instructions ran: " << instructions_completed << std::endl;
	std::cout << "Time taken: " << seconds << "s" << std::endl;
	std::cout << "Instructions per second: " << static_cast<long>(instructions_completed / seconds) << "s-1" << std::endl;
	std::cout << "FP: " << m_registers[2].getValue() << std::endl;
	std::cout << "SP: " << m_registers[14].getValue() << std::endl;
}

inline void CPU::operation()
{
	m_ir.setValue(m_ram.read(4, m_pc.getValue())); //Copy instruction to instruction register
	unsigned opcode = (m_ir.getValue() >> 25) & 0x1f; //Get the opcode for the instruction

	switch (m_ir.getValue() >> 30) //Switch instruction format and opcode
	{
	case 0:
		lSplit(); //L-type instructions

		switch (opcode)
		{
		case 0:
			cpu_lui();
			break;
		case 1:
			cpu_jal();
			break;
		case 2:
			cpu_cwait();
			break;
		case 3:
			cpu_time();
			break;
		case 4:
			cpu_halt();
			break;
		}

		break;
	case 1:
		rSplit(); //R-type instructions

		switch (opcode)
		{
		case 0:
			switch (funct3)
			{
			case 0:
				cpu_add();
				break;
			case 1:
				cpu_addu();
				break;
			case 2:
				cpu_sub();
				break;
			case 3:
				cpu_subu();
				break;
			case 4:
				cpu_and();
				break;
			case 5:
				cpu_or();
				break;
			case 6:
				cpu_xor();
				break;
			case 7:
				cpu_not();
				break;
			}
			break;
		case 1:
			switch (funct3)
			{
			case 1:
				cpu_shr();
				break;
			case 2:
				cpu_shl();
				break;
			case 5:
				cpu_shru();
				break;
			case 6:
				cpu_shlu();
				break;
			}
			break;
		case 2:
			switch (funct3)
			{
			case 0:
				cpu_mul();
				break;
			case 1:
				cpu_mulh();
				break;
			case 2:
				cpu_div();
				break;
			case 3:
				cpu_rem();
				break;
			case 4:
				cpu_mulu();
				break;
			case 5:
				cpu_mulhu();
				break;
			case 6:
				cpu_divu();
				break;
			case 7:
				cpu_remu();
				break;
			}
			break;
		case 3:
			switch (funct3)
			{
			case 0:
				cpu_bne();
				break;
			case 1:
				cpu_blt();
				break;
			case 2:
				cpu_beq();
				break;
			case 3:
				cpu_ble();
				break;
			case 5:
				cpu_bltu();
				break;
			case 7:
				cpu_bleu();
				break;
			}
			break;
		}

		break;
	case 2:
		iSplit(); //I-type instructions

		switch (opcode)
		{
		case 1:
			switch (funct3)
			{
			case 1:
				cpu_lhu();
				break;
			case 2:
				cpu_lbu();
				break;
			case 4:
				cpu_lw();
				break;
			case 5:
				cpu_lh();
				break;
			case 6:
				cpu_lb();
				break;
			}
			break;
		case 2:
			switch (funct3)
			{
			case 0:
				cpu_addi();
				break;
			case 1:
				cpu_addui();
				break;
			case 2:
				cpu_subi();
				break;
			case 3:
				cpu_subui();
				break;
			case 4:
				cpu_andi();
				break;
			case 5:
				cpu_ori();
				break;
			case 6:
				cpu_xori();
				break;
			}
			break;
		case 3:
			switch (funct3)
			{
			case 1:
				cpu_shri();
				break;
			case 2:
				cpu_shli();
				break;
			case 5:
				cpu_shrui();
				break;
			case 6:
				cpu_shlui();
				break;
			}
			break;
		case 4:
			switch (funct3)
			{
			case 0:
				cpu_muli();
				break;
			case 1:
				cpu_mulhi();
				break;
			case 2:
				cpu_divi();
				break;
			case 3:
				cpu_remi();
				break;
			case 4:
				cpu_mului();
				break;
			case 5:
				cpu_mulhui();
				break;
			case 6:
				cpu_divui();
				break;
			case 7:
				cpu_remui();
				break;
			}
			break;
		case 5:
			switch (funct3)
			{
			case 0:
				cpu_sw();
				break;
			case 1:
				cpu_sh();
				break;
			case 2:
				cpu_sb();
				break;
			}
			break;
		case 6:
			cpu_core();
			break;
		case 7:
			cpu_jalr();
			break;
		case 8:
			cpu_in();
			break;
		case 9:
			cpu_out();
			break;
		}

		break;
	case 3:
		irSplit(); //IR-type instructions

		switch (opcode)
		{
		case 0:
			cpu_amoswap();
			break;
		}

		break;
	}
}

//Split functions separate instruction and select registers

inline void CPU::rSplit()
{
	funct7 = m_ir.getValue() & 127;
	rs2 = &m_registers[(m_ir.getValue() >> 7) & 31];
	rs1 = &m_registers[(m_ir.getValue() >> 12) & 31];
	funct3 = (m_ir.getValue() >> 17) & 7;
	rd = &m_registers[(m_ir.getValue() >> 20) & 31];
}

inline void CPU::iSplit()
{
	imm = m_ir.getValue() & 4095;
	rs1 = &m_registers[(m_ir.getValue() >> 12) & 31];
	funct3 = (m_ir.getValue() >> 17) & 7;
	rd = &m_registers[(m_ir.getValue() >> 20) & 31];
}

inline void CPU::irSplit()
{
	imm = ((m_ir.getValue() & 127) << 3) + ((m_ir.getValue() >> 17) & 7);
	rs2 = &m_registers[(m_ir.getValue() >> 7) & 31];
	rs1 = &m_registers[(m_ir.getValue() >> 12) & 31];
	rd = &m_registers[(m_ir.getValue() >> 20) & 31];
}

inline void CPU::lSplit()
{
	imm = m_ir.getValue() & 1048575;
	rd = &m_registers[(m_ir.getValue() >> 20) & 31];
}


/* ---------------------------------
OPERATIONS
--------------------------------- */


inline void CPU::cpu_lui()
{
	rd->setValue(imm << 12);
}

inline void CPU::cpu_jal()
{
	rd->setValue(m_pc.getValue() + convertSigned(imm, 12) * 2);
	m_pc.setValue(rd->getValue() - 4);
}

inline void CPU::cpu_addi()
{
	rd->setValue(rs1->getValue() + convertSigned(imm, 20));
}

inline void CPU::cpu_addui()
{
	rd->setValue(rs1->getValue() + imm);
}

inline void CPU::cpu_subi()
{
	rd->setValue(rs1->getValue() - convertSigned(imm, 20));
}

inline void CPU::cpu_subui()
{
	rd->setValue(rs1->getValue() - imm);
}

inline void CPU::cpu_andi()
{
	rd->setValue(rs1->getValue() & imm);
}

inline void CPU::cpu_ori()
{
	rd->setValue(rs1->getValue() | imm);
}

inline void CPU::cpu_xori()
{
	rd->setValue(rs1->getValue() ^ imm);
}

inline void CPU::cpu_add()
{
	rd->setValue(rs1->getValue() + convertSigned(rs2->getValue(), 0));
}

inline void CPU::cpu_addu()
{
	rd->setValue(rs1->getValue() + rs2->getValue());
}

inline void CPU::cpu_sub()
{
	rd->setValue(rs1->getValue() - convertSigned(rs2->getValue(), 0));
}

inline void CPU::cpu_subu()
{
	rd->setValue(rs1->getValue() - rs2->getValue());
}

inline void CPU::cpu_and()
{
	rd->setValue(rs1->getValue() & rs2->getValue());
}

inline void CPU::cpu_or()
{
	rd->setValue(rs1->getValue() | rs2->getValue());
}

inline void CPU::cpu_xor()
{
	rd->setValue(rs1->getValue() ^ rs2->getValue());
}

inline void CPU::cpu_not()
{
	rd->setValue(!rs1->getValue());
}

inline void CPU::cpu_beq()
{
	if (rs1->getValue() == rs2->getValue())
	{
		rd->setValue(m_pc.getValue() + convertSigned(funct7, 25) * 2);
		m_pc.setValue(rd->getValue() - 4);
	}
}

inline void CPU::cpu_bne()
{
	if (rs1->getValue() != rs2->getValue())
	{
		rd->setValue(m_pc.getValue() + convertSigned(funct7, 25) * 2);
		m_pc.setValue(rd->getValue() - 4);
	}
}

inline void CPU::cpu_blt()
{
	if (convertSigned(rs1->getValue(), 0) < convertSigned(rs2->getValue(), 0))
	{
		rd->setValue(m_pc.getValue() + convertSigned(funct7, 25) * 2);
		m_pc.setValue(rd->getValue() - 4);
	}
}

inline void CPU::cpu_ble()
{
	if (convertSigned(rs1->getValue(), 0) <= convertSigned(rs2->getValue(), 0))
	{
		rd->setValue(m_pc.getValue() + convertSigned(funct7, 25) * 2);
		m_pc.setValue(rd->getValue() - 4);
	}
}

inline void CPU::cpu_bltu()
{
	if (rs1->getValue() < rs2->getValue())
	{
		rd->setValue(m_pc.getValue() + convertSigned(funct7, 25) * 2);
		m_pc.setValue(rd->getValue() - 4);
	}
}

inline void CPU::cpu_bleu()
{
	if (rs1->getValue() <= rs2->getValue())
	{
		rd->setValue(m_pc.getValue() + convertSigned(funct7, 25) * 2);
		m_pc.setValue(rd->getValue() - 4);
	}
}

inline void CPU::cpu_shri()
{
	rd->setValue(static_cast<unsigned>((static_cast<int>(rs1->getValue())) >> imm));
}

inline void CPU::cpu_shli()
{
	rd->setValue(static_cast<unsigned>((static_cast<int>(rs1->getValue())) << imm));
}

inline void CPU::cpu_shrui()
{
	rd->setValue(rs1->getValue() >> imm);
}

inline void CPU::cpu_shlui()
{
	rd->setValue(rs1->getValue() << imm);
}

inline void CPU::cpu_shr()
{
	rd->setValue(static_cast<unsigned>(static_cast<int>(rs1->getValue()) >> rd->getValue()));
}

inline void CPU::cpu_shl()
{
	rd->setValue(static_cast<unsigned>(static_cast<int>(rs1->getValue()) << rd->getValue()));
}

inline void CPU::cpu_shru()
{
	rd->setValue(rs1->getValue() >> rd->getValue());
}

inline void CPU::cpu_shlu()
{
	rd->setValue(rs1->getValue() << rd->getValue());
}

inline void CPU::cpu_muli()
{
	rd->setValue(convertSigned(rs1->getValue(), 0) * convertSigned(imm, 20));
}

inline void CPU::cpu_mulhi()
{
	rd->setValue((convertSigned(rs1->getValue(), 0) * convertSigned(imm, 20)) >> 32);
}

inline void CPU::cpu_mului()
{
	rd->setValue(rs1->getValue() * imm);
}

inline void CPU::cpu_mulhui()
{
	rd->setValue((rs1->getValue() * imm) >> 32);
}

inline void CPU::cpu_divi()
{
	rd->setValue(convertSigned(rs1->getValue(), 0) / convertSigned(imm, 20));
}

inline void CPU::cpu_divui()
{
	rd->setValue(rs1->getValue() / imm);
}

inline void CPU::cpu_remi()
{
	rd->setValue(convertSigned(rs1->getValue(), 0) % convertSigned(imm, 20));
}

inline void CPU::cpu_remui()
{
	rd->setValue(rs1->getValue() % imm);
}

inline void CPU::cpu_mul()
{
	rd->setValue(convertSigned(rs1->getValue(), 0) * convertSigned(rs2->getValue(), 0));
}

inline void CPU::cpu_mulh()
{
	rd->setValue((convertSigned(rs1->getValue(), 0) * convertSigned(rs2->getValue(), 0)) >> 32);
}

inline void CPU::cpu_mulu()
{
	rd->setValue(rs1->getValue() * rs2->getValue());
}

inline void CPU::cpu_mulhu()
{
	rd->setValue((rs1->getValue() * rs2->getValue()) >> 32);
}

inline void CPU::cpu_div()
{
	rd->setValue(convertSigned(rs1->getValue(), 0) / convertSigned(rs2->getValue(), 0));
}

inline void CPU::cpu_divu()
{
	rd->setValue(rs1->getValue() / rs2->getValue());
}

inline void CPU::cpu_rem()
{
	rd->setValue(convertSigned(rs1->getValue(), 0) % convertSigned(rs2->getValue(), 0));
}

inline void CPU::cpu_remu()
{
	rd->setValue(rs1->getValue() % rs2->getValue());
}

inline void CPU::cpu_sw()
{
	m_ram.set(4, rd->getValue(), rs1->getValue() + convertSigned(imm, 20));
}

inline void CPU::cpu_sh()
{
	m_ram.set(2, rd->getValue(), rs1->getValue() + convertSigned(imm, 20));
}

inline void CPU::cpu_sb()
{
	m_ram.set(1, rd->getValue(), rs1->getValue() + convertSigned(imm, 20));
}

inline void CPU::cpu_lw()
{
	rd->setValue(m_ram.read(4, rs1->getValue() + convertSigned(imm, 20)));
}

inline void CPU::cpu_lh()
{
	rd->setValue(convertSigned(m_ram.read(2, rs1->getValue() + convertSigned(imm, 20)), 16));
}

inline void CPU::cpu_lb()
{
	rd->setValue(convertSigned(m_ram.read(1, rs1->getValue() + convertSigned(imm, 20)), 24));
}

inline void CPU::cpu_lhu()
{
	rd->setValue(m_ram.read(2, rs1->getValue() + convertSigned(imm, 20)));
}

inline void CPU::cpu_lbu()
{
	rd->setValue(m_ram.read(1, rs1->getValue() + convertSigned(imm, 20)));
}

inline void CPU::cpu_amoswap()
{
	rd->setValue(m_ram.read(4, rs1->getValue() + convertSigned(imm, 22)));
	m_ram.set(4, rs2->getValue(), rs1->getValue() + convertSigned(imm, 22));
}

inline void CPU::cpu_halt()
{
	m_done = true;
}

inline void CPU::cpu_jalr()
{
	rd->setValue(rs1->getValue() + convertSigned(imm, 20) * 2);
	m_pc.setValue(rd->getValue() - 4);
}

inline void CPU::cpu_core()
{
}

inline void CPU::cpu_cwait()
{
}

inline void CPU::cpu_time()
{
}

inline void CPU::cpu_in()
{
	static int cur_char = 0;

	if (in_buffer == "")
	{
		std::cin.ignore();
		std::getline(std::cin, in_buffer);
		cur_char = 0;
	}

	if (in_buffer.size() <= cur_char)
	{
		rd->setValue(0);
		in_buffer.clear();
	}
	else
	{
		rd->setValue(in_buffer[cur_char]);
		++cur_char;
	}
}

inline void CPU::cpu_out()
{
	rd->setValue(rs1->getValue());
	if ((rd->getValue() & 0xff) == '\n')
		std::cout << std::endl;
	else
		std::cout << static_cast<char>(rd->getValue() & 0xff);
}


int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "Boot: ";
	std::string filename;
	std::cin >> filename;

	CPU processor = CPU(filename); //Start CPU

	system("pause");
}

