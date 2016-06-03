from Assembler.assembler_data import *


#Find out whether an instruction should be immediate/register
def getInstructionType(instruction):
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

	return immediate

#Return binary value for a register
def getRegisterBinary(reg):
	num = int(getRegister(reg)[1:])
	binary = str(bin(num))[2:]
	return binary.zfill(5)

#Get the binary value for an immediate
def immToBinary(num, length, signed = False):
	if signed:
		if num < 0:
			nnum = -1 * (num + 1)
		else:
			nnum = num
		binary = str(bin(int(nnum)))[2:]

		if len(binary) > length-1:
			raise AssemblerError("Signed immediate exceeds maximum/minimum value")

		if nnum != num:
			binary = binary.zfill(length)
			nbinary = ""
			for i in range(len(binary)):
				if binary[i] == "1":
					nbinary += "0"
				else:
					nbinary += "1"
			binary = nbinary


		return binary.zfill(length)

	else:
		if num < 0:
			raise AssemblerError("Unsigned immediate less than 0")

		binary = str(bin(num))[2:]

		if len(binary) > length:
			raise AssemblerError("Unsigned immediate exceeds maximum value")

		return binary.zfill(length)

#Convert a single instruction to binary
def convertInstruction(instruction):
	binary = ""

	immediate = getInstructionType(instruction)

	#Switch to immediate instructions if necessary
	if immediate and instruction[0] in IMMEDIATE_TYPE:
		instruction[0] += "I"

	splitted = splitInstruction(instruction)

	#Number
	if splitted[0] == "":
		return [immToBinary(int(instruction[0]), 32)]

	#Get opcode
	try:
		binary += OPCODES[splitted[0]]
	except ValueError:
		raise AssemblerError(splitted[0] + " is not a macro or instruction.")

	#Add rd to instruction
	binary += getRegisterBinary(splitted[1])

	try:
		if splitted[0] == "LUI":
			binary += immToBinary(splitted[3], 20)

		elif splitted[0] == "JAL":
			binary += immToBinary(splitted[3]/2, 20, signed=True)

		elif splitted[0] == "BEQ":
			binary += "010" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + immToBinary(splitted[3]/2, 7, signed=True)

		elif splitted[0] == "BNE":
			binary += "000" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + immToBinary(splitted[3]/2, 7, signed=True)

		elif splitted[0] == "BLT":
			binary += "001" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + immToBinary(splitted[3]/2, 7, signed=True)

		elif splitted[0] == "BLE":
			binary += "011" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + immToBinary(splitted[3]/2, 7, signed=True)

		elif splitted[0] == "BLTU":
			binary += "101" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + immToBinary(splitted[3]/2, 7, signed=True)

		elif splitted[0] == "BLEU":
			binary += "111" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + immToBinary(splitted[3]/2, 7, signed=True)

		elif splitted[0] == "LW":
			binary += "100" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12, signed=True)

		elif splitted[0] == "LH":
			binary += "101" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12, signed=True)

		elif splitted[0] == "LB":
			binary += "110" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12, signed=True)

		elif splitted[0] == "LHU":
			binary += "001" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12, signed=True)

		elif splitted[0] == "LBU":
			binary += "010" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12, signed=True)

		elif splitted[0] == "ADDI":
			binary += "000" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12, signed=True)

		elif splitted[0] == "ADDUI":
			binary += "001" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12)

		elif splitted[0] == "SUBI":
			binary += "010" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12, signed=True)

		elif splitted[0] == "SUBUI":
			binary += "011" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12)

		elif splitted[0] == "ANDI":
			binary += "100" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12)

		elif splitted[0] == "ORI":
			binary += "101" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12)

		elif splitted[0] == "XORI":
			binary += "110" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12)

		elif splitted[0] == "ADD":
			binary += "001" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + "0000000"

		elif splitted[0] == "ADDU":
			binary += "001" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + "0000000"

		elif splitted[0] == "SUB":
			binary += "011" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + "0000000"

		elif splitted[0] == "SUBU":
			binary += "011" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + "0000000"

		elif splitted[0] == "AND":
			binary += "100" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + "0000000"

		elif splitted[0] == "OR":
			binary += "101" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + "0000000"

		elif splitted[0] == "XOR":
			binary += "110" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + "0000000"

		elif splitted[0] == "NOT":
			binary += "111" + getRegisterBinary(splitted[2][1]) + "000000000000"

		elif splitted[0] == "SHLI":
			binary += "010" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12)

		elif splitted[0] == "SHLUI":
			binary += "110" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12)

		elif splitted[0] == "SHRI":
			binary += "001" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12)

		elif splitted[0] == "SHRUI":
			binary += "101" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12)

		elif splitted[0] == "SHL":
			binary += "010" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + "0000000"

		elif splitted[0] == "SHLU":
			binary += "110" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + "0000000"

		elif splitted[0] == "SHR":
			binary += "001" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + "0000000"

		elif splitted[0] == "SHRU":
			binary += "101" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + "0000000"

		elif splitted[0] == "MULI":
			binary += "000" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12)

		elif splitted[0] == "MULHI":
			binary += "001" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12)

		elif splitted[0] == "MULUI":
			binary += "100" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12)

		elif splitted[0] == "MULHUI":
			binary += "101" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12)

		elif splitted[0] == "DIVI":
			binary += "010" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12)

		elif splitted[0] == "DIVUI":
			binary += "110" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12)

		elif splitted[0] == "REMI":
			binary += "011" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12)

		elif splitted[0] == "REMUI":
			binary += "111" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12)

		elif splitted[0] == "MUL":
			binary += "000" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + "0000000"

		elif splitted[0] == "MULH":
			binary += "001" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + "0000000"

		elif splitted[0] == "MULU":
			binary += "100" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + "0000000"

		elif splitted[0] == "MULHU":
			binary += "101" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + "0000000"

		elif splitted[0] == "DIV":
			binary += "010" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + "0000000"

		elif splitted[0] == "DIVU":
			binary += "110" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + "0000000"

		elif splitted[0] == "REM":
			binary += "011" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + "0000000"

		elif splitted[0] == "REMU":
			binary += "111" + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + "0000000"

		elif splitted[0] == "JALR":
			binary += "000" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3]/2, 12, signed=True)

		elif splitted[0] == "AMOSWAP":
			imm_binary = immToBinary(splitted[3], 10, signed=True)
			binary += imm_binary[7:] + getRegisterBinary(splitted[2][0]) + getRegisterBinary(splitted[2][1]) + imm_binary[:7]

		elif splitted[0] == "HALT":
			binary+="00000000000000000000"

		elif splitted[0] == "TIME":
			binary+="00000000000000000000"

		elif splitted[0] == "CORE":
			binary += "000" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12)

		elif splitted[0] == "CWAIT":
			binary += immToBinary(splitted[3], 20)

		elif splitted[0] == "SW":
			binary += "000" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12, signed=True)

		elif splitted[0] == "SH":
			binary += "001" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12, signed=True)

		elif splitted[0] == "SB":
			binary += "010" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12, signed=True)

		elif splitted[0] == "IN":
			binary += "000" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12, signed=True)

		elif splitted[0] == "OUT":
			binary += "000" + getRegisterBinary(splitted[2][0]) + immToBinary(splitted[3], 12, signed=True)

	except IndexError:
		raise AssemblerError("More registers expected for " + splitted[0] + ".")

	return [binary]

#Convert an entire program to binary
def convertAssembly(assembly):
	binary = []
	for instruction in assembly:
		p("\t" + str(instruction) + "  => ", e = " ")
		binary += convertInstruction(instruction)
		p(str(binary[-1]))

	return binary