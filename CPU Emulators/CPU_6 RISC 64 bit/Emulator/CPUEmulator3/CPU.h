#ifndef __CPU_INCLUDED__
#define __CPU_INCLUDED__

#include "RAM.h"
#include "register.h"

typedef unsigned immediate; //Represents any constant value taken from an instruction

class CPU
{
private:
	RAM m_ram;
	Register m_ir;
	Register m_pc;
	Register m_registers[32];

	immediate imm;
	Register* rs1;
	Register* rs2;
	Register* rd;
	immediate funct7;
	immediate funct3;
	std::string in_buffer;

	bool m_done;
	unsigned long instructions_completed;

	void loop();
	void operation();

	//Split instructions
	void rSplit();
	void iSplit();
	void irSplit();
	void lSplit();

	//Operations
	void cpu_lui();
	void cpu_jal();
	void cpu_beq();
	void cpu_bne();
	void cpu_blt();
	void cpu_ble();
	void cpu_bltu();
	void cpu_bleu();
	void cpu_lb();
	void cpu_lh();
	void cpu_lw();
	void cpu_lbu();
	void cpu_lhu();
	void cpu_sb();
	void cpu_sh();
	void cpu_sw();
	void cpu_addi();
	void cpu_addui();
	void cpu_subi();
	void cpu_subui();
	void cpu_xori();
	void cpu_ori();
	void cpu_andi();
	void cpu_add();
	void cpu_addu();
	void cpu_sub();
	void cpu_subu();
	void cpu_xor();
	void cpu_or();
	void cpu_and();
	void cpu_not();
	void cpu_shli();
	void cpu_shlui();
	void cpu_shri();
	void cpu_shrui();
	void cpu_shl();
	void cpu_shlu();
	void cpu_shr();
	void cpu_shru();
	void cpu_muli();
	void cpu_mulhi();
	void cpu_mului();
	void cpu_mulhui();
	void cpu_divi();
	void cpu_divui();
	void cpu_remi();
	void cpu_remui();
	void cpu_mul();
	void cpu_mulh();
	void cpu_mulu();
	void cpu_mulhu();
	void cpu_div();
	void cpu_divu();
	void cpu_rem();
	void cpu_remu();
	void cpu_amoswap();
	void cpu_core();
	void cpu_cwait();
	void cpu_time();
	void cpu_halt();
	void cpu_jalr();
	void cpu_in();
	void cpu_out();

public:
	CPU();
	explicit CPU(std::string f) : CPU() { start(f); };

	void start(std::string);
};

#endif