import json, os



#Errors for parser and ParseTree class
class ParserError(Exception):
	pass
class ParseTreeError(Exception):
	pass



SPECIAL_TOKENS = ["$", "INT", "STRING"]

working_stack = []
output_stack = []



#Iterator for ParseTree()
class TreeIterator(object):
	def __init__(self, tree):
		self.parse_tree = tree
		self.__current = -1

	def __next__(self):
		self.__current += 1

		if self.__current < len(self.parse_tree):
			return self.parse_tree.getNode(self.__current)

		raise StopIteration



#Container for parse tree
class ParseTree(object):
	def __init__(self, tree):
		self.__parse_tree = tree

		#Convert nodes to ParseTree()s
		for i, node in enumerate(self.__parse_tree[1]):
			if isinstance(node, str):
				continue;

			self.__parse_tree[1][i] = ParseTree(node)

		self.__buff = "" #Buffer for getTerminals()

	#Returns the child node with a given type
	def getNodeByType(self, t):
		for node in self.__parse_tree[1]:
			if node.getType() == t:
				return node

	#Get a node by index
	def getNode(self, index):
		if index >= len(self.__parse_tree[1]):
			raise ParseTreeError("Parse tree index is out of range")

		#Return string on terminal
		if isinstance(self.__parse_tree[1][index], str):
			return self.__parse_tree[1][index]

		return self.__parse_tree[1][index]

	#Get the type of the root node
	def getType(self):
		return self.__parse_tree[0]

	#Gets a string representing all terminals in this node
	def getTerminals(self):
		self.__buff = ""
		self.__getTerminalsRecursive(self)
		return self.__buff

	def __getTerminalsRecursive(self, node):
		for part in node:
			#If terminal add to buffer
			if isinstance(part, str):
				self.__buff += part
			else:
				self.__getTerminalsRecursive(part)

	#Gets first node of type
	def getFirstOf(self, search):
		return self.__getFirstOfRecursive(self, search)

	def __getFirstOfRecursive(self, tree, search):
		#Search children
		for node in tree:
			#Skip string literals
			if isinstance(node, str):
				continue;

			if node.getType() == search:
				return node

			#See if desired node is in this node
			x = self.__getFirstOfRecursive(node, search)
			if not x == None:
				return x

		#Node could not be found
		return None

	#Returns the number of child nodes
	def __len__(self):
		return len(self.__parse_tree[1])

	#Print the tree
	def __str__(self):
		return self.__strRecursive(self)

	def __strRecursive(self, node, indent = "", skipped = False):
		#List all child nodes
		if indent == "" or (not len(node) == 1 and not isinstance(node, str)):
			ret = indent
			if skipped:
				ret += "..."
			ret += "- " + node.getType() + "\n"

			#Calculate indent for branch
			nindent = indent
			if skipped:
				nindent += "   "
			nindent += "|"

			#Print branch
			for i in node:
				if not isinstance(i, str):
					ret += self.__strRecursive(i, indent = nindent)

			return ret

		#Skip long chains of 1 child nodes
		elif len(node) == 1 and not isinstance(node.getNode(0), str):
			return self.__strRecursive(node.getNode(0), indent = indent, skipped = True)

		#Print leaf nodes
		else:
			ret = indent + " "

			if skipped:
				ret += "... "

			ret += node.getType() + "\n"
			return ret

	#Make tree iterable
	def __iter__(self):
		return TreeIterator(self)

	#Allow len(ParseTree())
	def __len__(self):
		return len(self.__parse_tree[1])


#Read parse table
parse_table = {}
path = os.path.join(os.path.dirname(__file__), "parse_table.txt")
with open(path, "r") as table:
	parse_table = json.loads(table.readline())

#Returns the current parser state
def getState():
	for token in reversed(working_stack):
		if isinstance(token, int):
			return token
	raise ParserError("Stack is empty!")

#Generates a branch after a reduction
def formBranch(token, num):
	global output_stack

	components = output_stack[-num:]
	output_stack = output_stack[:-num]
	output_stack.append([token, components])

to_replace = ""
#Parse the next token
def parseStep(ntoken):
	global working_stack
	global output_stack
	global to_replace

	#Check for end
	if not ntoken == "PROGRAM":

		#Terminals query actions
		if (ntoken[0] == '"' and ntoken[-1] == '"') or ntoken in SPECIAL_TOKENS:
			#Replace generic tokens
			if not ntoken in parse_table["actions"]:
				if ntoken[1:-1].isdigit():
					to_replace = ntoken
					ntoken = "INT"
				else:
					to_replace = ntoken
					ntoken = "STRING"

			#Get action
			action = parse_table["actions"][ntoken][getState()]
			if not action == "":
				if action[0] == "S": #Shift
					#Strip "s
					if ntoken != "$" and ntoken in SPECIAL_TOKENS:
						ntoken = [ntoken, [to_replace[1:-1]]]
					else:
						ntoken = ntoken[1:-1]

					#Add to stack
					working_stack.append(ntoken)
					output_stack.append(ntoken)
					working_stack.append(int(action[1:]))

				elif action[0] == "R": #Reduction

					working_stack = working_stack[:-2 * parse_table["reductions"][int(action[1:])][1]] #Remove tokens
					working_stack.append(parse_table["reductions"][int(action[1:])][0])
					#Group component tokens
					formBranch(parse_table["reductions"][int(action[1:])][0], parse_table["reductions"][int(action[1:])][1])

					parseStep(working_stack[-1])
					parseStep(ntoken) #parseStep on token that was lookahead
			elif not ntoken == "$": #Ignore errors for EOS
				print(working_stack)
				raise ParserError("Unexpected token: " + ntoken)

		#Non-terminals query goto
		else:
			goto = parse_table["goto"][ntoken][getState()]
			if not goto == "":
				working_stack.append(goto)
			else:
				raise ParserError("Unexpected grammar construct: " + ntoken)

#Main parser loop
def parse(tokens):
	global working_stack
	global output_stack

	working_stack = [0]
	output_stack = []

	for token in tokens:
		parseStep('"' + token + '"')
	parseStep("$") #Add EOS token

	#Return tree with special nodes added
	return ParseTree(["PROGRAM", [["START", []]] + output_stack + [["END", []]]])