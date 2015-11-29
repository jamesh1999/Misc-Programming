//Fibonacci using registers
import setup
import print

unsigned@S1
unsigned@S2
unsigned@S3
unsigned@S4

Lui S4 1000
Add S1 1
Add S2 1

Push S1
Call :print_d

Push S2 S1

:LOOP
Pop S2 S1
Add S3 S1 S2
Push S3
Call :print_d
Push S2 S3
Branch (S4 >= S3) :LOOP

Halt