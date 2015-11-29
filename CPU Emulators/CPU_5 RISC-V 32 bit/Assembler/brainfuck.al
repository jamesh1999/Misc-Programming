import setup
import print


Var unsigned prog_start = 4000
Var unsigned prog_location = 4000
Var unsigned pointer = 3000
Var unsigned pointer_start = 3000
Var unsigned pointer_end = 3255

Mov S1 prog_start
In S2
:JUMPBACK
Sw S2 S1
Add S1 4
In S2
Branch (S2 != ZERO) :JUMPBACK

:LOOPBACK
Lw S1 prog_location
Add S2 ZERO 62
if(S1 == S2)
{
	Add pointer 4
	Goto :INSTR_END
}

Add S2 ZERO 60
if(S1 == S2)
{
	Sub pointer 4
	Goto :INSTR_END
}

Add S2 ZERO 43
if(S1 == S2)
{
	Lw S3 pointer
	Add S3 1
	Sw S3 pointer
	Goto :INSTR_END
}

Add S2 ZERO 45
if(S1 == S2)
{
	Lw S3 pointer
	Sub S3 1
	Sw S3 pointer
	Goto :INSTR_END
}

Add S2 ZERO 91
if(S1 == S2)
{
	Lw S3 pointer
	if(S3 == ZERO)
	{
		:FORWARD_SEARCH_CALLBACK
		Add prog_location 4
		Lw S4 prog_location
		Add S5 ZERO 93
		Branch(S4 != S5) :FORWARD_SEARCH_CALLBACK
	}
	Goto :INSTR_END
}

Add S2 ZERO 93
if(S1 == S2)
{
	Lw S3 pointer
	if(S3 != ZERO)
	{
		:REVERSE_SEARCH_CALLBACK
		Sub prog_location 4
		Lw S4 prog_location
		Add S5 ZERO 91
		Branch(S4 != S5) :REVERSE_SEARCH_CALLBACK
	}
	Goto :INSTR_END
}

Add S2 ZERO 44
if(S1 == S2)
{
	Lw S3 pointer
	Push S3
	Call :print_d
	Goto :INSTR_END
}

Add S2 ZERO 46
if(S1 == S2)
{
	In S3
	Sw S3 pointer
	Goto :INSTR_END
}

Goto :BREAK

:INSTR_END
Add pointer 4
Branch(pointer > pointer_end) :BREAK
Branch(pointer > pointer_start) :LOOPBACK

:BREAK
Halt