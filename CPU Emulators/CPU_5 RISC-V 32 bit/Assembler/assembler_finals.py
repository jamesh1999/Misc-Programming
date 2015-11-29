from assembler_data import *



#Remove all jump anchors
def removeAnchors(assembly):
	jump_anchors = {} #Handle any :jumps

	while True:
		for i in range(len(assembly)):
			line = assembly[i]

			if line[0] == ":":
				jump_anchors[line[1]] = i
				assembly.pop(i)
				break;
		else:
			break;

	#Calculate jump positions
	while True:
		for i in range(len(assembly)):
			line = assembly[i]

			if ":" in line:
				for j in range(len(line)):
					if line[j] == ':':
						diff = (jump_anchors[line[j+1]]-i)*4
						if diff < 0:
							diff *= -1
							line[j] = "-"
						else:
							line[j] = "+"

						line[j+1] = str(diff)
				assembly[i] = line
				break;
		else:
			break;

	return assembly

def finalPassAssembly(assembly):
	global GLOBAL_COUNT
	
	#Create Globals
	p("\tCreating global variables...")
	new_assembly1 = []
	for line in assembly:
		if line[0] == "VAR":
			GLOBALS[line[2]] = [line[1], 0]
			if "=" in line:
				GLOBALS[line[2]][1] = line[line.index("=") + 1]
				p("\tAssigned " + line[2] + " = " + line[line.index("=") + 1])
			else:
				p("\tAssigned " + line[2] + " = NULL")

			assignment = []
			if line[1] == "UNSIGNED":
				assignment.append(["ADDUI", "X30", "ZERO", GLOBALS[line[2]][1]])
			else:
				assignment.append(["ADDI", "X30", "ZERO", GLOBALS[line[2]][1]])
			assignment.append(["SW", "X30", "GP", "+", str(GLOBAL_COUNT * 4)])
			new_assembly1 += assignment
			GLOBALS[line[2]][1] = GLOBAL_COUNT * 4
			GLOBAL_COUNT += 1

		else:
			new_assembly1.append(line);

	#Remove Globals
	p("\tRemoving globals...")
	new_assembly2 = []
	spare = []
	for r in REGISTER_STATUS.keys():
			if REGISTER_STATUS[r] == "ASSEMBLER":
				spare.append(r)

	for line in new_assembly1:
		splitted = splitInstruction(line)
		used = 0

		append_extra = None
		if splitted[1] in GLOBALS.keys():
			if not splitted[0] == "SW":
				append_extra = ["SW", spare[0], "GP", "+", str(GLOBALS[splitted[1]][1])]
				line[line.index(splitted[1])] = spare[0]
				used += 1
			else:
				new_assembly2.append(["LW", spare[0], "GP", "+", str(GLOBALS[splitted[1]][1])])
				line[line.index(splitted[1])] = spare[0]
				used += 1

		for (i,r) in enumerate(splitted[2]):
			if r in GLOBALS.keys():
				if not splitted[2][i] in line:
					new_assembly2.append(["LW", spare[0], "GP", "+", str(GLOBALS[r][1])])
				else:
					new_assembly2.append(["LW", spare[used], "GP", "+", str(GLOBALS[r][1])])
					while splitted[2][i] in line:
						line[line.index(splitted[2][i])] = spare[used]
					used += 1

		new_assembly2.append(line)

		if not append_extra == None:
			new_assembly2.append(append_extra)

	#Jump anchors
	p("\tRemoving jump anchors...")
	new_assembly3 = removeAnchors(new_assembly2)

	return new_assembly3