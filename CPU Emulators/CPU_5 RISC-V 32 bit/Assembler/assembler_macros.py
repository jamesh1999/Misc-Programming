from Assembler.assembler_data import *



def convertBasicMacro(macro):
	template = list(BASIC_MACROS[macro[0]])

	#Handle macro registers, immediates...
	try:
		for i, elem in enumerate(template):
			if elem[0] == "%":
				if len(elem) > 3 and elem[-3:] == "...":
					template = template[:i] + macro[int(elem[1:-3]):] + template[i + 1:]

				else:
					template[i] = macro[int(elem[1:])]

	except IndexError:
		i = 0
		for elem in template:
			if elem[0] == "%":
				if len(elem) > 3 and elem[-3:] == "...":
					elem = elem[:-3]
				if int(elem) > i:
					i = int(elem)

		raise AssemblerError(macro[0] + " requires at least " + str(i) + " terms.")

	return template

#Expand a macro into its corresponding instruction(s)
def convertMacro(macro):
	instructions = []
	op = macro[0]
	
	try:

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
			instructions.append(["ADDUI", "FP", "SP", "4"])
		
		#Stack return
		elif op == "RET":
			instructions.append(["SUBUI", "SP", "FP", "12"])
			instructions.append(["LW", "FP", "SP", "+", "4"])
			instructions.append(["LW", "RA", "SP", "+", "8"])
			instructions.append(["JALR", "ZERO", "RA", "+", "4"])

		#Set register
		elif op == "SET":
			upper = int(macro[2]) // 4096
			lower = int(macro[2]) % 4096

			if upper:
				instructions.append(["LUI", macro[1], str(upper)])
				if lower:
					instructions.append(["ADDUI", macro[1], str(lower)])
			elif lower:
				instructions.append(["ADDUI", macro[1], "ZERO", str(lower)])

	except IndexError:
		raise AssemblerError(macro[0] + " requires more terms.")
	except ValueError:
		raise AssemblerError(macro[0] + " expected and integer received a string.")

	return instructions

#Substitute any branch statements with equivalent instructions
def removeBranches(assembly):
	for i, line in enumerate(assembly):

		if line[0] == "BRANCH":
			try:
				new_line = []
				condition = line[1:4]

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

				else:
					raise AssemblerError(condition[1] + "is not a valid Branch condition.")

				new_line += ["ZERO", condition[0], condition[2], ":"]
				new_line.append(line[line.index(":") + 1])
				assembly[i] = new_line

			except IndexError:
				raise AssemblerError(str(line) + " is not a valid Branch statement.")

	return assembly



def macrosAssembly(assembly):

	#Remove branch statements
	p("\tRemoving branches...")
	assembly = removeBranches(assembly)

	#Remove other macros
	p("\tExpanding macros...")
	new_assembly = []
	for line in assembly:
		#Basic macros
		if line[0] in BASIC_MACROS.keys():
			new_assembly.append(convertBasicMacro(line))
			p("\t" + str(line) + "  =>  " + str(new_assembly[-1]) + "\n")

		#Advanced macros
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