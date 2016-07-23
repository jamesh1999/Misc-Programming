import setup
import print
Add SP 4
Goto :ANCHOR_0
> main
Add SP 8
Add $1 ZERO 0
Mov $0 $1
Sw $0 FP + 0
Add $3 ZERO 0
Mov $2 $3
Sw $2 FP + 4
:ANCHOR_1
Lw $4 FP + 4
Mov $5 $4
Add $6 ZERO 4
Branch ( $5 < $6 ) :ANCHOR_3
Mov $19 ZERO
Goto :ANCHOR_4
:ANCHOR_3
Not $19 ZERO
:ANCHOR_4
Branch ( $19 == ZERO ) :ANCHOR_2
Add SP 4
Add $8 ZERO 0
Mov $7 $8
Sw $7 FP + 0
:ANCHOR_5
Lw $9 FP + 0
Mov $10 $9
Add $11 ZERO 3
Branch ( $10 < $11 ) :ANCHOR_7
Mov $16 ZERO
Goto :ANCHOR_8
:ANCHOR_7
Not $16 ZERO
:ANCHOR_8
Branch ( $16 == ZERO ) :ANCHOR_6
Add SP 0
Add $13 ZERO 1
Lw $12 FP - 28
Add $12 $13
Sw $12 FP - 28
Sub SP 0
Add $15 ZERO 1
Lw $14 FP + 0
Add $14 $15
Sw $14 FP + 0
Goto :ANCHOR_5
:ANCHOR_6
Goto :ANCHOR_2
Sub SP 4
Add $18 ZERO 1
Lw $17 FP + 4
Add $17 $18
Sw $17 FP + 4
Goto :ANCHOR_1
:ANCHOR_2
Lw $20 FP + 0
Mov $21 $20
Sub SP 8
Sw $21 FP - 12
Ret
Sub SP 8
:ANCHOR_0
Call :main
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
Call :print_str
Lw $24 FP + 0
Push $24
Call :print_d
Sub SP 4
Halt
