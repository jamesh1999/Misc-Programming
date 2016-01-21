from assembler_data import *



#Remove all jump anchors
def removeAnchors(assembly):
	jump_anchors = {} #Handle any :jumps

	try:
		while True:
			for i in range(len(assembly)):
				line = assembly[i]

				if line[0] == ":":
					jump_anchors[line[1]] = i
					assembly.pop(i)
					break;
			else:
				break;
	except IndexError:
		raise AssemblerError("Jump anchor expected after ':'.")

	#Calculate jump positions
	while True:
		for i in range(len(assembly)):
			line = assembly[i]

			if ":" in line:
				for j in range(len(line)):
					if line[j] == ':':
						try:
							anchor = line[j + 1]
						except IndexError:
							raise AssemblerError("Jump anchor expected after ':'.")

						try:
							diff = (jump_anchors[anchor] - i) * 4
						except IndexError:
							raise AssemblerError(anchor + " is not a valid jump anchor.")

						if diff < 0:
							diff *= -1
							line[j] = "-"
						else:
							line[j] = "+"

						line[j + 1] = str(diff)
				assembly[i] = line
				break;
		else:
			break;

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

		except IndexError:
			raise AssemblerError(str(line) + " is not a valid global declaration.")

		else:
			new_assembly.append(line);

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
					new_assembly.append(["SW", register, "GP", "-", str(int(REGISTER_CONTENTS[getRegister(register)]) * 4)])
					new_assembly.append(["LW", register, "GP", "-", str(int(token[1:]) * 4)])

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