import setup
import print
Add SP
Goto : ANCHOR_0
> fact
Add SP
Add $42 ZERO 1
Sw $42 FP + 4
Lw $270 FP + 0
Sw $270 FP + 8
: ANCHOR_1
Lw $272 FP + 8
Add $89 ZERO 1
Branch $272 > $89 : ANCHOR_3
Mov $72 ZERO
Goto : ANCHOR_4
: ANCHOR_3
Not $72 ZERO
: ANCHOR_4
Branch $72 == ZERO : ANCHOR_2
Lw $274 FP + 8
Lw $275 FP + 4
Mul $275 $274
Sw $275 FP + 4
Add $107 ZERO 1
Lw $273 FP + 8
Sub $273 $107
Sw $273 FP + 8
Goto : ANCHOR_1
: ANCHOR_2
Lw $276 FP + 4
Sub SP
Sw $276 FP + 0
Ret
Sub SP
: ANCHOR_0
Goto : ANCHOR_5
> main
Add SP
Add $177 ZERO 0
Sw $177 FP + 8
: ANCHOR_6
Lw $278 FP + 8
Add $201 ZERO 3
Branch $278 < $201 : ANCHOR_8
Mov $184 ZERO
Goto : ANCHOR_9
: ANCHOR_8
Not $184 ZERO
: ANCHOR_9
Branch $184 == ZERO : ANCHOR_7
Lw $280 FP + 8
Push $280
Call : print_d
Add $219 ZERO 1
Lw $279 FP + 8
Add $279 $219
Sw $279 FP + 8
Goto : ANCHOR_6
: ANCHOR_7
Mov $281 SP
Add SP 8
Add $257 ZERO 3
Push $257
Mov SP $281
Call : fact
Lw $242 SP + 12
Add $264 ZERO 6
Add $236 $242 $264
Sub SP
Sw $236 FP + 0
Ret
Sub SP
: ANCHOR_5
Call : main
Lw $282 SP + 12
Add $283 SP 4
Set $284 77
Push $284
Set $284 97
Push $284
Set $284 105
Push $284
Set $284 110
Push $284
Set $284 32
Push $284
Set $284 101
Push $284
Set $284 120
Push $284
Set $284 105
Push $284
Set $284 116
Push $284
Set $284 101
Push $284
Set $284 100
Push $284
Set $284 32
Push $284
Set $284 119
Push $284
Set $284 105
Push $284
Set $284 116
Push $284
Set $284 104
Push $284
Set $284 32
Push $284
Set $284 99
Push $284
Set $284 111
Push $284
Set $284 100
Push $284
Set $284 101
Push $284
Set $284 58
Push $284
Set $284 32
Push $284 ZERO $283
Call : print_str
Sub SP $283 4
Push $282
Call : print_d
Sub SP
Halt