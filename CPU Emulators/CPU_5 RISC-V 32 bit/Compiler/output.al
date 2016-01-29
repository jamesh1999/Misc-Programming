import setup
import print
Add SP 4
Goto :SKIP
> main
Add SP 4
Add $0 ZERO 2
Add $1 ZERO 5
Mul $3 $0 $1
Mov $2 $3
Sw $2 FP + 0
Add $5 ZERO 10
Lw $4 FP + 0
Add $4 $5
Sw $4 FP + 0
Lw $6 FP + 0
Mov $7 $6
Sub SP 4
Push $7
Ret
Sub SP 4
:SKIP
Sub SP 4
Call :main
Call :print_d
Pop $8
Mov $9 SP
Add $9 4
Add $10 ZERO 77
Push $10
Add $10 ZERO 97
Push $10
Add $10 ZERO 105
Push $10
Add $10 ZERO 110
Push $10
Add $10 ZERO 32
Push $10
Add $10 ZERO 101
Push $10
Add $10 ZERO 120
Push $10
Add $10 ZERO 105
Push $10
Add $10 ZERO 116
Push $10
Add $10 ZERO 101
Push $10
Add $10 ZERO 100
Push $10
Add $10 ZERO 32
Push $10
Add $10 ZERO 119
Push $10
Add $10 ZERO 105
Push $10
Add $10 ZERO 116
Push $10
Add $10 ZERO 104
Push $10
Add $10 ZERO 32
Push $10
Add $10 ZERO 99
Push $10
Add $10 ZERO 111
Push $10
Add $10 ZERO 100
Push $10
Add $10 ZERO 101
Push $10
Add $10 ZERO 58
Push $10
Add $10 ZERO 32
Push $10
Mov $10 ZERO
Push $2 , $9
Call :print_str
Push $8
Call :print_d
Halt
