<<START>>
//Setup assembly
import setup
import print

<<END>>
Call :main
Mov $0 SP
Add $0 4
Add $2 ZERO 'M'
Push $2
Add $2 ZERO 'a'
Push $2
Add $2 ZERO 'i'
Push $2
Add $2 ZERO 'n'
Push $2
Add $2 ZERO ' '
Push $2
Add $2 ZERO 'e'
Push $2
Add $2 ZERO 'x'
Push $2
Add $2 ZERO 'i'
Push $2
Add $2 ZERO 't'
Push $2
Add $2 ZERO 'e'
Push $2
Add $2 ZERO 'd'
Push $2
Add $2 ZERO ' '
Push $2
Add $2 ZERO 'w'
Push $2
Add $2 ZERO 'i'
Push $2
Add $2 ZERO 't'
Push $2
Add $2 ZERO 'h'
Push $2
Add $2 ZERO ' '
Push $2
Add $2 ZERO 'c'
Push $2
Add $2 ZERO 'o'
Push $2
Add $2 ZERO 'd'
Push $2
Add $2 ZERO 'e'
Push $2
Add $2 ZERO ':'
Push $2
Add $2 ZERO ' '
Push $2
Push ZERO, $0
Call :print_str
Push $1
Call :print_d
Halt

<<SCOPE_START>>
Add SP ?(SCOPE_OFFSET)

<<SCOPE_END>>
Sub SP ?(SCOPE_OFFSET)

<<MACRO>>

<<stmnt_init>[0]>
Mov $1 @<expr>
Sw $1 FP + ?(STRING, addr)

<<stmnt_init>[1]>


<<stmnt_func>>
Goto :SKIP
> ?(STRING, id)
[<arguments>]
[<block>]
:SKIP

<<stmnt_call>>
Call :?(STRING, id)

<<stmnt_flow_while>>
:LOOP
[<expr>]
Branch (@<expr> == ZERO) :END
[<statement>]
Goto :LOOP
:ENDLOOP

<<stmnt_flow_for>>
:LOOP
Branch (@<expr> == ZERO) :END
[<statement>]
[<stmnt_assign>]
Goto :LOOP
:ENDLOOP

<<stmnt_flow_if>>
Brance (@<expr> == ZERO) :END
[<statement>]
:END

<<stmnt_flow_break>>
Goto :ENDLOOP

<<stmnt_flow_continue>>
Goto :LOOP

<<stmnt_flow_return>[0]>
Sub SP ?(SCOPE_OFFSET)
Ret

<<stmnt_flow_return>[1]>
Sub SP ?(SCOPE_OFFSET)
Push @<expr>
Ret

<<arguments>>
Pop $1
Sw $1 FP + ?(STRING, addr)

<<parameters>[0]>
Lw $1 FP + ?(STRING, addr)
Push $1

<<expr_assign>[0]>
Mov $1 @<expr>
Sw $1 FP + ?(STRING, addr)

<<expr_assign>[1]>
Lw $1 FP + ?(STRING, addr)
Add $1 @<expr>
Sw $1 FP + ?(STRING, addr)

<<expr_assign>[2]>
Lw $1 FP + ?(STRING, addr)
Sub $1 @<expr>
Sw $1 FP + ?(STRING, addr)

<<expr_assign>[3]>
Lw $1 FP + ?(STRING, addr)
Mul $1 @<expr>
Sw $1 FP + ?(STRING, addr)

<<expr_assign>[4]>
Lw $1 FP + ?(STRING, addr)
Div $1 @<expr>
Sw $1 FP + ?(STRING, addr)

<<expr_assign>[5]>
Lw $1 FP + ?(STRING, addr)
Mod $1 @<expr>
Sw $1 FP + ?(STRING, addr)

<<expr_assign>[6]>
Lw $1 FP + ?(STRING, addr)
Shl $1 @<expr>
Sw $1 FP + ?(STRING, addr)

<<expr_assign>[7]>
Lw $1 FP + ?(STRING, addr)
Shr $1 @<expr>
Sw $1 FP + ?(STRING, addr)

<<expr_assign>[8]>
Lw $1 FP + ?(STRING, addr)
And $1 @<expr>
Sw $1 FP + ?(STRING, addr)

<<expr_assign>[9]>
Lw $1 FP + ?(STRING, addr)
Xor $1 @<expr>
Sw $1 FP + ?(STRING, addr)

<<expr_assign>[10]>
Lw $1 FP + ?(STRING, addr)
Or $1 @<expr>
Sw $1 FP + ?(STRING, addr)

<<expr_logical_or>[0]>
Or @ @<expr_logical_or> @<expr_logical_and>

<<expr_logical_and>[0]>
Branch (@<expr_logical_and> == ZERO) :RET_ZERO
Branch (@<expr_bitwise_or> == ZERO) :RET_ZERO
Not @ ZERO
Goto :END
:RET_ZERO
Mov @ ZERO
:END

<<expr_bitwise_or>[0]>
Or @ @<expr_bitwise_or> @<expr_bitwise_and>

<<expr_bitwise_and>[0]>
Or @ @<expr_bitwise_and> @<expr_conditional>

<<expr_conditional>[0]>
Branch (@<expr_conditional> == @<expr_shift>) :RET_ONE
Mov @ ZERO
Goto :END
:RET_ONE
Not @ ZERO
:END

<<expr_conditional>[1]>
Branch (@<expr_conditional> != @<expr_shift>) :RET_ONE
Mov @ ZERO
Goto :END
:RET_ONE
Not @ ZERO
:END

<<expr_conditional>[2]>
Branch (@<expr_conditional> > @<expr_shift>) :RET_ONE
Mov @ ZERO
Goto :END
:RET_ONE
Not @ ZERO
:END

<<expr_conditional>[3]>
Branch (@<expr_conditional> < @<expr_shift>) :RET_ONE
Mov @ ZERO
Goto :END
:RET_ONE
Not @ ZERO
:END

<<expr_conditional>[4]>
Branch (@<expr_conditional> >= @<expr_shift>) :RET_ONE
Mov @ ZERO
Goto :END
:RET_ONE
Not @ ZERO
:END

<<expr_conditional>[5]>
Branch (@<expr_conditional> <= @<expr_shift>) :RET_ONE
Mov @ ZERO
Goto :END
:RET_ONE
Not @ ZERO
:END

<<expr_shift>[0]>
Shr @ @<expr_shift> @<expr_add>

<<expr_shift>[1]>
Shl @ @<expr_shift> @<expr_add>

<<expr_add>[0]>
Add @ @<expr_add> @<expr_mul>

<<expr_add>[1]>
Sub @ @<expr_add> @<expr_mul>

<<expr_mul>[0]>
Mul @ @<expr_mul> @<expr_unary>

<<expr_mul>[1]>
Div @ @<expr_mul> @<expr_unary>

<<expr_mul>[2]>
Mod @ @<expr_mul> @<expr_unary>

<<expr_unary>[2]>
Branch (@<expr_unary> == ZERO) :RET_ONE
Mov @ ZERO
Goto :END
:RET_ONE
Not @ ZERO
:END

<<expr_unary>[3]>
Not @ @<expr_unary>

<<expr_unary>[4]>
Sub @ ZERO @<expr_unary>

<<term>[0]>
Add @ ZERO INT

<<term>[1]>
Add @ ZERO 1

<<term>[2]>
Add @ ZERO 0

<<term>[3]>
Lw $1 FP + ?(STRING, addr)
Mov @ $1