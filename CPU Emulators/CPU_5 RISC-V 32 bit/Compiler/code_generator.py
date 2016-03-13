import os
from tokenizer import *
import copy



CURRENT_REGISTER = 0
CURRENT_SCOPE = []
MAX_AT_LEVEL = 0

definitions = []
templates = {}
built_in = {}

symbols = []

tokenizer = Tokenizer()



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
					tokens = tokenizer.tokenizeString(npart[1:-1])
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
		tokens = tokenizer.tokenizeString(line)
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



#Check if the tree is equivalent to the definition
def isDefinitionMatch(tree, definition, pos = 0, root = True):
	#Does definition apply to this node
	if root and definition[0] != tree.getType():
		return False

	#Does definition match
	check = definition[2]
	for node in tree:

		#Check that string matches
		if isinstance(node, str):
			if len(check[pos]) > 2 and check[pos][1:-1] == node:
				pos += 1
			else:
				break;

		#Check that node matches
		elif check[pos] == node.getType():
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

current_anchors = []
num_anchors = 0
#Set jump anchors to unique values
def setUniqueAnchors(template):
	global current_anchors
	global num_anchors

	current_anchors.append([list(current_node), {}])

	#Define anchors in template
	for line in template:
		if line[0] == ":":
			current_anchors[-1][1][line[1]] = "ANCHOR_" + str(num_anchors)
			num_anchors += 1

	#Substitute anchors
	for i, line in enumerate(template):
		for j, token in enumerate(line):
			if token == ":":

				#Get relevant anchor
				for x in range(len(current_node)):
					if x == 0:
						test_scope = list(current_node)
					else:
						test_scope = current_node[:-x]

					for y in current_anchors:
						if y[0] == test_scope:
							break;
					else:
						continue;

					if line[j + 1] in y[1]:
						template[i][j + 1] = y[1][line[j + 1]]
						break;

	return template

#Generate the assembly for a single node in the parse tree
current_node = []
def generateNode(parse_tree):
	global CURRENT_SCOPE
	global MAX_AT_LEVEL
	global current_node

	symbols.updateScope(parse_tree)

	#Get nodes
	child_nodes = []
	child_templates = []
	for node in parse_tree:
		if not isinstance(node, str):
			child_nodes.append(node.getType())
	child_templates = [[]] * len(child_nodes)

	#Get the correct template
	template = []
	if parse_tree.getType() in templates:
		for definition in definitions:
			if isDefinitionMatch(parse_tree, definition):
				template = copy.deepcopy(templates[parse_tree.getType()][definition[1]])
				break;
		else:
			template = copy.deepcopy(templates[parse_tree.getType()][0])

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

	template = setUniqueAnchors(template)

	#Generate template line by line
	for i, line in enumerate(template):
		#Substitute STRING and INT placeholders
		while "INT" in line:
			x = line.index("INT")
			template[i][x] = parse_tree.getFirstOf("INT").getNode(0)
		while "STRING" in line:
			x = line.index("STRING")
			template[i][x] = parse_tree.getFirstOf("STRING").getNode(0)

		#Set custom symbols
		if line[0] == "?" and line[3] in ["=", "+", "-"]:
			handleSymbolQueries(line)
			template[i] = []
			continue;

		#Substitute custom symbols
		template[i] = symbols.replaceSymbols(line)

		#Handle any other symbol queries
		template[i] = handleSymbolQueries(line)

		#Generate child nodes
		if len(line) == 5 and line[0] == "[" and line[4] == "]":

			#Find respective node
			for j, node in enumerate(parse_tree):
				if not isinstance(node, str) and node.getType() == line[2]:
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

	symbols.endUpdateScope(parse_tree)

	return template



#Run the code generator on the entire parse tree
def generateCode(parse_tree, symbols_in):
	global symbols
	symbols = symbols_in

	return copy.deepcopy(templates["SETUP"][0]) + generateNode(parse_tree) + copy.deepcopy(templates["FINISH"][0])