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

//Takes an unsigned and sign-extends it before returning a signed int
inline int64_t convertSigned(uint64_t val, int signBit)
{
	val <<= signBit; //Shift sign bit into most significant bit

	if (val <= INT64_MAX)
		return static_cast<int>(val) >> signBit; //Positive value returned
	return (static_cast<int>(val - INT64_MIN) + INT64_MIN) >> signBit; //Negative value returned
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

	loadProgram(data, 0);

	loop(); //Start main loop
}

//Takes a program and loads it into RAM from 'start' onwards
void CPU::loadProgram(std::vector<unsigned char> data, unsigned start) const
{
	for (auto b = data.cbegin(); b != data.cend(); ++b)
		m_ram[(b - data.cbegin()) + start] = *b;
}

void CPU::loop()
{
	auto start_time = std::chrono::high_resolution_clock::now();

	while (!m_done)
	{
		operation();
		m_pc += 4; //Move the program counter to the next instruction
		m_registers[0] = 0; //Ensure that x0 (ZERO) remains at 0
		++instructions_completed;
	}

	auto delta_time = std::chrono::high_resolution_clock::now() - start_time; //Calculate time taken
	double seconds = std::chrono::duration_cast<std::chrono::milliseconds>(delta_time).count() / 1000.0;

	std::cout << "Instructions ran: " << instructions_completed << std::endl;
	std::cout << "Time taken: " << seconds << "s" << std::endl;
	std::cout << "Instructions per second: " << static_cast<long>(instructions_completed / seconds) << "s-1" << std::endl;
	std::cout << "FP: " << m_registers[16] << std::endl;
	std::cout << "SP: " << m_registers[15] << std::endl;
}

inline void CPU::operation()
{
	m_ir = read(4, m_pc); //Copy instruction to instruction register
	unsigned opcode = m_ir >> 25; //Get the opcode for the instruction
	rdest = (m_ir >> 20) & 0x1f;
	rega = (m_ir >> 15) & 0x1f;
	regb = (m_ir >> 10) & 0x1f;
	imm = m_ir & 0xfffff;

	if (m_safe && opcode > 63) return;
	(this->*m_operations[opcode])(); //Call function
}


/* ---------------------------------
OPERATIONS
--------------------------------- */


inline void CPU::cpu_jal()
{
	m_registers[rdest] = m_pc + convertSigned(imm & 0xfffff, 12) * 2;
	m_pc = m_registers[rdest] - 4;
}

inline void CPU::cpu_addi()
{
	m_registers[rdest] = m_registers[rega] + (imm & 0x7fff);
}

inline void CPU::cpu_subi()
{
	m_registers[rdest] = m_registers[rega] - (imm & 0x7fff);
}

inline void CPU::cpu_andi()
{
	m_registers[rdest] = m_registers[rega] & (imm & 0x7fff);
}

inline void CPU::cpu_ori()
{
	m_registers[rdest] = m_registers[rega] | (imm & 0x7fff);
}

inline void CPU::cpu_xori()
{
	m_registers[rdest] = m_registers[rega] ^ (imm & 0x7fff);
}

inline void CPU::cpu_add()
{
	m_registers[rdest] = m_registers[rega] + m_registers[regb];
}

inline void CPU::cpu_sub()
{
	m_registers[rdest] = m_registers[rega] - m_registers[regb];
}

inline void CPU::cpu_and()
{
	m_registers[rdest] = m_registers[rega] & m_registers[regb];
}

inline void CPU::cpu_or()
{
	m_registers[rdest] = m_registers[rega] | m_registers[regb];
}

inline void CPU::cpu_xor()
{
	m_registers[rdest] = m_registers[rega] ^ m_registers[regb];
}

inline void CPU::cpu_not()
{
	m_registers[rdest] = !m_registers[rega];
}

inline void CPU::cpu_beq()
{
	if (m_registers[rega] == m_registers[regb])
	{
		m_registers[rdest] = (m_pc + convertSigned(imm & 0x3ff, 22) * 2);
		m_pc = m_registers[rdest] - 4;
	}
}

inline void CPU::cpu_bne()
{
	if (m_registers[rega] != m_registers[regb])
	{
		m_registers[rdest] = (m_pc + convertSigned(imm & 0x3ff, 22) * 2);
		m_pc = m_registers[rdest] - 4;
	}
}

inline void CPU::cpu_blt()
{
	if (convertSigned(m_registers[rega], 0) < convertSigned(m_registers[regb], 0))
	{
		m_registers[rdest] = (m_pc + convertSigned(imm & 0x3ff, 22) * 2);
		m_pc = m_registers[rdest] - 4;
	}
}

inline void CPU::cpu_ble()
{
	if (convertSigned(m_registers[rega], 0) <= convertSigned(m_registers[regb], 0))
	{
		m_registers[rdest] = (m_pc + convertSigned(imm & 0x3ff, 22) * 2);
		m_pc = m_registers[rdest] - 4;
	}
}

inline void CPU::cpu_bltu()
{
	if (m_registers[rega] < m_registers[regb])
	{
		m_registers[rdest] = (m_pc + convertSigned(imm & 0x3ff, 22) * 2);
		m_pc = m_registers[rdest] - 4;
	}
}

inline void CPU::cpu_bleu()
{
	if (m_registers[rega] <= m_registers[regb])
	{
		m_registers[rdest] = (m_pc + convertSigned(imm & 0x3ff, 22) * 2);
		m_pc = m_registers[rdest] - 4;
	}
}

inline void CPU::cpu_shri()
{
	m_registers[rdest] = (static_cast<unsigned>((static_cast<int>(m_registers[rega])) >> (imm & 0x7fff)));
}

inline void CPU::cpu_shli()
{
	m_registers[rdest] = (static_cast<unsigned>((static_cast<int>(m_registers[rega])) << (imm & 0x7fff)));
}

inline void CPU::cpu_shrui()
{
	m_registers[rdest] = (m_registers[rega] >> (imm & 0x7fff));
}

inline void CPU::cpu_shlui()
{
	m_registers[rdest] = (m_registers[rega] << (imm & 0x7fff));
}

inline void CPU::cpu_shr()
{
	m_registers[rdest] = (static_cast<unsigned>(static_cast<int>(m_registers[rega]) >> m_registers[rdest]));
}

inline void CPU::cpu_shl()
{
	m_registers[rdest] = (static_cast<unsigned>(static_cast<int>(m_registers[rega]) << m_registers[rdest]));
}

inline void CPU::cpu_shru()
{
	m_registers[rdest] = (m_registers[rega] >> m_registers[rdest]);
}

inline void CPU::cpu_shlu()
{
	m_registers[rdest] = (m_registers[rega] << m_registers[rdest]);
}

inline void CPU::cpu_muli()
{
	m_registers[rdest] = (convertSigned(m_registers[rega], 0) * convertSigned((imm & 0x7fff), 17));
}

inline void CPU::cpu_mului()
{
	m_registers[rdest] = (m_registers[rega] * (imm & 0x7fff));
}

inline void CPU::cpu_divi()
{
	m_registers[rdest] = (convertSigned(m_registers[rega], 0) / convertSigned((imm & 0x7fff), 17));
}

inline void CPU::cpu_divui()
{
	m_registers[rdest] = (m_registers[rega] / (imm & 0x7fff));
}

inline void CPU::cpu_remi()
{
	m_registers[rdest] = (convertSigned(m_registers[rega], 0) % convertSigned((imm & 0x7fff), 17));
}

inline void CPU::cpu_remui()
{
	m_registers[rdest] = (m_registers[rega] % (imm & 0x7fff));
}

inline void CPU::cpu_mul()
{
	m_registers[rdest] = (convertSigned(m_registers[rega], 0) * convertSigned(m_registers[regb], 0));
}

inline void CPU::cpu_mulu()
{
	m_registers[rdest] = (m_registers[rega] * m_registers[regb]);
}

inline void CPU::cpu_div()
{
	m_registers[rdest] = (convertSigned(m_registers[rega], 0) / convertSigned(m_registers[regb], 0));
}

inline void CPU::cpu_divu()
{
	m_registers[rdest] = (m_registers[rega] / m_registers[regb]);
}

inline void CPU::cpu_rem()
{
	m_registers[rdest] = (convertSigned(m_registers[rega], 0) % convertSigned(m_registers[regb], 0));
}

inline void CPU::cpu_remu()
{
	m_registers[rdest] = (m_registers[rega] % m_registers[regb]);
}

inline void CPU::cpu_sw()
{
	write(8, m_registers[rdest], m_registers[rega] + convertSigned((imm & 0x7fff), 17));
}

inline void CPU::cpu_sh()
{
	write(4, m_registers[rdest], m_registers[rega] + convertSigned((imm & 0x7fff), 17));
}

inline void CPU::cpu_ss()
{
	write(2, m_registers[rdest], m_registers[rega] + convertSigned((imm & 0x7fff), 17));
}

inline void CPU::cpu_sb()
{
	write(1, m_registers[rdest], m_registers[rega] + convertSigned((imm & 0x7fff), 17));
}

inline void CPU::cpu_lw()
{
	m_registers[rdest] = read(8, m_registers[rega] + convertSigned((imm & 0x7fff), 17));
}

inline void CPU::cpu_lh()
{
	m_registers[rdest] = read(4, m_registers[rega] + convertSigned((imm & 0x7fff), 17));
}

inline void CPU::cpu_ls()
{
	m_registers[rdest] = read(2, m_registers[rega] + convertSigned((imm & 0x7fff), 17));
}

inline void CPU::cpu_lb()
{
	m_registers[rdest] = read(1, m_registers[rega] + convertSigned((imm & 0x7fff), 17));
}

inline void CPU::cpu_halt()
{
	m_done = true;
}

void CPU::cpu_swi()
{
	m_swi = true;
}

inline void CPU::cpu_jalr()
{
	m_registers[rdest] = m_registers[rega] + convertSigned((imm & 0x7fff), 17) * 2;
	m_pc = m_registers[rdest] - 4;
}

inline void CPU::cpu_ind()
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
		m_registers[rdest] = 0;
		in_buffer.clear();
	}
	else
	{
		m_registers[rdest] = in_buffer[cur_char];
		++cur_char;
	}
}

void CPU::cpu_ipi()
{
	m_ipi = true;
}

inline void CPU::cpu_outd()
{
	m_registers[rdest] = m_registers[rega];
	if ((m_registers[rdest] & 0xff) == '\n')
		std::cout << std::endl;
	else
		std::cout << static_cast<char>(m_registers[rdest] & 0xff);
}


int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "Boot: ";
	std::string filename;
	std::cin >> filename;

	CPU processor = CPU(filename); //Start CPU

	system("pause");
}

