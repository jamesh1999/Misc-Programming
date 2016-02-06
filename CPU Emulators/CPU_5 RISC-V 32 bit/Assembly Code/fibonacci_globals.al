//Fibonacci using global variables
import setup
import print

Var unsigned a = 1
Var unsigned b = 1
Var unsigned temp = 0

Lui $0 1000

Push a
Call :print_d

:LOOP
Mov temp a
Add a b
Mov b temp
Push a
Call :print_d
Branch $0 >= a :LOOP

Halt