#include "stdafx.h"
#include <limits>
#include "ALU.h"



ALU::ALU()
{
	for(auto &flag : m_flags) //Carry,A greater,Equal,Output=0 (A=0 after operation)
	{
		flag = 0;
	}
}



//Perform the operation given by the CPU
void ALU::operation(int code, Register a, Register b)
{
	//Reset all flags that aren't required
	m_flags[1] = 0;
	m_flags[2] = 0;
	m_flags[3] = 0;

	//Check if unary operation
	if(!(code==0||code==1||code==4||code==9||code==13||code==16||code==17||code==18||code==19||code==20))
	{
		//Compare a and b if possible and set flags
		if(a.getValue()>b.getValue())
		{
			m_flags[1] = 1;
		}
		else if(a.getValue()==b.getValue())
		{
			m_flags[2] = 1;
		}
	}

	//Perform operation based on code (sets carry flag to 0 if it isn't set in the operation)
	if(code==0)
	{
		zero(a);
		m_flags[0] = 0;
	}
	else if(code==1)
	{
		one(a);
		m_flags[0] = 0;
	}
	else if(code==2)
	{
		add(a,b);
	}
	else if(code==3)
	{
		addc(a,b);
	}
	else if(code==4)
	{
		inc(a);
	}
	else if(code==5)
	{
		sub(a,b);
	}
	else if(code==6)
	{
		subc(a,b);
	}
	else if(code==7)
	{
		rsub(a,b);
	}
	else if(code==8)
	{
		rsubc(a,b);
	}
	else if(code==9)
	{
		dec(a);
	}
	else if(code==10)
	{
		mul(a,b);
	}
	else if(code==11)
	{
		random(a);
		m_flags[0] = 0;
	}
	else if(code==16)
	{
		shl(a);
	}
	else if(code==17)
	{
		shlc(a);
	}
	else if(code==18)
	{
		shr(a);
	}
	else if(code==19)
	{
		shrc(a);
	}
	else if(code==20)
	{
		not(a);
		m_flags[0] = 0;
	}
	else if(code==21)
	{
		and(a,b);
		m_flags[0] = 0;
	}
	else if(code==22)
	{
		nand(a,b);
		m_flags[0] = 0;
	}
	else if(code==23)
	{
		or(a,b);
		m_flags[0] = 0;
	}
	else if(code==24)
	{
		nor(a,b);
		m_flags[0] = 0;
	}
	else if(code==25)
	{
		xor(a,b);
		m_flags[0] = 0;
	}
	else if(code==26)
	{
		xnor(a,b);
		m_flags[0] = 0;
	}
	else if(code==27)
	{
		bclr(a,b);
		m_flags[0] = 0;
	}
	else if(code==31)
	{
		m_flags[0] = 0;
	}

	//Check if output is 0 and set flag accordingly
	if(a.getValue())
	{
		m_flags[3]=1;
	}
}



//-----OPERATIONS-----



//a=0
void ALU::zero(Register a)
{
	a.clear();
}

//a=1
void ALU::one(Register a)
{
	a.setValue(1);
}

//a=a+b
void ALU::add(Register a,Register b)
{
	if(a.getValue()>UINT_MAX-b.getValue())
	{
		m_flags[0] = 1;
	}
	else
	{
		m_flags[0] = 0;
	}
	a.setValue(a.getValue()+b.getValue());
}

//a=a+b+c
void ALU::addc(Register a,Register b)
{
	unsigned val = a.getValue()+b.getValue()+m_flags[0];
	if(a.getValue()>UINT_MAX-b.getValue()-m_flags[0])
	{
		m_flags[0] = 1;
	}
	else
	{
		m_flags[0] = 0;
	}
	a.setValue(val);
}

//a=a+1
void ALU::inc(Register a)
{
	if(a.getValue()==UINT_MAX)
	{
		m_flags[0] = 1;
	}
	else
	{
		m_flags[0] = 0;
	}
	a.setValue(a.getValue()+1);
}

//a=a-b
void ALU::sub(Register a, Register b)
{
	if(a.getValue()<b.getValue())
	{
		m_flags[0] = 1;
	}
	else
	{
		m_flags[0] = 0;
	}
	a.setValue(a.getValue()-b.getValue());
}

//a=a-b+c-1
void ALU::subc(Register a, Register b)
{
	unsigned val = a.getValue()-b.getValue()+m_flags[0]-1;
	if(a.getValue()<b.getValue()-m_flags[0]+1)
	{
		m_flags[0] = 1;
	}
	else
	{
		m_flags[0] = 0;
	}
	a.setValue(val);
}

//a=b-a
void ALU::rsub(Register a, Register b)
{
	if(b.getValue()<a.getValue())
	{
		m_flags[0] = 1;
	}
	else
	{
		m_flags[0] = 0;
	}
	a.setValue(b.getValue()-a.getValue());
}

//a=b-a+c-1
void ALU::rsubc(Register a, Register b)
{
	unsigned val = b.getValue()-a.getValue()+m_flags[0]-1;
	if(b.getValue()<a.getValue()-m_flags[0]+1)
	{
		m_flags[0] = 1;
	}
	else
	{
		m_flags[0] = 0;
	}
	a.setValue(val);
}

//a=a-1
void ALU::dec(Register a)
{
	if(!a.getValue())
	{
		m_flags[0] = 1;
	}
	else
	{
		m_flags[0] = 0;
	}
	a.setValue(a.getValue()-1);
}

//a=a*b
void ALU::mul(Register a, Register b)
{
	if(a.getValue()>UINT_MAX/b.getValue())
	{
		m_flags[0] = 1;
	}
	else
	{
		m_flags[0] = 0;
	}
	a.setValue(a.getValue()*b.getValue());
}

void ALU::random(Register a)
{
	a.setValue(rand());
}


//Binary operations
void ALU::shl(Register a)
{
	m_flags[0] = a.getValue()>>31;
	a.setValue(a.getValue()<<1);
}

void ALU::shlc(Register a)
{
	unsigned val = (a.getValue()<<1)+m_flags[0];
	m_flags[0] = a.getValue()>>31;
	a.setValue(val);
}

void ALU::shr(Register a)
{
	m_flags[0] = a.getValue()&1;
	a.setValue(a.getValue()>>1);
}

void ALU::shrc(Register a)
{
	unsigned val = (a.getValue()>>1)+m_flags[0]*2147483648;
	m_flags[0] = a.getValue()&1;
	a.setValue(val);
}

void ALU::not(Register a)
{
	a.setValue(~a.getValue());
}

void ALU::and(Register a, Register b)
{
	a.setValue(a.getValue()&b.getValue());
}

void ALU::nand(Register a, Register b)
{
	a.setValue(~(a.getValue()&b.getValue()));
}

void ALU::or(Register a, Register b)
{
	a.setValue(a.getValue()|b.getValue());
}

void ALU::nor(Register a, Register b)
{
	a.setValue(~(a.getValue()|b.getValue()));
}

void ALU::xor(Register a, Register b)
{
	a.setValue(a.getValue()^b.getValue());
}

void ALU::xnor(Register a, Register b)
{
	a.setValue(~(a.getValue()^b.getValue()));
}

void ALU::bclr(Register a, Register b)
{
	a.setValue(a.getValue()&(~b.getValue()));
}