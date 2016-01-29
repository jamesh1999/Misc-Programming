import os
from tokeniser import *
import copy

CURRENT_REGISTER = 0
CURRENT_SCOPE = []
MAX_AT_LEVEL = 0

definitions = []
templates = {}
built_in = {}

symbols = []



#Read code generator config
path = os.path.join(os.path.dirname(__file__), "Configuration/code_generator.conf")
with open(path, "r") as ifile:
	lines = ifile.readlines()

	for line in lines:
		parts = line.split("::=")

		if len(parts) == 2:
			lhs = parts[0].strip()
			rhs = parts[1].split()

			lhs = lhs.split("[")
			num = int(lhs[1].strip(" \t\n[]"))
			lhs = lhs[0].strip("<> \t\n")

			nrhs = []
			for part in rhs:
				npart = part.strip("<> \t\n")
				if npart[0] == '"' and npart[-1] == '"':
					tokens = tokenise(npart[1:-1])
					for t in tokens:
						nrhs.append('"' + t + '"')
				else:
					nrhs.append(npart)

			definitions.append([lhs, num + 1, nrhs]) #Increment num 0 is default case

#Read templates
path = os.path.join(os.path.dirname(__file__), "Configuration/templates.al")
with open(path, "r") as ifile:
	lines = ifile.readlines()

	#Separate templates
	temp = []
	for line in lines:
		tokens = tokenise(line)
		if len(tokens) > 0 and tokens[0] == "<":
			temp.append([tokens])
		elif len(tokens) > 0:
			temp[-1].append(tokens)

	#Combine definitions and templates into dictionary
	for template in temp:
		node = template[0]

		if not node[2] in templates:
			templates[node[2]] = []

		if len(node) == 8:
			num = int(node[5]) + 1
		else:
			num = 0

		while len(templates[node[2]]) <= num:
			templates[node[2]].append([])

		templates[node[2]][num] = template[1:]

#Read built-in functions
path = os.path.join(os.path.dirname(__file__), "Configuration/Built-in Functions")
for filename in next(os.walk(path))[2]:
	file_path = os.path.join(path, filename)

	with open(file_path, "r") as ifile:
		lines = ifile.readlines()
		built_in[filename.split('.')[0]] = lines



#Get the first instance of a node in a parse tree
def getFirstInstanceOf(tree, search):
	for node in tree[1]:
		#Skip string literals
		if isinstance(node, str):
			continue;

		if node[0] == search:
			return node

		x = getFirstInstanceOf(node, search)
		if not x == None:
			return x

	return None

#Check if the tree is equivalent to the definition
def isDefinitionMatch(tree, definition, pos = 0, root = True):
	#Does definition apply to this node
	if root and definition[0] != tree[0]:
		return False

	#Does definition match
	check = definition[2]
	for node in tree[1]:

		#Check that string matches
		if isinstance(node, str):
			if len(check[pos]) > 2 and check[pos][1:-1] == node:
				pos += 1
			else:
				break;

		#Check that node matches
		elif check[pos] == node[0]:
			pos += 1
		else:
			b, pos = isDefinitionMatch(node, definition, pos = pos, root = False)
			if not b:
				break;
	else:
		#Matches (so far)
		if root:
			return True if pos == len(check) else False #Return true if at end of definition
		else:
			return True, pos

	#Doesn't match
	if root:
		return False
	else:
		return False, 0

#Set all arbitrary registers to unique values
def setUniqueRegisters(template, child_nodes, child_templates):
	global CURRENT_REGISTER

	#Assign registers used only by template
	assigned = {}
	for i,line in enumerate(template):
		for j,token in enumerate(line):
			if token[0] == '$':
				if not token[1:] in assigned:
					assigned[token[1:]] = "$" + str(CURRENT_REGISTER)
					CURRENT_REGISTER += 1

				template[i] = line[:j] + [assigned[token[1:]]] + line[j + 1:]

	#Assign i/o registers
	assigned = [None] * len(child_nodes)
	for i in range(len(template)):

		#Continue until finished
		while True:
			line = template[i]
			for j in range(len(line)):
				if line[j] == '@' and line[j + 1] == '<' and line[j + 3] == '>':

					#Not assigned
					if assigned[child_nodes.index(line[j + 2])] == None:
						assigned[child_nodes.index(line[j + 2])] = '$' + str(CURRENT_REGISTER)

						#Update child template
						for k, child_line in enumerate(child_templates[child_nodes.index(line[j + 2])]):

							while '@' in child_line:
								y = child_line.index('@')
								child_line = child_line[:y] + ['$' + str(CURRENT_REGISTER)] + child_line[y + 1:]

							child_templates[child_nodes.index(line[j + 2])][k] = child_line

						#Move to new register
						CURRENT_REGISTER += 1
						
					#Update assembly
					template[i] = line[:j] + [assigned[child_nodes.index(line[j + 2])]] + line[j + 4:]
					break;
			else:
				break;

	return [template, child_templates]

#Generate the assembly for a single node in the parse tree
def generateNode(parse_tree):
	global CURRENT_SCOPE
	global MAX_AT_LEVEL

	if symbols.isScoped(parse_tree):
		CURRENT_SCOPE.append(MAX_AT_LEVEL)
		symbols.updateScope(CURRENT_SCOPE)

	child_nodes = []
	child_templates = []
	for node in parse_tree[1]:
		child_nodes.append(node[0])
		if not isinstance(node, str):
			child_templates.append(generateNode(node))
		else:
			child_templates.append([])

	#Get the correct template
	template = []
	if parse_tree[0] in templates:
		for definition in definitions:
			if isDefinitionMatch(parse_tree, definition):
				template = copy.deepcopy(templates[parse_tree[0]][definition[1]])
				break;
		else:
			template = copy.deepcopy(templates[parse_tree[0]][0])

	#Insert pre-frame-change assembly
	prepend = []
	if symbols.isScoped(parse_tree):
		prepend += copy.deepcopy(templates["SCOPE_START"][0])

	#Insert post-frame-change assembly
	if symbols.isScoped(parse_tree):
		template += copy.deepcopy(templates["SCOPE_END"][0])

	#Substitute STRING and INT placeholders
	for i,line in enumerate(template):
		if "INT" in line:
			x = line.index("INT")
			template[i][x] = getFirstInstanceOf(parse_tree, "INT")[1][0]
		if "STRING" in line:
			x = line.index("STRING")
			template[i][x] = getFirstInstanceOf(parse_tree, "STRING")[1][0]

	#Handle all symbol queries
	for i,line in enumerate(template):
		if "?" in line:
			segment = line[line.index("?"):]
			query = line[line.index("?"):line.index("?") + segment.index(")") + 1]
			template[i] = line[:line.index("?")] + [symbols.symbolQuery(query)] + line[line.index("?") + segment.index(")") + 1:]
	for i,line in enumerate(prepend):
		if "?" in line:
			segment = line[line.index("?"):]
			query = line[line.index("?"):line.index("?") + segment.index(")") + 1]
			prepend[i] = line[:line.index("?")] + [symbols.symbolQuery(query)] + line[line.index("?") + segment.index(")") + 1:]

	corrected = setUniqueRegisters(template, child_nodes, child_templates)
	template = corrected[0]
	child_templates = corrected[1]

	#Insert the assembly from the child nodes
	#Insert the assembly for child nodes in the middle
	while True:
		for i,line in enumerate(template):
			if len(line) >= 3 and line[0] == "[" and line[4] == "]":

				#If template exists
				if line[2] in child_nodes:
					insert = list(child_templates[child_nodes.index(line[2])])
					child_templates[child_nodes.index(line[2])] = []
					template = template[:i] + insert + template[i + 1:]
				else:
					template = template[:i] + template[i + 1:]

				break;
		else:
			break;

	#Insert the assembly for child nodes at the start
	for t in child_templates:
		prepend += t
	template = prepend + template

	if symbols.isScoped(parse_tree):
		CURRENT_SCOPE = CURRENT_SCOPE[:-1]
		symbols.updateScope(CURRENT_SCOPE)
		MAX_AT_LEVEL += 1

	return template



#Run the code generator on the entire parse tree
def generateCode(parse_tree, symbols_in):
	global symbols
	symbols = symbols_in

	return setUniqueRegisters(copy.deepcopy(templates["START"][0]), [], [])[0] + generateNode(parse_tree) + setUniqueRegisters(copy.deepcopy(templates["END"][0]), [], [])[0]