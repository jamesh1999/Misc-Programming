#ifndef __ALU_INCLUDED__
#define __ALU_INCLUDED__

#include "register.h"

class ALU
{
private:
	void zero(Register);
	void one(Register);
	void add(Register, Register);
	void addc(Register, Register);
	void inc(Register);
	void sub(Register, Register);
	void subc(Register, Register);
	void rsub(Register, Register);
	void rsubc(Register, Register);
	void dec(Register);
	void mul(Register, Register);
	void random(Register);
	void shl(Register);
	void shlc(Register);
	void shr(Register);
	void shrc(Register);
	void not(Register);
	void and(Register, Register);
	void nand(Register, Register);
	void or(Register, Register);
	void nor(Register, Register);
	void xor(Register, Register);
	void xnor(Register, Register);
	void bclr(Register, Register);
public:
	ALU();

	int m_flags[4];

	void operation(int, Register, Register);
};

#endif