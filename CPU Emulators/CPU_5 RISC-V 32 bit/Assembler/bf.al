ADD FP 996
ADD SP 1000
ADD GP 2000
JAL + 216
LW S1 SP - 0
SUBUI SP 4
SW FP SP + 4
SW RA SP + 8
ADDUI SP 8
ADDU FP SP ZERO
SW ZERO SP + 4
ADDUI SP 4
REM S2 S1 10
ADD S2 48
SW S2 SP + 4
ADDUI SP 4
DIV S1 10
BNE ZERO S1 ZERO - 20
LW S1 SP - 0
SUBUI SP 4
BEQ ZERO S1 ZERO + 12
OUT S1
JAL - 16
ADD S1 ZERO 10
OUT S1
SUBUI SP FP 8
LW FP SP + 4
LW RA SP + 8
JALR ZERO RA + 4
LW S1 SP - 0
SUBUI SP 4
SW FP SP + 4
SW RA SP + 8
ADDUI SP 8
ADDU FP SP ZERO
OUT S1
SUBUI SP FP 8
LW FP SP + 4
LW RA SP + 8
JALR ZERO RA + 4
LW S2 SP - 0
SUBUI SP 4
SW FP SP + 4
SW RA SP + 8
ADDUI SP 8
ADDU FP SP ZERO
LW S1 S2
BEQ ZERO S1 ZERO + 16
OUT S1
ADD S2 4
JAL - 16
ADD S1 ZERO 10
OUT S1
SUBUI SP FP 8
LW FP SP + 4
LW RA SP + 8
JALR ZERO RA + 4
ADDUI X30 ZERO 4000
SW X30 GP + 0
ADDUI X30 ZERO 4000
SW X30 GP + 4
ADDUI X30 ZERO 3000
SW X30 GP + 8
ADDUI X30 ZERO 3000
SW X30 GP + 12
ADDUI X30 ZERO 3255
SW X30 GP + 16
LW X30 GP + 0
ADDUI S1 X30 0
IN S2
SW S2 S1
ADD S1 4
IN S2
BNE ZERO S2 ZERO - 12
LW X30 GP + 4
LW S1 X30
ADD S2 ZERO 62
BNE ZERO S1 S2 + 20
LW X30 GP + 8
ADD X30 4
SW X30 GP + 8
JAL + 260
ADD S2 ZERO 60
BNE ZERO S1 S2 + 20
LW X30 GP + 8
SUB X30 4
SW X30 GP + 8
JAL + 236
ADD S2 ZERO 43
BNE ZERO S1 S2 + 28
LW X30 GP + 8
LW S3 X30
ADD S3 1
LW X30 GP + 8
SW S3 X30
JAL + 204
ADD S2 ZERO 45
BNE ZERO S1 S2 + 28
LW X30 GP + 8
LW S3 X30
SUB S3 1
LW X30 GP + 8
SW S3 X30
JAL + 172
ADD S2 ZERO 91
BNE ZERO S1 S2 + 48
LW X30 GP + 8
LW S3 X30
BNE ZERO S3 ZERO + 32
LW X30 GP + 4
ADD X30 4
SW X30 GP + 4
LW X30 GP + 4
LW S4 X30
ADD S5 ZERO 93
BNE ZERO S4 S5 - 24
JAL + 120
ADD S2 ZERO 93
BNE ZERO S1 S2 + 48
LW X30 GP + 8
LW S3 X30
BEQ ZERO S3 ZERO + 32
LW X30 GP + 4
SUB X30 4
SW X30 GP + 4
LW X30 GP + 4
LW S4 X30
ADD S5 ZERO 91
BNE ZERO S4 S5 - 24
JAL + 68
ADD S2 ZERO 44
BNE ZERO S1 S2 + 32
LW X30 GP + 8
LW S3 X30
SW S3 SP + 4
ADDUI SP 4
JAL RA 4
JAL - 532
JAL + 32
ADD S2 ZERO 46
BNE ZERO S1 S2 + 20
IN S3
LW X30 GP + 8
SW S3 X30
JAL + 8
JAL + 40
LW X30 GP + 8
ADD X30 4
SW X30 GP + 8
LW X30 GP + 16
LW X29 GP + 8
BLE ZERO X30 X29 + 16
LW X30 GP + 12
LW X29 GP + 8
BLE ZERO X30 X29 - 320
HALT
