Addui S2 4095
Addui S3 4095

:LOOPTWO
:LOOP
Addui S1 1
Branch (S1 != S2) :LOOP
Add S1 ZERO ZERO
Addui S4 1
Branch (S4 != S3) :LOOPTWO

Halt