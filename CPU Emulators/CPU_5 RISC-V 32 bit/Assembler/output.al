import setup
import print
Add SP 4
Goto :SKIP
> main
Add SP 4
Add $0 ZERO 2
Add $1 ZERO 5
Mul $2 $0 $1
Add $3 ZERO 6
Add $5 $2 $3
Mov $4 $5
Sw $4 FP + 0
Add $7 ZERO 10
Lw $6 FP + 0
Add $6 $7
Sw $6 FP + 0
Add $9 ZERO 2
Lw $8 FP + 0
Div $8 $9
Sw $8 FP + 0
Lw $10 FP + 0
Mov $11 $10
Sub SP 4
Sw $11 FP - 12
Ret
Sub SP 4
:SKIP
Call :main
Mov $12 SP
Add $12 4
Set $13 77
Push $13
Set $13 97
Push $13
Set $13 105
Push $13
Set $13 110
Push $13
Set $13 32
Push $13
Set $13 101
Push $13
Set $13 120
Push $13
Set $13 105
Push $13
Set $13 116
Push $13
Set $13 101
Push $13
Set $13 100
Push $13
Set $13 32
Push $13
Set $13 119
Push $13
Set $13 105
Push $13
Set $13 116
Push $13
Set $13 104
Push $13
Set $13 32
Push $13
Set $13 99
Push $13
Set $13 111
Push $13
Set $13 100
Push $13
Set $13 101
Push $13
Set $13 58
Push $13
Set $13 32
Push $13 , ZERO , $12
Call :print_str
Lw $14 FP + 0
Push $14
Call :print_d
Sub SP 4
Halt
