import setup
import print
Add SP 8
Goto : ANCHOR_0
> fact
Add SP 12
Add $39 ZERO 1
Sw $39 FP + 4
Lw $264 FP + 0
Sw $264 FP + 8
: ANCHOR_1
Lw $266 FP + 8
Add $85 ZERO 1
Branch $266 > $85 : ANCHOR_3
Mov $68 ZERO
Goto : ANCHOR_4
: ANCHOR_3
Not $68 ZERO
: ANCHOR_4
Branch $68 == ZERO : ANCHOR_2
Lw $268 FP + 8
Lw $269 FP + 4
Mul $269 $268
Sw $269 FP + 4
Add $103 ZERO 1
Lw $267 FP + 8
Sub $267 $103
Sw $267 FP + 8
Goto : ANCHOR_1
: ANCHOR_2
Lw $270 FP + 4
Sub SP 12
Sw $270 FP + 0
Ret
Sub SP 12
: ANCHOR_0
Goto : ANCHOR_5
> main
Add SP 12
Add $171 ZERO 0
Sw $171 FP + 8
: ANCHOR_6
Lw $272 FP + 8
Add $195 ZERO 3
Branch $272 < $195 : ANCHOR_8
Mov $178 ZERO
Goto : ANCHOR_9
: ANCHOR_8
Not $178 ZERO
: ANCHOR_9
Branch $178 == ZERO : ANCHOR_7
Lw $274 FP + 8
Push $274
Call : print_d
Add $213 ZERO 1
Lw $273 FP + 8
Add $273 $213
Sw $273 FP + 8
Goto : ANCHOR_6
: ANCHOR_7
Mov $275 SP
Add SP 8
Add $251 ZERO 3
Push $251
Mov SP $275
Call : fact
Lw $236 SP + 12
Add $258 ZERO 6
Add $230 $236 $258
Sub SP 12
Sw $230 FP + 0
Ret
Sub SP 12
: ANCHOR_5
Call : main
Lw $276 SP + 12
Add $277 SP 4
Set $278 77
Push $278
Set $278 97
Push $278
Set $278 105
Push $278
Set $278 110
Push $278
Set $278 32
Push $278
Set $278 101
Push $278
Set $278 120
Push $278
Set $278 105
Push $278
Set $278 116
Push $278
Set $278 101
Push $278
Set $278 100
Push $278
Set $278 32
Push $278
Set $278 119
Push $278
Set $278 105
Push $278
Set $278 116
Push $278
Set $278 104
Push $278
Set $278 32
Push $278
Set $278 99
Push $278
Set $278 111
Push $278
Set $278 100
Push $278
Set $278 101
Push $278
Set $278 58
Push $278
Set $278 32
Push $278 ZERO $277
Call : print_str
Sub SP $277 4
Push $276
Call : print_d
Sub SP 8
Halt