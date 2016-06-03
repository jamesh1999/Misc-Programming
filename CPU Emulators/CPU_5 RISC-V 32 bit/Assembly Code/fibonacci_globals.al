//Fibonacci using global variables
import setup
import print

Var a = 1
Var b = 1
Var temp = 0

Lui $0 1000

Push a
Call :print_d

:LOOP
Mov temp a
Addu a b
Mov b temp
Push a
Call :print_d
Branch $0 >= a :LOOP

Halt