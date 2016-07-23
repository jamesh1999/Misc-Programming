import os, copy, yaml
from tokenizer import *


CURRENT_REGISTER = 0
CURRENT_NODE = []
MAX_AT_LEVEL = 0

templates = {}
python_snippets = []
snippet_globals = {}
snippet_locals = {}

symbols = []

tokenizer = Tokenizer()

#Read templates
path = os.path.join(os.path.dirname(__file__), "Configuration/templates.yml")
with open(path, "r") as ifile:

	#Parse YAML
	tree = yaml.load(ifile)

	#Run snippet initialisation code
	code_obj = compile(tree["INIT"], "init", "exec")
	exec(code_obj, snippet_locals, snippet_globals)

	#Get data from YAML
	for node in tree["TEMPLATES"]:
		name, data = list(node.items())[0]

		definitions = []
		for definition in data:

			if "template" in definition:
				template = definition["template"]
			else:
				template = ""

			#Compile all python snippets
			finished = False
			while not finished:
				finished = True

				for i in range(len(template) - 1):
					if template[i:i + 2] == "{{":
						section = template[i + 2:]
						section = section[:section.index("}}")]

						#Skip if symbol query
						if len(section.split(':')) == 2 and section.split(':')[0].isalnum() and section.split(':')[1].isalnum():
							continue;
						#Skip if child node
						if section[0] == '<' and section[-1] == '>':
							continue;
						#Skip if already compiled
						if section.isdigit():
							continue;

						finished = False

						#Compile
						code_obj = compile(section, "snippets", "exec")
						template = template[:i + 2] + str(len(python_snippets)) + template[i + len(section) + 2:]
						python_snippets.append(code_obj)

			if "default" in definition:
				default = definition["default"]
			else:
				default = False

			d = []
			if "definition" in definition:
				parts = definition["definition"].split()

				for part in parts:
					part = part.strip("<> \t\n")

					if len(part) > 1 and part[0] == '"' and part[-1] == '"':
						tokens = tokenizer.tokenizeString(part[1:-1])
						for token in tokens:
							d.append('"' + token + '"')
					else:
						d.append(part)

			if "conditions" in definition:
				#Compile conditions
				conditions = compile(definition["conditions"], "conditions", "eval")
			else:
				conditions = compile("True", "conditions", "eval")

			if "output" in definition:
				#Compile output
				output = compile(definition["output"], "output", "eval")
			else:
				output = compile("None", "output", "eval")

			definitions.append([default, d, conditions, template, output])

		templates[name] = definitions
				


#Check if the tree is equivalent to the definition
def isDefinitionMatch(tree, definition, pos = 0, root = True):
	#Does definition match
	for node in tree:

		#Check that string matches
		if isinstance(node, str):
			if len(definition[pos]) > 2 and definition[pos][1:-1] == node:
				pos += 1
			else:
				break;

		#Check that node matches
		elif definition[pos] == node.getType():
			pos += 1
		else:
			b, pos = isDefinitionMatch(node, definition, pos = pos, root = False)
			if not b:
				break;
	else:
		#Matches (so far)
		if root:
			return True if pos == len(definition) else False #Return true if at end of definition
		else:
			return True, pos

	#Doesn't match
	if root:
		return False
	else:
		return False, 0

#Assigns a template to the node and its children
def setTemplate(tree):
	#Set children
	for child in tree:
		if not isinstance(child, str):
			setTemplate(child)

	#Get the correct template
	template = ""
	out = None
	snippet_locals["NODE"] = tree
	if tree.getType() in templates:
		for definition in templates[tree.getType()]:
			if definition[1] != [] and isDefinitionMatch(tree, definition[1]):
				if eval(definition[2], snippet_locals, snippet_globals):
					template = definition[3]
					out = definition[4]
					break;
		else:
			for definition in templates[tree.getType()]:
				if definition[0]:
					template = definition[3]
					out = definition[4]
					break;
	tree.template = template

	#Set output type
	if template != "":
		tree.output_type = eval(out, snippet_locals, snippet_globals)

	#Pass through child output if none
	else:
		for node in tree:
			if not isinstance(node, str) and node.output_type != None:
				tree.output_type = node.output_type
				break;
		else:
			tree.output_type = None

#Assign output registers to all nodes
def assignOutputRegisters(node):
	global CURRENT_REGISTER
	node.output = CURRENT_REGISTER
	CURRENT_REGISTER += 1

	for child in node:
		if not isinstance(child, str):
			assignOutputRegisters(child)

#Set all arbitrary registers to unique values
def setInternalRegisters(template):
	global CURRENT_REGISTER

	#Assign registers used only by template
	assigned = {}
	section = 0
	while '$' in template[section:]:
		i = section + template[section:].index('$')

		val = ""
		index = i + 1
		while index < len(template) and template[index].isdigit():
			val += template[index]
			index += 1

		if not val in assigned:
			assigned[val] = '$' + str(CURRENT_REGISTER)
			CURRENT_REGISTER += 1

		template = template[:i] + assigned[val] + template[index:]

		section = index

	return template

#Set IO registers to their unique values
def setIORegisters(template, parse_tree):

	#Assign i/o registers
	while '@' in template:
		i = template.index('@')

		#Output from child
		if i + 1 < len(template) and template[i + 1] == '<':
			section = template[i + 2:]
			name = section[:section.index('>')]

			for node in parse_tree:
				if isinstance(node, str):
					continue;
				if node.getType() == name:
					template = template[:i] + '$' + str(node.output) + section[len(name) + 1:]

		#Output from this node
		else:
			template = template[:i] + '$' + str(parse_tree.output) + template[i + 1:]

	return template

current_anchors = []
num_anchors = 0
#Assign unique values to jump anchors
def assignUniqueAnchors(template):
	global current_anchors
	global num_anchors

	current_anchors.append([list(CURRENT_NODE), {}])

	#Define anchors in template
	for line in template.split('\n'):
		if len(line.strip()) and line.strip()[0] == ':':

			name = ""
			index = line.index(':') + 1
			while index < len(line) and (line[index].isalnum() or line[index] == '_'):
				name += line[index]
				index += 1

			current_anchors[-1][1][name] = "ANCHOR_" + str(num_anchors)
			num_anchors += 1

#Replace jump anchors with unique values
def replaceUniqueAnchors(template):
	global current_anchors

	start = 0
	finished = False
	while not finished:
		finished = True

		#Substitute anchors
		for i in range(start, len(template)):
			if template[i] == ":":

				name = ""
				index = i + 1
				while (template[index].isalnum() or template[index] == '_'):
					name += template[index]
					index += 1

				#Get relevant anchor
				for x in range(len(CURRENT_NODE)):
					if x == 0:
						test_scope = list(CURRENT_NODE)
					else:
						test_scope = CURRENT_NODE[:-x]

					for y in current_anchors:
						if y[0] == test_scope:
							break;
					else:
						continue;

					if name in y[1]:
						name = y[1][name]
						break;

				#Substitute and update working section
				template = template[:i + 1] + name + template[index:]
				start = index
				finished = False
				break;

	return template

#Generate the assembly for a single node in the parse tree from the template
def generateNode(parse_tree):
	global CURRENT_NODE
	global MAX_AT_LEVEL
	global python_snippets
	global snippet_globals
	global snippet_locals

	symbols.updateScope(parse_tree)

	template = parse_tree.template
	print(template)
	assignUniqueAnchors(template)

	#Get nodes
	child_nodes = []
	child_templates = []
	for node in parse_tree:
		if not isinstance(node, str):
			child_nodes.append(node.getType())

	child_templates = [[]] * len(child_nodes)

	index = 0
	#Generate all child nodes
	for i,node in enumerate(parse_tree):
		if isinstance(node, str):
			continue;

		CURRENT_NODE.append(i)
		child_templates[index] = generateNode(node)
		CURRENT_NODE.pop()
		index += 1

	#Pass first child output through if no template
	if template == "":
		for i, node in enumerate(child_nodes):
			tree_node = None
			for child in parse_tree:
				if isinstance(child, str):
					continue;
				if child.getType() == node:
					tree_node = child

			output = "$" + str(tree_node.output)
			if output in child_templates[i]:
				template = "Mov @ @<" + tree_node.getType() + ">\n"
				break;

	#Substitute STRING and INT placeholders
	while "INT" in template:
		x = template.index("INT")
		template = template[:x] + parse_tree.getFirstOf("INT").getNode(0) + template[x + 3:]
	while "STRING" in template:
		x = template.index("STRING")
		template = template[:x] + parse_tree.getFirstOf("STRING").getNode(0) + template[x + 6:]

	#Add child template insertion marks
	for node in reversed(child_nodes):
		if not "{{<" + node + ">}}" in template:
			template = "{{<" + node + ">}}\n" + template

	#Insert pre-frame-change assembly
	if symbols.isScoped(parse_tree):
		template = templates["SCOPE_START"][0][3] + template

	#Insert post-frame-change assembly
	if symbols.isScoped(parse_tree):
		template += templates["SCOPE_END"][0][3]

	template = setInternalRegisters(template)

	#Handle embedded code/symbol queries/child nodes
	while "{{" in template:
		section = template[template.index("{{") + 2:template.index("}}")]

		#Child node
		if section[0] == '<' and section[-1] == '>':
			if section[1:-1] in child_nodes:
				insert = child_templates[child_nodes.index(section[1:-1])]
			else:
				insert = ""
			template = template[:template.index("{{")] + insert + template[template.index("}}") + 2:]

		#Symbol query
		elif len(section.split(':')) == 2 and section.split(':')[0].isalnum() and section.split(':')[1].isalnum():
			splitted = section.split(':')
			template = template[:template.index("{{")] + symbols.query(splitted) + template[template.index("}}") + 2:]

		#Embedded code
		else:
			snippet_locals["NODE"] = parse_tree
			snippet_locals["RET"] = ""
			exec(python_snippets[int(section)], snippet_globals, snippet_locals)
			template = template[:template.index("{{")] + snippet_locals["RET"] + template[template.index("}}") + 2:]

	template = replaceUniqueAnchors(template)
	template = setIORegisters(template, parse_tree)
	symbols.endUpdateScope(parse_tree)

	return template



#Run the code generator on the entire parse tree
def generateCode(parse_tree, symbols_in):
	global symbols
	symbols = symbols_in
	snippet_locals["SYMBOLS"] = symbols
	snippet_locals["PARSE_TREE"] = parse_tree

	assignOutputRegisters(parse_tree)
	setTemplate(parse_tree)
	assembly = templates["SETUP"][0][3] + generateNode(parse_tree) + templates["FINISH"][0][3]

	n_assembly = ""
	for line in assembly.split('\n'):
		if line != "":
			n_assembly += line + '\n'

	return n_assembly