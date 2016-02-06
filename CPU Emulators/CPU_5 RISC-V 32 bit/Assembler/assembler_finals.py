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
						#Substitute in JAL instruction
						if -MAX_JUMPS["JAL"] - 1 <= diff <= MAX_JUMPS["JAL"]:
							insert = [["JAL", ":", "INSERTED_" + str(inserted)],
									  ["JAL", ":", line[line.index(":") + 1]],
									  [":", "INSERTED_" + str(inserted)]]
							inserted += 1
							line[line.index(":") + 1] = "8"
							line[line.index(":")] = "+"


						#Substitute a JALR instruction
						else:
							insert = [["JAL", ":", "INSERTED_" + str(inserted)],
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
				GLOBALS[line[2]] = [line[1], 0]

				if "=" in line:
					GLOBALS[line[2]][1] = line[line.index("=") + 1]
					p("\tAssigned " + line[2] + " = " + line[line.index("=") + 1])
				else:
					p("\tAssigned " + line[2] + " = NULL")

				assignment = []
				if line[1] == "UNSIGNED":
					assignment.append(["ADDUI", line[2], "ZERO", GLOBALS[line[2]][1]])
				elif line[1] == "SIGNED":
					assignment.append(["ADDI", line[2], "ZERO", GLOBALS[line[2]][1]])
				else:
					raise AssemblerError("Expected unsigned/signed instead of " + line[1] + ".")

				new_assembly += assignment
				GLOBALS[line[2]][1] = GLOBAL_COUNT * 4
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
				append_extra = ["SW"] + register_out + ["GP", "+", str(GLOBALS[splitted[1]][1])]
			else:
				new_assembly.append(["LW"] + register_out + ["GP", "+", str(GLOBALS[splitted[1]][1])])

			line = line[:line.index(splitted[1])] + register_out + line[line.index(splitted[1]) + 1:]

		#Handle globals as input registers
		for (i,r) in enumerate(splitted[2]):
			if r in GLOBALS.keys():

				if not r in line:
					new_assembly.append(["LW"] + register_out + ["GP", "+", str(GLOBALS[r][1])])
				else:
					register = getNextArbitraryRegister()
					new_assembly.append(["LW"] + register + ["GP", "+", str(GLOBALS[r][1])])
					while r in line:
						line = line[:line.index(r)] + register + line[line.index(r) + 1:]

		new_assembly.append(line)

		if not append_extra == None:
			new_assembly.append(append_extra)
	return new_assembly

def removeArbitraryRegisters(assembly):
	global REGISTER_CONTENTS
	last_used = ASSIGNABLE[:]

	new_assembly = []
	for line in assembly:
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

				#Move register to end of queue
				last_used.remove(register)
				last_used.append(getRegister(register))
				REGISTER_CONTENTS[getRegister(register)] = token[1:]

				line[line.index(token)] = register

		new_assembly.append(line)

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