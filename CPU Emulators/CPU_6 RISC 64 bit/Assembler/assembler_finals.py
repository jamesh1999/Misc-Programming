from Assembler.assembler_data import *



#Remove all jump anchors
def removeAnchors(assembly):
	inserted = 0

	#Add necessary placeholder instructions
	while True:
		i_offset = 0
		for i, line in enumerate(assembly):

			if ":" in line:
				if line[0] == ":":
					i_offset += 1
				else:
					x = line.index(":")

					try:
						anchor = line[x + 1]
					except IndexError:
						raise AssemblerError("Jump anchor expected after ':'.")

					j_offset = 0
					for j, test_line in enumerate(assembly):
						if test_line[0] == ":":
							if test_line[1] == anchor:
								break;
							else:
								j_offset += 1
					else:
						raise AssemblerError(anchor + " is not a valid jump anchor.")

					#Get difference
					diff = ((j - j_offset) - (i - i_offset)) * 4

					#Difference too great
					if line[0] in MAX_JUMPS and (diff > MAX_JUMPS[line[0]] or diff < -MAX_JUMPS[line[0]] - 1):
						if line[0] == "JAL":
							inverted = ["JAL"]
						#Invert the branch condition
						elif line[0] == "BEQ":
							inverted = line[:line.index(':')]
							inverted[0] = "BNE"
						elif line[0] == "BNE":
							inverted = line[:line.index(':')]
							inverted[0] = "BEQ"
						elif line[0] == "BLT":
							inverted = ["BLE", line[2], line[1]]
						elif line[0] == "BLE":
							inverted = ["BLT", line[2], line[1]]
						elif line[0] == "BLTU":
							inverted = ["BLEU", line[2], line[1]]
						elif line[0] == "BLEU":
							inverted = ["BLTU", line[2], line[1]]


						#Substitute in JAL instruction
						if -MAX_JUMPS["JAL"] - 1 <= diff <= MAX_JUMPS["JAL"]:
							insert = [inverted + [":", "INSERTED_" + str(inserted)],
									  ["JAL", ":", line[line.index(":") + 1]],
									  [":", "INSERTED_" + str(inserted)]]
							inserted += 1
							line[line.index(":") + 1] = "8"
							line[line.index(":")] = "+"


						#Substitute a JALR instruction
						else:
							insert = [inverted + [":", "INSERTED_" + str(inserted)],
									  ["ADDUI", "ZERO", "ZERO"],
									  ["ADDUI", "ZERO", "ZERO"],
									  ["JALR", "ZERO", "JMP", line[line.index(":") + 1]],
									  [":", "INSERTED_" + str(inserted)]]
							inserted += 1
							line[line.index(":") + 1] = "8"
							line[line.index(":")] = "+"

						assembly = assembly[:i] + [line] + insert + assembly[i + 1:]
						break;
		else:
			break;

	#Remove anchors and calculate definite positions
	jump_anchors = {}
	while True:
		for i, line in enumerate(assembly):
			if line[0] == ":":
				jump_anchors[line[1]] = i
				assembly.pop(i)
				break;
		else:
			break;

	#Calculate jump positions
	for i, line in enumerate(assembly):

		if ':' in line:
			for j in range(len(line)):
				if line[j] == ':':
					anchor = line[j + 1]
					
					#Normal jump no need to set JMP
					if not line[0] == "JALR":
						diff = (jump_anchors[anchor] - i) * 4

						if diff < 0:
							diff *= -1
							line[j] = '-'
						else:
							line[j] = '+'

						line[j + 1] = str(diff)

					#JALR instruction requires JMP
					else:
						assembly[i - 2] = ["LUI", "JMP", str(jump_anchors[anchor] // 4096)]
						assembly[i - 1] = ["ADDUI", "JMP", str(jump_anchors[anchor] % 4096)]
						line = ["JALR", "ZERO", "JMP", '+', '0']

			assembly[i] = line

	return assembly

#Assign memory locations and default values for global variables
def createGlobals(assembly):
	global GLOBAL_COUNT
	global GLOBALS

	new_assembly = []
	for line in assembly:
		try:

			if line[0] == "VAR":
				GLOBALS[line[1]] = 0

				if "=" in line:
					GLOBALS[line[1]] = line[line.index("=") + 1]
					p("\tAssigned " + line[1] + " = " + line[line.index("=") + 1])
				else:
					p("\tAssigned " + line[1] + " = NULL")

				new_assembly.append(["ADDUI", line[1], "ZERO", GLOBALS[line[1]]])
				GLOBALS[line[1]] = GLOBAL_COUNT * 4
				GLOBAL_COUNT += 1

			else:
				new_assembly.append(line)

		except IndexError:
			raise AssemblerError(str(line) + " is not a valid global declaration.")

	return new_assembly

#Substitute any references to globals
def removeGlobalReferences(assembly):
	new_assembly = []
	for line in assembly:
		splitted = splitInstruction(line)

		#Handle global as output register
		append_extra = None
		if splitted[1] in GLOBALS.keys():
			register_out = getNextArbitraryRegister()

			if not splitted[0] == "SW":
				append_extra = ["SW"] + register_out + ["GP", "+", str(GLOBALS[splitted[1]])]
			else:
				new_assembly.append(["LW"] + register_out + ["GP", "+", str(GLOBALS[splitted[1]])])

			line = line[:line.index(splitted[1])] + register_out + line[line.index(splitted[1]) + 1:]

		#Handle globals as input registers
		for (i,r) in enumerate(splitted[2]):
			if r in GLOBALS.keys():

				if not r in line:
					new_assembly.append(["LW"] + register_out + ["GP", "+", str(GLOBALS[r])])
				else:
					register = getNextArbitraryRegister()
					new_assembly.append(["LW"] + register + ["GP", "+", str(GLOBALS[r])])
					while r in line:
						line = line[:line.index(r)] + register + line[line.index(r) + 1:]

		new_assembly.append(line)

		if not append_extra == None:
			new_assembly.append(append_extra)
	return new_assembly

def removeArbitraryRegisters(assembly):
	global REGISTER_CONTENTS
	last_used = ASSIGNABLE[:]

	cnt = 0
	used = []
	new_assembly = []
	for line in assembly:
		#Save before jump so contents not lost
		if line[0] in MAX_JUMPS or line[0] == "JALR" or line[0] == ':':
			for register in REGISTER_CONTENTS:
				if not REGISTER_CONTENTS[register] == None:
					new_assembly.append(["SW", register, "GP", "-", str(int(REGISTER_CONTENTS[getRegister(register)]) * 4 + 4)])
					REGISTER_CONTENTS[register] = None

		for token in line:
			if token[0] == "$":
				for r in REGISTER_CONTENTS:
					if REGISTER_CONTENTS[r] == token[1:]:
						register = r
						break;
				else:
					register = last_used[0]
					if not REGISTER_CONTENTS[getRegister(register)] == None:
						new_assembly.append(["SW", register, "GP", "-", str(int(REGISTER_CONTENTS[getRegister(register)]) * 4 + 4)])
					new_assembly.append(["LW", register, "GP", "-", str(int(token[1:]) * 4 + 4)])

					#Update performance variables
					if not token in used:
						used.append(token)
						cnt += 1

				#Move register to end of queue
				if register in last_used:
					last_used.remove(register)
					last_used.append(register)
				REGISTER_CONTENTS[register] = token[1:]

				line[line.index(token)] = register

		new_assembly.append(line)

	if cnt > 0:
		p("\tArbitrary registers substituted with " + str(len(used)/cnt * 100)[:5] + "% efficiency")
	else:
		p("\tNo arbitrary registers.")

	return new_assembly

def finalPassAssembly(assembly):
	
	#Create Globals
	p("\tCreating global variables...")
	assembly = createGlobals(assembly)

	#Remove Globals
	p("\tRemoving globals...")
	assembly = removeGlobalReferences(assembly)

	#Remove arbitrary registers
	p("\tRemoving arbitrary registers...")
	assembly = removeArbitraryRegisters(assembly)

	#Jump anchors
	p("\tRemoving jump anchors...")
	assembly = removeAnchors(assembly)

	return assembly