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
Add SP 0
Lw $8 FP + 8
Mov $10 $8
Lw $9 FP + 4
Mul $9 $10
Sw $9 FP + 4
Sub SP 0
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
Sub SP 12
Sw $18 FP + 0
Ret
Sub SP 12
: ANCHOR_5
Call : main
Lw $19 SP + 12
Mov $20 SP
Add $20 4
Set $21 77
Push $21
Set $21 97
Push $21
Set $21 105
Push $21
Set $21 110
Push $21
Set $21 32
Push $21
Set $21 101
Push $21
Set $21 120
Push $21
Set $21 105
Push $21
Set $21 116
Push $21
Set $21 101
Push $21
Set $21 100
Push $21
Set $21 32
Push $21
Set $21 119
Push $21
Set $21 105
Push $21
Set $21 116
Push $21
Set $21 104
Push $21
Set $21 32
Push $21
Set $21 99
Push $21
Set $21 111
Push $21
Set $21 100
Push $21
Set $21 101
Push $21
Set $21 58
Push $21
Set $21 32
Push $21 , ZERO , $20
Call : print_str
Push $19
Call : print_d
Sub SP 8
Halt
