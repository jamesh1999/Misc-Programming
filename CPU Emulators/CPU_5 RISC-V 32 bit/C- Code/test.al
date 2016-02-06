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
Add SP 0
Mov $3 SP
Add SP 8
Add $2 ZERO 3
Push $2
Mov SP $3
Call : fact
Lw $4 FP - 16
Sub SP 0
Sw $4 FP - 12
Ret
Sub SP 0
: ANCHOR_1
Call : main
Mov $5 SP
Add $5 4
Set $6 77
Push $6
Set $6 97
Push $6
Set $6 105
Push $6
Set $6 110
Push $6
Set $6 32
Push $6
Set $6 101
Push $6
Set $6 120
Push $6
Set $6 105
Push $6
Set $6 116
Push $6
Set $6 101
Push $6
Set $6 100
Push $6
Set $6 32
Push $6
Set $6 119
Push $6
Set $6 105
Push $6
Set $6 116
Push $6
Set $6 104
Push $6
Set $6 32
Push $6
Set $6 99
Push $6
Set $6 111
Push $6
Set $6 100
Push $6
Set $6 101
Push $6
Set $6 58
Push $6
Set $6 32
Push $6 , ZERO , $5
Call : print_str
Lw $7 FP + 4
Push $7
Call : print_d
Sub SP 8
Halt
