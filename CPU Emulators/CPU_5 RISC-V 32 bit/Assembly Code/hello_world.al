//Basic hello world program
import setup
import print

Var h = 72
Var e = 101
Var l = 108
Var o = 111
Var space = 32
Var w = 119
Var r = 114
Var d = 100
Var nl = 10

Mov $0 SP
Addu $0 4
Push h, e, l, l, o, space, w, o, r, l, d, nl, ZERO, $0
Call :print_str
Halt