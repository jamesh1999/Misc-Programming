format = []
search = ""
blank = ""
scoped = ""

symbols = []
CUR_ADDR = 0

with open("definitions.bnf") as ifile:
	lines = ifile.readlines()

	#Get lines for symbol generator
	relevant = []
	reading = False
	for line in lines:
		if line.strip() == "#SYMBOL":
			reading = True
			continue;
		elif line[0] == "#":
			reading = False
			continue;

		if reading:
			relevant.append(line)

	#Parse lines
	for line in relevant:
		parts = line.split("::=")

		if len(parts) == 2:
			lhs = parts[0].strip("<>\t\n ")
			rhs = parts[1].split()

			for i,part in enumerate(rhs):
				rhs[i] = part.strip("<>\t\n ")

			if lhs == "FORMAT":
				format = rhs
			elif lhs == "DEFINITION":
				search = rhs[0]
			elif lhs == "DEFAULT":
				blank = rhs[0].strip("\"'")
			elif lhs == "SCOPED":
				scoped = rhs[0]

#Concatenate all terminals
buff = ""
def concatTerminals(node, reset = True):
	global buff

	if reset:
		buff = ""

	for part in node[1]:
		if isinstance(part, str):
			buff += part
		else:
			concatTerminals(part, reset = False)

	return buff

#Add a symbol to the list
def addSymbol(node, main = True):
	global symbols
	global cur_scope
	global CUR_ADDR

	if main:
		symbols.append([blank] * (len(format) + 1) + [cur_scope[:]])
		symbols[-1][4] = CUR_ADDR
		CUR_ADDR += 4

	for part in node[1]:
		#Add info if it isn't already there
		if part[0] in format:
			if symbols[-1][format.index(part[0])] == blank:
				symbols[-1][format.index(part[0])] = concatTerminals(part)
		elif not isinstance(part, str):
			#Search symbol for other parts
			addSymbol(part, main = False)

#Recursively get all symbols in a tree
cur_scope = []
max_at_level = 0
def getSymbols(parse_tree):
	global cur_scope
	global max_at_level

	for node in parse_tree[1]:
		#Ignore terminals
		if isinstance(node, str):
			continue;

		#Set scope for next symbols & search new scope
		if node[0] == scoped:

			cur_scope.append(max_at_level)
			tmp = max_at_level + 1
			max_at_level = 0

			getSymbols(node)

			max_at_level = tmp
			cur_scope = cur_scope[:-1]

		#Add symbol
		elif node[0] == search:
			addSymbol(node)

		#Search child node
		else:
			getSymbols(node)

#Runs everything and outputs table
def generateSymbolTable(parse_tree):
	getSymbols(parse_tree)

	return symbols