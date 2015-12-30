Goto :PRINT_SKIP

>print_d $0
Push ZERO
:PRINTD_REPEAT
Rem $1 $0 10
Add $1 48
Push $1
Div $0 10
Branch ($0 != ZERO) :PRINTD_REPEAT
:PRINTD_DIGITS
Pop $0
Branch ($0 == ZERO) :PRINTD_DONE
Out $0
Goto :PRINTD_DIGITS
:PRINTD_DONE
Add $0 ZERO 10
Out $0
Ret

>print_ch $0
Out $0
Ret

>print_str $1
:PRINTSTR_AGAIN
Lw $0 $1
Branch ($0 == ZERO) :PRINTSTR_END
Out $0
Add $1 4
Goto :PRINTSTR_AGAIN
:PRINTSTR_END
Add $0 ZERO 10
Out $0
Ret


:PRINT_SKIP