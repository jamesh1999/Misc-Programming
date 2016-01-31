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
entire_tree = []



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
	#Search children
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

				line[j] = assigned[token[1:]]
		template[i] = line

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

#Handle all symbol queries on a line
def handleSymbolQueries(line):
	while "?" in line:
		segment = line[line.index("?"):]
		query = segment[:segment.index(")") + 1]
		result = symbols.symbolQuery(query)
		if result == None:
			line = line[:line.index("?")] + segment[len(query):]
			continue;

		if isinstance(result, str):
			result = [result]

		line = line[:line.index("?")] + result + segment[len(query):]

	return line

#Generate the assembly for a single node in the parse tree
current_node = []
def generateNode(parse_tree):
	global CURRENT_SCOPE
	global MAX_AT_LEVEL
	global current_node

	if symbols.isScoped(parse_tree):
		CURRENT_SCOPE.append(MAX_AT_LEVEL)
		symbols.updateScope(CURRENT_SCOPE)

	#Get nodes
	child_nodes = []
	child_templates = []
	for node in parse_tree[1]:
		if not isinstance(node, str):
			child_nodes.append(node[0])
	child_templates = [[]] * len(child_nodes)

	#Get the correct template
	template = []
	if parse_tree[0] in templates:
		for definition in definitions:
			if isDefinitionMatch(parse_tree, definition):
				template = copy.deepcopy(templates[parse_tree[0]][definition[1]])
				break;
		else:
			template = copy.deepcopy(templates[parse_tree[0]][0])

	#Add child template insertion marks
	for node in reversed(child_nodes):
		if not ["[", "<", node, ">", "]"] in template:
			template = [["[", "<", node, ">", "]"]] + template

	#Insert pre-frame-change assembly
	if symbols.isScoped(parse_tree):
		template = copy.deepcopy(templates["SCOPE_START"][0]) + template

	#Insert post-frame-change assembly
	if symbols.isScoped(parse_tree):
		template += copy.deepcopy(templates["SCOPE_END"][0])

	#Generate template line by line
	for i, line in enumerate(template):
		#Substitute STRING and INT placeholders
		while "INT" in line:
			x = line.index("INT")
			template[i][x] = getFirstInstanceOf(parse_tree, "INT")[1][0]
		while "STRING" in line:
			x = line.index("STRING")
			template[i][x] = getFirstInstanceOf(parse_tree, "STRING")[1][0]

		#Set custom symbols
		if line[0] == "?" and line[3] == "=":
			template[i] = handleSymbolQueries(line)

		#Substitute custom symbols
		template[i] = symbols.replaceSymbols(line)

		#Handle any other symbol queries
		template[i] = handleSymbolQueries(line)

		#Generate child nodes
		if len(line) == 5 and line[0] == "[" and line[4] == "]":

			#Find respective node
			for j, node in enumerate(parse_tree[1]):
				if not isinstance(node, str) and node[0] == line[2]:
					break;
			else:
				#Node not present: move to next line
				template[i] = []
				continue;

			#Generate node & change scope
			current_node.append(j)
			child_templates[child_nodes.index(line[2])] = generateNode(node)
			current_node.pop()

	corrected = setUniqueRegisters(template, child_nodes, child_templates)
	template = corrected[0]
	child_templates = corrected[1]

	#Remove empty lines
	while [] in template:
		template.remove([])

	#Insert the assembly for child nodes in the middle
	while True:
		for i,line in enumerate(template):
			if len(line) >= 3 and line[0] == "[" and line[4] == "]":

				insert = list(child_templates[child_nodes.index(line[2])])
				child_templates[child_nodes.index(line[2])] = []
				template = template[:i] + insert + template[i + 1:]

				break;
		else:
			break;

	if symbols.isScoped(parse_tree):
		CURRENT_SCOPE = CURRENT_SCOPE[:-1]
		symbols.updateScope(CURRENT_SCOPE)
		MAX_AT_LEVEL += 1

	return template



#Run the code generator on the entire parse tree
def generateCode(parse_tree, symbols_in):
	global symbols
	global entire_tree

	#Add START and END nodes
	parse_tree[1] = [["START", []]] + parse_tree[1] + [["END", []]]

	symbols = symbols_in
	entire_tree = parse_tree

	return copy.deepcopy(templates["SETUP"][0]) + generateNode(parse_tree) + copy.deepcopy(templates["FINISH"][0])