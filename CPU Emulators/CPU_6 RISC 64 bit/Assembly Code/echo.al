//Echo input
import setup
import print
1
Add $1 SP 4

:JUMP_BACK
In $2
Push $2
Branch $2 != ZERO :JUMP_BACK

Push $1
Call :print_str
Halt