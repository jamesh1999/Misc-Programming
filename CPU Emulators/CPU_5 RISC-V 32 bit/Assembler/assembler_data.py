import os

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
					"JMP":"X30",
					"GP":"X31"}

REGISTER_CONTENTS = {"X0":None,
					"X1":None,
					"X2":None,
					"X3":None,
					"X4":None,
					"X5":None,
					"X6":None,
					"X7":None,
					"X8":None,
					"X9":None,
					"X10":None,
					"X11":None,
					"X12":None,
					"X13":None,
					"X14":None,
					"X15":None,
					"X16":None,
					"X17":None,
					"X18":None,
					"X19":None,
					"X20":None,
					"X21":None,
					"X22":None,
					"X23":None,
					"X24":None,
					"X25":None,
					"X26":None,
					"X27":None,
					"X28":None,
					"X29":None,
					"X30":None,
					"X31":None}

ASSIGNABLE = ["X3", "X4", "X5", "X6", "X7", "X8", "X9", "X10", "X11", "X12", "X13", "X16", "X17", "X18", "X19", "X20", "X21", "X22", "X23", "X24", "X25", "X26", "X27", "X28", "X29"]
CUR_REGISTER = -1



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
			":":"xxxxxxx", #Placeholders to prevent errors
			"ASSEMBLER":"xxxxxxx",
			"SIGNED":"xxxxxxx",
			"UNSIGNED":"xxxxxxx"}

#Instructions that can be made immediate
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

OTHER_MACROS = ["PUSH", "POP", "DEL", "CALL", ">", "RET", "SET"] #Expansion defined in assembler_macros.py



#---------------- GLOBALS ----------------#
GLOBALS = {}
GLOBAL_COUNT = 0



#---------------- ASSEMBLER VARIABLES ----------------#
LOG_TEXT = ""
MAX_JUMPS = {"JAL" : 1048575, "BEQ" : 127, "BLT" : 127, "BLE" : 127, "BLTU" : 127, "BLEU" : 127, "BNE" : 127}



#---------------- ERRORS ----------------#
#Generic error for anything in the assembler
class AssemblerError(Exception):
    def __init__(self, val):
    	self.val = val
    def __str__(self):
    	return "\n\n\nOh noes! The assembler ground to a halt!\nAssemblerError:\n" + repr(self.val)



#---------------- FUNCTIONS ----------------#

#Print to stdout and log
def p(text, e = "\n"):
	global LOG_TEXT
	print(text, end = e)
	LOG_TEXT += str(text) + e

#Save log
def saveLog():
	path = os.path.join(os.path.dirname(__file__), "log.txt")
	with open(path, "w") as log_file:
		log_file.write(LOG_TEXT)

#Get all arbitrary registers assigned in input assembly
def initArbitraryRegisters(assembly):
	global CUR_REGISTER
	CUR_REGISTER = -1
	for line in assembly:
		for token in line:
			try:
				if token[0] == "$" and int(token[1:]) > CUR_REGISTER:
					CUR_REGISTER = int(token[1:])
			except ValueError:
				raise AssemblerError("Integer expected after '$'.")

#Get the next arbitrary register that hasn't been used
def getNextArbitraryRegister():
	return ["$" + str(getNextArbitraryRegisterNum())]
def getNextArbitraryRegisterNum():
	global CUR_REGISTER
	CUR_REGISTER += 1
	return CUR_REGISTER

#Resolve conflicts in arbitrary registers between assembly files
def setUniqueRegisters(assembly):
	global CUR_REGISTER
	maximum = -1
	for i, line in enumerate(assembly):
		for j, token in enumerate(line):
			if token[0] == "$":
				try:
					if int(token[1:]) > maximum:
						maximum = int(token[1:])
					assembly[i][j] = "$" + str(int(token[1:]) + CUR_REGISTER + 1)
				except ValueError:
					raise AssemblerError("Integer expected after '$'.")

	CUR_REGISTER += maximum + 1
	return assembly

#Return the name of a register X--
def getRegister(reg):
	if not isReg(reg):
		raise AssemblerError(reg + " is not a register.")
	else:
		return REGISTER_NAMES[reg] if reg in REGISTER_NAMES else reg

#Checks whether rg i a register
def isReg(reg):
	return True if reg in REGISTER_NAMES or reg[0]=="X" else False

#Separate an instruction into opcode, destination register, input registers, immediate
def splitInstruction(instruction):
	op = instruction[0]

	if op.isdigit():
		return ["","",[],0]

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
		if instruction[i + 1].isdigit():
			immediate = instruction[i + 1]

			if instruction[i] == "-":
				immediate = "-" + immediate


	#Remove output register if a separate input A is provided
	if OPCODES[op][:2]=="01" and len(registers)==3:
		registers = registers[1:]

	elif OPCODES[op][:2]=="10" and len(registers)==2:
		registers = registers[1:]

	elif OPCODES[op][:2]=="11" and len(registers)==3:
		registers = registers[1:]

	return [op, dest, registers, int(immediate)]