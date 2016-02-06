//Basic hello world program
import setup
import print

Var unsigned h = 72
Var unsigned e = 101
Var unsigned l = 108
Var unsigned o = 111
Var unsigned space = 32
Var unsigned w = 119
Var unsigned r = 114
Var unsigned d = 100
Var unsigned nl = 10

Mov $0 SP
Add $0 4
Push h, e, l, l, o, space, w, o, r, l, d, nl, ZERO, $0
Call :print_str
Halt