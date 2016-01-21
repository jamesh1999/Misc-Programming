import os

format = []
search = ""
blank = ""
scoped = ""

size_rules = []
default_size = 0
symbol_offsets = []

symbols = []



#Container for data to return
class SymbolTable(object):
	def __init__(self):
		self.format = []
		self.scoped = ""

		self.symbols = []
		self.scope_offsets = []

		self.cur_scope = []

	#Returns information from the symbol table
	def symbolQuery(self, query):
		#Get arguments
		query = query[query.index('(') + 1: query.index(')')]
		args = []
		for token in query:
			if not token == ',':
				args.append(token)

		highest = -1
		symbol = []
		for s in self.symbols:

			#Check whether symbol is in scope
			test_scope = s[self.format.index("SCOPE")]
			for i,level in enumerate(test_scope):
				if not level == self.cur_scope[i]:
					break;
			else:
				#If symbol matches
				if s[0] == args[0]:
					highest = len(test_scope)
					symbol = s
		else:
			return None

		#Scope query
		if(args[0] == "SCOPE_LEN"):
			for offset in scope_offsets:
				if offset[0] == self.cur_scope:
					return str(offset[1])

		#Information query
		elif len(args) == 2:
			if args[1] == "id":
				return symbol[self.format.index("STRING")]
			elif args[1] == "type":
				t = ""
				for i, part in enumerate(symbol):
					if i == self.format.index("STRING"):
						break;
					if i == self.format.index("ADDR"):
						break;
					if i == self.format.index("SIZE"):
						break;
					if i == self.format.index("SCOPE"):
						break;

					t += part

				return part

			elif args[1] == "addr":
				return str(symbol[self.format.index("ADDR")])
			elif args[1] == "size":
				return str(symbol[self.format.index("SIZE")])
			elif args[1] == "scope":
				return str(symbol[self.format.index("SCOPE")])

		#Check query
		elif len(args) == 3:
			nquery = ["?", "(", args[0], ",", args[1], ")"]
			return str(self.symbolQuery(nquery) == args[2])

	#Check whether a node is scoped (for code generator)
	def isScoped(node):
		return node[0] == self.scoped

	#Set the current scope for queries
	def updateScope(self, scope):
		self.cur_scope = scope

	#Print the table
	def __str__(self):
		ret = " | ".join(self.format)
		for symbol in self.symbols:
			ret += "\n"
			for i,val in enumerate(symbol):
				if len(str(val)) < len(str(self.format[i])):
					ret += (len(str(self.format[i])) - len(str(val))) * ' '
				ret += str(val) + " | "

		return ret



#Read config
path = os.path.join(os.path.dirname(__file__), "Configuration/symbol_generator.conf")
with open(path, "r") as ifile:
	lines = ifile.readlines()

	#Parse lines
	for line in lines:
		parts = line.split("::=")

		if len(parts) == 2:
			lhs = parts[0].strip("<>\t\n ")
			rhs = parts[1].split()

			for i, part in enumerate(rhs):
				if not part == "->":
					rhs[i] = part.strip("<>\t\n ")

			#Symbol table configuration
			if lhs == "FORMAT":
				format = rhs
			elif lhs == "DEFINITION":
				search = rhs[0]
			elif lhs == "DEFAULT":
				blank = rhs[0].strip("\"'")
			elif lhs == "SCOPED":
				scoped = rhs[0]

			#Type size configuration
			elif "->" in rhs:
				x = rhs.index("->")
				size_rules.append([lhs, rhs[:x], int(rhs[-1])])
			elif lhs == "DEFAULT_SIZE":
				default_size = int(rhs[0])



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

	#Create symbol
	if main:
		symbols.append([blank] * len(format))

		#Get offset
		for i, offset in enumerate(symbol_offsets):
			if offset[0] == cur_scope:
				break;

		symbols[-1][format.index("ADDR")] = symbol_offsets[i][1]
		symbols[-1][format.index("SCOPE")] = cur_scope[:]

	#Get type from parse tree
	for part in node[1]:
		#Add info if it isn't already there
		if part[0] in format:
			if symbols[-1][format.index(part[0])] == blank:
				symbols[-1][format.index(part[0])] = concatTerminals(part)

		elif not isinstance(part, str):
			#Search symbol for other parts
			addSymbol(part, main = False)

	#Work out size and add offset
	if main:
		for rule in size_rules:
			if symbols[-1][format.index(rule[0])] == rule[1]:
				s = rule[2]
				break;
		else:
			s = default_size

		symbols[-1][format.index("SIZE")] = s
		symbol_offsets[i][1] += s

#Recursively get all symbols in a tree
cur_scope = []
max_at_level = 0
def getSymbols(parse_tree):
	global cur_scope
	global max_at_level
	global symbol_offsets
	symbol_offsets.append([cur_scope, 0])

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

	#Package data
	to_return = SymbolTable()
	to_return.symbols = symbols
	to_return.scope_offsets = symbol_offsets
	to_return.format = format
	to_return.scoped = scoped

	return to_return