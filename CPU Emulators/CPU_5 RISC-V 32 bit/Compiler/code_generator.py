from tokeniser import *

CURRENT_REGISTER = 0

definitions = []
templates = {}
symbols = []

with open("definitions.bnf", "r") as ifile:
	#Get lines for code generator
	relevant = []
	reading = False
	for line in ifile.readlines():
		if line.strip() == "#CODEGENERATOR":
			reading = True
			continue;
		elif line[0] == "#":
			reading = False
			continue;

		if reading:
			relevant.append(line)

	for line in relevant:
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


with open("templates.al", "r") as ifile:
	#Separate templates
	temp = []
	for line in ifile.readlines():
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


#Generate the assembly for a single node in the parse tree
def generateNode(parse_tree):
	global CURRENT_REGISTER

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
				template = templates[parse_tree[0]][definition[1]][:]
				break;
		else:
			template = templates[parse_tree[0]][0][:]

	#Substitute STRING and INT placeholders
	for i,line in enumerate(template):
		if "INT" in line:
			x = line.index("INT")
			template[i][x] = parse_tree[1][child_nodes.index("INT")][1][0]
		if "STRING" in line:
			x = line.index("STRING")
			template[i][x] = parse_tree[1][child_nodes.index("STRING")][1][0]

	#Assign registers used only by template
	assigned = {}
	for i,line in enumerate(template):
		if "$" in line:
			x = line.index("$")

			if not line[x + 1] in assigned:
				assigned[line[x + 1]] = str(CURRENT_REGISTER)
				CURRENT_REGISTER += 1

			template[i] = line[:x + 1] + assigned[line[x + 1]] +line[x + 2:]

	#Assign i/o registers
	assigned = [None] * len(child_nodes)
	for i,line in enumerate(template):
		if '@' in line:
			x = line.index('@')
			if line[x + 1] == '<' and line[x + 3] == '>':
				#Not assigned
				if assigned[child_nodes.index(line[x + 2])] == None:
					assigned[child_nodes.index(line[x + 2])] = CURRENT_REGISTER

					#Update child template
					for j,cline in enumerate(child_templates[child_nodes.index(line[x + 2])]):
						if '@' in cline:
							x = cline.index('@')
							child_templates[child_nodes.index(line[x + 2])][j] = cline[:x] + ["$", str(CURRENT_REGISTER)] + cline[x + 1:]

					CURRENT_REGISTER += 1
				#Update assembly
				template[i] = template[i][:x] + ["$", str(assigned[child_nodes.index(line[x + 2])])] + template[i][x + 4:]

	#Insert the assembly from the child nodes
	#Insert the assembly for child nodes in the middle
	for i,line in enumerate(template):
		if len(line) >= 3 and line[0] == "[" and line[2] == "]":
			insert = child_templates[child_nodes.index(line[1])][:]
			child_templates[child_nodes.index(line[1])] = []
			template = template[:i] + insert + templates[i + 1:]
	
	#Insert the assembly for child nodes at the start
	prepend = []
	for t in child_templates:
		prepend += t
	template = prepend + template

	return template



#Run the code generator on the entire parse tree
def generateCode(parse_tree, symbols_in):
	global symbols
	symbols = symbols_in

	return templates["START"][0][:] + generateNode(parse_tree) + templates["END"][0][:]