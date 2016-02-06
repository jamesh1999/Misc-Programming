import setup
import print
Add SP 8
Goto : ANCHOR_0
> fact
Add SP 4
Lw $0 FP + 0
Mov $1 $0
Sub SP 4
Sw $1 FP - 16
Ret
Sub SP 4
: ANCHOR_0
Goto : ANCHOR_1
> main
Add SP 4
Mov $3 SP
Add SP 8
Add $2 ZERO 3
Push $2
Mov SP $3
Call : fact
Lw $5 FP - 16
Mov $4 $5
Sw $4 FP + 0
Lw $6 FP + 0
Mov $7 $6
Sub SP 4
Sw $7 FP - 12
Ret
Sub SP 4
: ANCHOR_1
Call : main
Push SP
Call :print_d
Push FP
Call :print_d
Mov $8 SP
Add $8 4
Set $9 77
Push $9
Set $9 97
Push $9
Set $9 105
Push $9
Set $9 110
Push $9
Set $9 32
Push $9
Set $9 101
Push $9
Set $9 120
Push $9
Set $9 105
Push $9
Set $9 116
Push $9
Set $9 101
Push $9
Set $9 100
Push $9
Set $9 32
Push $9
Set $9 119
Push $9
Set $9 105
Push $9
Set $9 116
Push $9
Set $9 104
Push $9
Set $9 32
Push $9
Set $9 99
Push $9
Set $9 111
Push $9
Set $9 100
Push $9
Set $9 101
Push $9
Set $9 58
Push $9
Set $9 32
Push $9 , ZERO , $8
Call : print_str
Lw $10 FP + 4
Push $10
Call : print_d
Sub SP 8
Halt
