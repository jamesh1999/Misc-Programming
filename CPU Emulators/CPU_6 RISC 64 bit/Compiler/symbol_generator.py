import os, yaml



#Errors for parser and ParseTree class
class SymbolGeneratorError(Exception):
	pass
class SymbolTableError(Exception):
	pass



format = []
search = []
blank = ""
scoped = []
no_overhead = []
scope_overhead = 0
arguments = []

type_search = []
modifier_types = []
base_types = []
other_types = []

size_rules = []
default_size = 0
symbol_offsets = [[[0], 0]]

symbols = []



#Container for data to return
class SymbolTable(object):
	def __init__(self, symbols):
		self.format = []
		self.scoped = []
		self.no_overhead = []

		self.__symbols = symbols

		self.scope_offsets = []
		self.cur_scope = []
		self.max_at_level = 0
		self.scope_overhead = 0

	#Returns information from the symbol table
	def query(self, args):

		#Special queries
		if len(args) == 1:
			#Scope query
			if args[0] == "SCOPE_OFFSET":
				for offset in self.scope_offsets:
					if offset[0] == self.cur_scope:
						return str(offset[1])
				return "0"
			else:
				raise SymbolTableError(args[0] + " is not a special query.")

		#Symbol table query
		elif len(args) == 2:

			#Get corresponding symbol (if any)
			highest = -1
			symbol = []
			for s in self.__symbols:

				#Check whether symbol is in scope
				test_scope = s[self.format.index("SCOPE")]

				#In sub-scope
				if len(test_scope) > len(self.cur_scope):
					continue;

				#In different scope
				for i,level in enumerate(test_scope):
					if not level == self.cur_scope[i]:
						break;
				else:
					#If symbol matches
					if s[0] == args[0]:
						highest = len(test_scope)
						symbol = s
						break;

			#No matching symbol
			if symbol == []:
				return None

			if args[1] == "id":
				return symbol[self.format.index("STRING")]
			elif args[1] == "type":
				return symbol[self.format.index("TYPE")]

			elif args[1] == "addr":
				#Calculate stack offset for differing scopes
				offset = 0
				symbol_scope = symbol[self.format.index("SCOPE")]
				#Count levels with offsets
				cnt = 0
				for level in self.cur_scope[len(symbol_scope):]:
					if not isinstance(level, str):
						cnt += 1
				offset += cnt * self.scope_overhead

				#Factor in scope sizes
				for i in range(len(self.getRealScope(symbol_scope)), len(self.getRealScope(self.cur_scope))):
					working_scope = self.getRealScope(self.cur_scope)[:i]
					for x in self.scope_offsets:
						if x[0] == working_scope:
							offset += x[1]

				relative_addr = symbol[self.format.index("ADDR")] - offset
				ret = []
				if relative_addr < 0:
					ret.append("-")
					relative_addr *= -1
				else:
					ret.append("+")
				ret.append(str(relative_addr))

				return ' '.join(ret)
			elif args[1] == "size":
				return str(symbol[self.format.index("SIZE")])
			elif args[1] == "scope":
				return str(symbol[self.format.index("SCOPE")])
			elif args[1][:9] == "arguments_type":
				index = args.split('[')[:-1]
				return str(symbol[self.format.index("ARGUMENTS")][int(index)][0])
			elif args[1][:9] == "arguments_size":
				index = args.split('[')[:-1]
				return str(symbol[self.format.index("ARGUMENTS")][int(index)][1])
			elif args[1] in self.format:
				return str(symbol[self.format.index(args[1])])
			else:
				raise SymbolTableError(str(query) + " is not a valid query.")

		# #Check query
		# elif len(args) == 3:
		# 	nquery = ["?", "(", args[0], ",", args[1], ")"]
		# 	return str(self.que(nquery) == args[2])

		else:
			raise SymbolTableError("Badly formatted query: " + ' '.join(args))

	#Check whether a node is scoped (for code generator)
	def isScoped(self, node):
		return node.getType() in self.scoped

	#Handles scope changes at start of node
	def updateScope(self, node):
		if self.isScoped(node):
			if node.getType() in no_overhead:
				self.cur_scope.append(str(self.max_at_level))
			else:
				self.cur_scope.append(self.max_at_level)
		self.max_at_level = 0

	#Handles scope changes at end of node
	def endUpdateScope(self, node):
		if self.isScoped(node):
			self.cur_scope = self.cur_scope[:-1]
			if len(self.cur_scope) > 0:
				self.max_at_level = int(self.cur_scope[-1]) + 1
			else:
				self.max_at_level = 0

	#Print the table
	def __str__(self):
		ret = ""
		for col in self.format:
			ret += max(16 - len(str(col)), 0) * ' ' + str(col) + " | "
		ret = ret[:-3]
		ret += '\n' + '-' * len(ret)

		type_index = self.format.index("TYPE")

		for symbol in self.__symbols:
			ret += "\n"

			for i, val in enumerate(symbol):
				if i == type_index:
					add = ' '
					for level in val:
						add += ' '.join(level[0]) + ' ' + level[1] + ' '
					add = add[:-1] #Remove trailing space

				else:
					add = str(val)
				ret += max(16 - len(add), 0) * ' ' + add + " | "

			ret = ret[:-3]

		return ret

	#Gets the stack scope
	def getRealScope(self, scope):
		ret = []
		for val in scope:
			if not isinstance(val, str):
				ret.append(val)
		return ret



#Read config
path = os.path.join(os.path.dirname(__file__), "Configuration/symbol_generator.yml")
with open(path, "r") as ifile:
	
	#Parse YAML
	data = yaml.load(ifile)

	#Split all BNF definitions
	for key in ["FORMAT", "DEFINITION", "SCOPED", "NO_OVERHEAD", "ARGUMENTS", "TYPE", "TYPE_MODIFIERS", "TYPE_BASE", "TYPE_OTHER"]:
		if not key in data:
			continue;

		parts = data[key].split()

		for i, part in enumerate(parts):
			parts[i] = part.strip("<>\t\n ")

		data[key] = parts

	#Extract data for symbol table
	format = data["FORMAT"]
	search = data["DEFINITION"]
	blank = data["DEFAULT"]
	scoped = data["SCOPED"]
	no_overhead = data["NO_OVERHEAD"]
	scope_overhead = int(data["SCOPE_OVERHEAD"])
	arguments = data["ARGUMENTS"]
	type_search = data["TYPE"]
	modifier_types = data["TYPE_MODIFIERS"]
	other_types = data["TYPE_OTHER"]
	base_types = data["TYPE_BASE"]

	#Get type size rules
	default_size = int(data["DEFAULT_SIZE"])
	for rule in data["SIZES"]:
		top_level = None
		bottom_level = None
		top_level_modifiers = None
		bottom_level_modifiers = None

		if "top_level" in rule["conditions"]:
			top_level = rule["conditions"]["top_level"]
		if "bottom_level" in rule["conditions"]:
			bottom_level = rule["conditions"]["bottom_level"]
		if "top_level_modifiers" in rule["conditions"]:
			top_level_modifiers = rule["conditions"]["top_level_modifiers"]
		if "bottom_level_modifiers" in rule["conditions"]:
			bottom_level_modifiers = rule["conditions"]["bottom_level_modifiers"]

		d = [top_level, bottom_level, top_level_modifiers, bottom_level_modifiers, int(rule["size"])]
		size_rules.append(d)

#Add a symbol to the list
def addSymbol(node, main = True, index = -1):
	global symbols
	global symbol_offsets

	#Create symbol
	if main:
		symbols.append([blank] * len(format))

		#Get offset
		for i, offset in enumerate(symbol_offsets):
			if offset[0] == cur_real_scope:
				break;

		symbols[-1][format.index("ADDR")] = symbol_offsets[i][1]
		symbols[-1][format.index("SCOPE")] = list(cur_scope)
		symbols[-1][format.index("ARGUMENTS")] = []

		symbols[-1][format.index("TYPE")] = []

	#Get type from parse tree
	for part in node:
		if isinstance(part, str):
			continue;

		#Add new type level and switch level for child calls
		if part.getType() in type_search:
			symbols[-1][format.index("TYPE")].append([[], ""])
			addSymbol(part, main = False, index = index + 1)

		#Add modifier/base/other types to current level
		elif part.getType() in modifier_types:
			symbols[-1][format.index("TYPE")][index][0].append(part.getTerminals())
		elif part.getType() in base_types or part.getType() in other_types:
			symbols[-1][format.index("TYPE")][index][1] = part.getTerminals()

		#Add other format data
		elif part.getType() in format:
			if symbols[-1][format.index(part.getType())] == blank:
				symbols[-1][format.index(part.getType())] = ''.join(part.getTerminals())

		#Search symbol for other parts
		else:
			addSymbol(part, main = False, index = index)

	#Work out size and add offset
	if main:
		for rule in size_rules:
			if rule[0] != symbols[-1][format.index("TYPE")][-1][1]:
				continue;
			if rule[1] != symbols[-1][format.index("TYPE")][0][1]:
				continue;
			if rule[2] != symbols[-1][format.index("TYPE")][-1][0]:
				continue;
			if rule[3] != symbols[-1][format.index("TYPE")][0][0]:
				continue;

			#Rule applies
			s = rule[4]
			break;
		else:
			s = default_size

		symbols[-1][format.index("SIZE")] = s
		symbol_offsets[i][1] += s

#Apply arguments to previous symbol
def applyArguments(node, index = None):
	global symbols

	if index == None:
		index = len(symbols) - 1
	for part in node:
		if not isinstance(part, str):
			if part.getType() in search:

				addSymbol(part)
				symbols[index][format.index("ARGUMENTS")].append([symbols[-1][format.index("TYPE")], symbols[-1][format.index("SIZE")]])
			else:
				applyArguments(part, index = index)

#Recursively get all symbols in a tree
cur_scope = [0] 	 #Stack scope
cur_real_scope = [0] #Visibility scope
max_at_level = 0
def getSymbols(parse_tree):
	global cur_scope
	global cur_real_scope
	global max_at_level
	global symbol_offsets

	for node in parse_tree:
		#Ignore terminals
		if isinstance(node, str):
			continue;

		#Set scope for next symbols & search new scope
		if node.getType() in scoped:
			if node.getType() in no_overhead:
				cur_scope.append(str(max_at_level)) #Mark for no overhead
			else:
				cur_scope.append(max_at_level)
				cur_real_scope.append(max_at_level)
				symbol_offsets.append([list(cur_real_scope), 0])
			tmp = max_at_level + 1
			max_at_level = 0

			#Add symbols in scope
			getSymbols(node)

			max_at_level = tmp
			cur_scope = cur_scope[:-1]
			if not node.getType() in no_overhead:
				cur_real_scope = cur_real_scope[:-1]

		#Add symbol
		elif node.getType() in search:
			addSymbol(node)

		#Add arguments
		elif node.getType() in arguments:
			applyArguments(node)

		#Search child node
		else:
			getSymbols(node)

#Runs everything and outputs table
def generateSymbolTable(parse_tree):
	getSymbols(parse_tree)

	#Package data
	to_return = SymbolTable(symbols)
	to_return.scope_offsets = symbol_offsets
	to_return.format = format
	to_return.scoped = scoped
	to_return.no_overhead = no_overhead
	to_return.scope_overhead = scope_overhead

	return to_return