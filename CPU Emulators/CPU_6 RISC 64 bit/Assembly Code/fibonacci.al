//Fibonacci using registers
import setup
import print

Lui $0 1000
Add $1 1
Add $2 1

Push $1
Call :print_d

Push $2 $1

:LOOP
Pop $2 $1
Add $3 $1 $2
Push $3
Call :print_d
Push $2 $3
Branch $0 >= $3 :LOOP

Halt