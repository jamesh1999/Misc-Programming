#ifndef __CPU_INCLUDED__
#define __CPU_INCLUDED__

#include <vector>

class CPU;
typedef uint64_t Register;
typedef void (CPU::*funct_ptr)();

class CPU
{
private:
	//Hardware
	Register m_pc;
	Register m_ir;
	Register m_swi_loc;
	Register m_ipi_loc;
	Register m_hwi_loc;
	Register m_halt_loc;
	Register m_float_registers[32];
	Register m_registers[32];
	unsigned char* m_ram;

	//Instruction components
	unsigned imm;
	int rega;
	int regb;
	int rdest;

	//Other
	std::string in_buffer;
	bool m_safe = false;
	bool m_done = false;
	bool m_hwi = false;
	bool m_swi = false;
	bool m_ipi = false;
	bool m_enable_i = false;
	unsigned long instructions_completed = 0;

	void loop();
	void operation();

	void loadProgram(std::vector<unsigned char>, unsigned) const;
	uint64_t read(int, uint64_t);
	void write(int, uint64_t, uint64_t);

	//Operations
	void cpu_halt();
	void cpu_swi();
	void cpu_safe();
	void cpu_seti();

	void cpu_lw();
	void cpu_lh();
	void cpu_ls();
	void cpu_lb();
	void cpu_sw();
	void cpu_sh();
	void cpu_ss();
	void cpu_sb();
	void cpu_lfs();
	void cpu_lfd();
	void cpu_sfs();
	void cpu_sfd();

	void cpu_jal();
	void cpu_jalr();
	void cpu_beq();
	void cpu_bne();
	void cpu_blt();
	void cpu_ble();
	void cpu_bltu();
	void cpu_bleu();

	void cpu_fadd();
	void cpu_fsub();
	void cpu_fmul();
	void cpu_fdiv();
	void cpu_fsqrt();
	void cpu_fsgn();
	void cpu_fcmp();

	void cpu_not();
	void cpu_and();
	void cpu_or();
	void cpu_xor();
	void cpu_add();
	void cpu_sub();
	void cpu_mul();
	void cpu_mulu();
	void cpu_div();
	void cpu_divu();
	void cpu_rem();
	void cpu_remu();
	void cpu_shl();
	void cpu_shlu();
	void cpu_shr();
	void cpu_shru();

	void cpu_andi();
	void cpu_ori();
	void cpu_xori();
	void cpu_addi();
	void cpu_subi();
	void cpu_muli();
	void cpu_mului();
	void cpu_divi();
	void cpu_divui();
	void cpu_remi();
	void cpu_remui();
	void cpu_shli();
	void cpu_shlui();
	void cpu_shri();
	void cpu_shrui();

	void cpu_outd();
	void cpu_ind();
	void cpu_ipi();
	void cpu_pid();

	//Invalid instructions do nothing
	void cpu_noop();

	funct_ptr m_operations[128] =
	{
		&cpu_halt,
		&cpu_swi,
		&cpu_safe,
		&cpu_seti,
		&cpu_lw,
		&cpu_lh,
		&cpu_ls,
		&cpu_lb,
		&cpu_sw,
		&cpu_sh,
		&cpu_ss,
		&cpu_sb,
		&cpu_lfs,
		&cpu_lfd,
		&cpu_sfs,
		&cpu_sfd,
		&cpu_jal,
		&cpu_jalr,
		&cpu_beq,
		&cpu_bne,
		&cpu_blt,
		&cpu_ble,
		&cpu_bltu,
		&cpu_bleu,
		&cpu_fadd,
		&cpu_fsub,
		&cpu_fmul,
		&cpu_fdiv,
		&cpu_fsqrt,
		&cpu_fsgn,
		&cpu_fcmp,
		&cpu_noop,
		&cpu_not,
		&cpu_and,
		&cpu_or,
		&cpu_xor,
		&cpu_add,
		&cpu_sub,
		&cpu_mul,
		&cpu_mulu,
		&cpu_div,
		&cpu_divu,
		&cpu_rem,
		&cpu_remu,
		&cpu_shl,
		&cpu_shlu,
		&cpu_shr,
		&cpu_shru,
		&cpu_noop,
		&cpu_andi,
		&cpu_ori,
		&cpu_xori,
		&cpu_addi,
		&cpu_subi,
		&cpu_muli,
		&cpu_mului,
		&cpu_divi,
		&cpu_divui,
		&cpu_remi,
		&cpu_remui,
		&cpu_shli,
		&cpu_shlui,
		&cpu_shri,
		&cpu_shrui,
		&cpu_outd,
		&cpu_ind,
		&cpu_ipi,
		&cpu_pid,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop,
		&cpu_noop
	};

public:
	CPU() = default;
	explicit CPU(std::string f) : CPU() { start(f); };

	void start(std::string);
};

#endif