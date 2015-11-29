from assembler_data import *



def convertBasic(macro):
	template = BASIC_MACROS[macro[0]][:]

	#Handle macro registers, immediates...
	for i,elem in enumerate(template):
		if elem[0] == "%":
			if len(elem) > 3 and elem[-3:] == "...":
				template = template[:i] + macro[int(elem[1]):]

			else:
				template[i] = macro[int(elem[1])]

	return template

#Expand a macro into its corresponding instruction(s)
def convertMacro(macro):
	instructions = []
	op = macro[0]
	
	#Stack push
	if op == "PUSH":
		cnt = 0
		for i in macro[1:]:
			cnt += 4
			instructions.append(["SW", i, "SP", "+", str(cnt)])
		instructions.append(["ADDUI", "SP", str(cnt)])
	
	#Stack pop
	elif op == "POP":
		cnt = 0
		for i in macro[1:]:
			instructions.append(["LW", i, "SP", "-", str(cnt)])
			cnt += 4
		instructions.append(["SUBUI", "SP", str(cnt)])
	
	#Stack delete
	elif op == "DEL":
		dist = int(macro[1]) * 4
		instructions.append(["SUBUI", "SP", str(dist)])
	
	#Stack call
	elif op == "CALL":
		instructions.append(["JAL", "RA", "4"])
		instructions.append(["JAL", macro[1], macro[2]])
	
	#Function start points
	elif op == ">":
		instructions.append([":",macro[1]])
		cnt = 0
		if len(macro) > 2:
			for val in macro[2:]:
				instructions.append(["LW", val, "SP", "-", str(cnt)])
				cnt += 4
			instructions.append(["SUBUI", "SP", str(cnt)])
		instructions.append(["SW", "FP", "SP", "+", "4"])
		instructions.append(["SW", "RA", "SP", "+", "8"])
		instructions.append(["ADDUI", "SP", "8"])
		instructions.append(["ADDU", "FP", "SP", "ZERO"])
	
	#Stack return
	elif op == "RET":
		instructions.append(["SUBUI", "SP", "FP", "8"])
		instructions.append(["LW", "FP", "SP", "+", "4"])
		instructions.append(["LW", "RA", "SP", "+", "8"])
		instructions.append(["JALR", "ZERO", "RA", "+", "4"])

	return instructions

def removeIfs(assembly):
	if_cnt = 0
	for i,line in enumerate(assembly):
		if line[0] == "IF":
			depth = 0
			found = False
			for j, other_line in enumerate(assembly[i:]):
				for k,val in enumerate(other_line):
					if val=="{" :
						depth += 1
						if depth == 1:
							assembly[i + j].pop(k) 
							if assembly[i + j] == []:
								assembly.pop(i + j)
					elif val=="}":
						depth -= 1

						if depth == 0:
							assembly[i] = ["BRANCH", "(", line[2], "", line[4], ")", ":", "IFSTMT_"+str(if_cnt)]
							assembly[i + j - 1] = [":", "IFSTMT_"+str(if_cnt)] # -1 because removed earlier instruction
							if_cnt += 1

							if line[3] == "==":
								assembly[i][3] = "!="

							elif line[3] == "!=":
								assembly[i][3] = "=="

							elif line[3] == ">":
								assembly[i][3] = "<="

							elif line[3] == ">=":
								assembly[i][3] = "<"

							elif line[3] == "<":
								assembly[i][3] = ">="

							elif line[3] == "<=":
								assembly[i][3] = ">"
							found = True
							break;		
				if found:
					found = False
					break;					

	return assembly

#Remove any branch statements
def removeBranches(assembly):
	for i,line in enumerate(assembly):

		if line[0] == "BRANCH":
			new_line = []
			condition = line[2:5]

			if condition[1] == "==":
				new_line.append('BEQ')

			elif condition[1] == "!=":
				new_line.append('BNE')

			elif condition[1] == ">":
				new_line.append('BGT')

			elif condition[1] == ">=":
				new_line.append('BGE')

			elif condition[1] == "<":
				new_line.append('BLT')

			elif condition[1] == "<=":
				new_line.append('BLE')

			if "[" in line:
				new_line.append(line[line.index("[") + 1])
			else:
				new_line.append("ZERO")

			new_line += [condition[0], condition[2], ":"]
			new_line.append(line[line.index(":") + 1])
			assembly[i] = new_line

	return assembly



def macrosAssembly(assembly):
	#Remove if statements
	p("\tRemoving ifs...")
	assembly = removeIfs(assembly)

	#Remove branch statements
	p("\tRemoving branches...")
	assembly = removeBranches(assembly)

	#Remove other macros
	p("\tExpanding macros...")
	new_assembly = []
	for line in assembly:
		if line[0] in BASIC_MACROS.keys():
			new_assembly.append(convertBasic(line))
			p("\t" + str(line) + "  =>  " + str(new_assembly[-1]) + "\n")

		elif line[0] in OTHER_MACROS:
			expanded = convertMacro(line)
			new_assembly += expanded
			p("\t" + str(line) + "  =>  [" + str(expanded[0]) + ",")
			for i in range(len(expanded) - 1):
				p("\t\t\t" + str(expanded[i + 1]) + ",")
			p("")

		else:
			new_assembly.append(line)

	return new_assembly