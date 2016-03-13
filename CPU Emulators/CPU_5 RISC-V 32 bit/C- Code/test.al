import setup
import print
Add SP 8
Goto : ANCHOR_0
> fact
Add SP 12
Add $1 ZERO 1
Mov $0 $1
Sw $0 FP + 4
Lw $2 FP + 0
Mov $4 $2
Mov $3 $4
Sw $3 FP + 8
: ANCHOR_1
Lw $5 FP + 8
Mov $6 $5
Add $7 ZERO 1
Branch ( $6 > $7 ): ANCHOR_3
Mov $13 ZERO
Goto : ANCHOR_4
: ANCHOR_3
Not $13 ZERO
: ANCHOR_4
Branch ( $13 == ZERO ): ANCHOR_2
Lw $8 FP + 8
Mov $10 $8
Lw $9 FP + 4
Mul $9 $10
Sw $9 FP + 4
Add $12 ZERO 1
Lw $11 FP + 8
Sub $11 $12
Sw $11 FP + 8
Goto : ANCHOR_1
: ANCHOR_2
Lw $14 FP + 4
Mov $15 $14
Sub SP 12
Sw $15 FP + 0
Ret
Sub SP 12
: ANCHOR_0
Goto : ANCHOR_5
> main
Add SP 12
Mov $17 SP
Add SP 8
Add $16 ZERO 3
Push $16
Mov SP $17
Call : fact
Lw $18 SP + 12
Add $19 ZERO 6
Add $20 $18 $19
Sub SP 12
Sw $20 FP + 0
Ret
Sub SP 12
: ANCHOR_5
Call : main
Lw $21 SP + 12
Mov $22 SP
Add $22 4
Set $23 77
Push $23
Set $23 97
Push $23
Set $23 105
Push $23
Set $23 110
Push $23
Set $23 32
Push $23
Set $23 101
Push $23
Set $23 120
Push $23
Set $23 105
Push $23
Set $23 116
Push $23
Set $23 101
Push $23
Set $23 100
Push $23
Set $23 32
Push $23
Set $23 119
Push $23
Set $23 105
Push $23
Set $23 116
Push $23
Set $23 104
Push $23
Set $23 32
Push $23
Set $23 99
Push $23
Set $23 111
Push $23
Set $23 100
Push $23
Set $23 101
Push $23
Set $23 58
Push $23
Set $23 32
Push $23 , ZERO , $22
Call : print_str
Push $21
Call : print_d
Sub SP 8
Halt
