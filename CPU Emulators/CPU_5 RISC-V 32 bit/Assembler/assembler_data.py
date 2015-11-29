#---------------- REGISTER INFORMATION ----------------#
REGISTER_NAMES = {"ZERO":"X0",
					"RA":"X1",
					"FP":"X2",
					"S1":"X3",
					"S2":"X4",
					"S3":"X5",
					"S4":"X6",
					"S5":"X7",
					"S6":"X8",
					"S7":"X9",
					"S8":"X10",
					"S9":"X11",
					"S10":"X12",
					"S11":"X13",
					"SP":"X14",
					"TP":"X15",
					"S12":"X16",
					"S13":"X17",
					"S14":"X18",
					"S15":"X19",
					"S16":"X20",
					"S17":"X21",
					"S18":"X22",
					"S19":"X23",
					"S20":"X24",
					"S21":"X25",
					"S22":"X26",
					"S23":"X27",
					"S24":"X28",
					"S25":"X29",
					"S26":"X30",
					"GP":"X31"}

REGISTER_STATUS = {"X0":"UNSIGNED",
					"X1":"UNSIGNED",
					"X2":"UNSIGNED",
					"X3":"SIGNED",
					"X4":"SIGNED",
					"X5":"SIGNED",
					"X6":"SIGNED",
					"X7":"SIGNED",
					"X8":"SIGNED",
					"X9":"SIGNED",
					"X10":"SIGNED",
					"X11":"SIGNED",
					"X12":"SIGNED",
					"X13":"SIGNED",
					"X14":"UNSIGNED",
					"X15":"UNSIGNED",
					"X16":"SIGNED",
					"X17":"SIGNED",
					"X18":"SIGNED",
					"X19":"SIGNED",
					"X20":"SIGNED",
					"X21":"SIGNED",
					"X22":"SIGNED",
					"X23":"SIGNED",
					"X24":"SIGNED",
					"X25":"SIGNED",
					"X26":"ASSEMBLER",
					"X27":"ASSEMBLER",
					"X28":"ASSEMBLER",
					"X29":"ASSEMBLER",
					"X30":"ASSEMBLER",
					"X31":"UNSIGNED"}

LAST_CHANGED = {"X0":-1,
					"X1":-1,
					"X2":-1,
					"X3":-1,
					"X4":-1,
					"X5":-1,
					"X6":-1,
					"X7":-1,
					"X8":-1,
					"X9":-1,
					"X10":-1,
					"X11":-1,
					"X12":-1,
					"X13":-1,
					"X14":-1,
					"X15":-1,
					"X16":-1,
					"X17":-1,
					"X18":-1,
					"X19":-1,
					"X20":-1,
					"X21":-1,
					"X22":-1,
					"X23":-1,
					"X24":-1,
					"X25":-1,
					"X26":-1,
					"X27":-1,
					"X28":-1,
					"X29":-1,
					"X30":-1,
					"X31":-1}

REGISTER_CONTENTS = {"X0":0,
					"X1":0,
					"X2":0,
					"X3":0,
					"X4":0,
					"X5":0,
					"X6":0,
					"X7":0,
					"X8":0,
					"X9":0,
					"X10":0,
					"X11":0,
					"X12":0,
					"X13":0,
					"X14":0,
					"X15":0,
					"X16":0,
					"X17":0,
					"X18":0,
					"X19":0,
					"X20":0,
					"X21":0,
					"X22":0,
					"X23":0,
					"X24":0,
					"X25":0,
					"X26":0,
					"X27":0,
					"X28":0,
					"X29":0,
					"X30":0,
					"X31":0}



#---------------- INSTRUCTION INFORMATION ----------------#
OPCODES = {"LUI":"0000000",
			"JAL":"0000001",
			"CWAIT":"0000010",
			"TIME":"0000011",
			"HALT":"0000100",
			"ADD":"0100000",
			"ADDU":"0100000",
			"SUB":"0100000",
			"SUBU":"0100000",
			"AND":"0100000",
			"OR":"0100000",
			"XOR":"0100000",
			"NOT":"0100000",
			"SHL":"0100001",
			"SHLU":"0100001",
			"SHR":"0100001",
			"SHRU":"0100001",
			"MUL":"0100010",
			"MULH":"0100010",
			"MULU":"0100010",
			"MULHU":"0100010",
			"DIV":"0100010",
			"DIVU":"0100010",
			"REM":"0100010",
			"REMU":"0100010",
			"CORE":"1000111",
			"LW":"1000001",
			"LH":"1000001",
			"LB":"1000001",
			"LHU":"1000001",
			"LBU":"1000001",
			"ADDI":"1000010",
			"ADDUI":"1000010",
			"SUBI":"1000010",
			"SUBUI":"1000010",
			"ANDI":"1000010",
			"ORI":"1000010",
			"XORI":"1000010",
			"NOTI":"1000010",
			"SHLI":"1000011",
			"SHLUI":"1000011",
			"SHRI":"1000011",
			"SHRUI":"1000011",
			"MULI":"1000100",
			"MULHI":"1000100",
			"MULUI":"1000100",
			"MULHUI":"1000100",
			"DIVI":"1000100",
			"DIVUI":"1000100",
			"REMI":"1000100",
			"REMUI":"1000100",
			"SW":"1000101",
			"SH":"1000101",
			"SB":"1000101",
			"JALR":"1000111",
			"BEQ":"0100011",
			"BLT":"0100011",
			"BLE":"0100011",
			"BLTU":"0100011",
			"BLEU":"0100011",
			"BNE":"0100011",
			"AMOSWAP":"1100000",
			"OUT":"1001001",
			"IN":"1001000",
			":":"xxxxxxx",
			"ASSEMBLER":"xxxxxxx",
			"SIGNED":"xxxxxxx",
			"UNSIGNED":"xxxxxxx"}

UNSIGNED_TYPE = ["ADD", "SUB", "SHL", "SHR", "MUL", "MULH", "DIV", "REM", "LH", "LB", "ADDI", "SUBI", "SHLI", "SHRI", "MULI", "MULHI", "DIVI", "REMI" "BLT", "BLE"]

IMMEDIATE_TYPE = ["ADD", "ADDU", "SUB", "SUBU", "AND", "OR", "XOR", "NOT", "SHL", "SHLU", "SHR", "SHRU", "MUL", "MULH", "MULU", "MULHU", "DIV", "DIVU", "REM", "REMU"]



#---------------- MACROS ----------------#
#% indicates part of macro and index can only be 1 digit, ... can only be used at end and indicates all remainder of macro
BASIC_MACROS = {"NOOP":["ADD", "ZERO", "ZERO"], 				#No operation
				"BGT":["BLE", "%1", "%3", "%2", "%4..."],		#Branch greater than
				"BGTU":["BLEU", "%1", "%3", "%2", "%4..."],		#Branch greater than unsigned
				"BGE":["BLT", "%1", "%3", "%2", "%4..."],		#Branch greater than or equal to
				"BGEU":["BLTU", "%1", "%3", "%2", "%4..."],		#Branch greater than or equal to unsigned
				"GOTO":["JAL", "%1", "%2"],						#Goto address
				"TOP":["LW", "%1", "SP"],						#View stack top
				"INC":["ADDI", "%1", "1"],						#Increment
				"INCU":["ADDUI", "%1", "1"],					#Increment unsigned
				"DEC":["SUBI", "%1", "1"],						#Decrement
				"DECU":["SUBUI", "%1", "1"],					#Decrement unsigned
				"MOV":["ADDUI", "%1", "%2", "0"],				#Move
				"CLEAR":["ADD", "%1", "ZERO", "ZERO"]}			#Clear register

OTHER_MACROS = ["PUSH", "POP", "DEL", "CALL", ">", "RET"]



#---------------- GLOBALS ----------------#
GLOBALS = {}
GLOBAL_COUNT = 0



#---------------- ASSEMBLER VARIABLES ----------------#
LOG_TEXT = ""



#---------------- ERRORS ----------------#
#Generic error for anything in the assembler
class AssemblerError(Exception):
    pass



#---------------- FUNCTIONS ----------------#

#Print to stdout and log
def p(text, e = "\n"):
	global LOG_TEXT
	print(text, end = e)
	LOG_TEXT += str(text) + e

#Save log
def saveLog():
	with open("log.txt", "w") as log_file:
		log_file.write(LOG_TEXT)
		log_file.close()

#Return the name of a register X--
def getRegister(reg):
	return REGISTER_NAMES[reg] if reg in REGISTER_NAMES else reg

#Checks whether rg i a register
def isReg(reg):
	return True if reg in REGISTER_NAMES or reg[0]=="X" else False

#Separate an instruction into opcode, destination register, input registers, immediate
def splitInstruction(instruction):
	op = instruction[0]

	#Get destination
	if len(instruction) > 1 and (isReg(instruction[1]) or instruction[1] in GLOBALS):
		dest = instruction[1]
	else:
		dest = "ZERO"

	#Get input registers
	registers = []
	for i in instruction[1:]:
		if isReg(i) or i in GLOBALS:
			registers.append(i)

	#Find an immediate else 0
	immediate = "0"
	for i in range(len(instruction) - 1):
		if instruction[i+1].isdigit():
			immediate = instruction[i+1]

			if instruction[i] == "-":
				immediate = "-" + immediate

	if OPCODES[op][:2]=="01" and len(registers)==3:
		registers = registers[1:]

	elif OPCODES[op][:2]=="10" and len(registers)==2:
		registers = registers[1:]

	elif OPCODES[op][:2]=="11" and len(registers)==3:
		registers = registers[1:]

	return [op, dest, registers, int(immediate)]

#Find out whether an instruction should be signed/unsigned or immediate/register
def getInstructionType(instruction):
	#Check if all registers are unsigned
	for i in instruction:
		if (isReg(i) and REGISTER_STATUS[getRegister(i)] == "SIGNED") or (i in GLOBALS.keys() and GLOBALS[i][0] == "SIGNED"):
			unsigned = False
			break;
	else:
		unsigned = True

	#Check for an immediate
	for i in instruction:
		try:
			i = int(i)
			immediate = True
			break;
		except:
			pass
	else:
		immediate = False

	return unsigned, immediate