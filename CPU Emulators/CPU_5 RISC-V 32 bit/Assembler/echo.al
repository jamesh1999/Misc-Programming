//Echo input
import setup
import print

Add S2 SP 4

:JUMP_BACK
In S1
Push S1
Branch(S1 != ZERO) :JUMP_BACK

Push S2
Call :print_str
Halt