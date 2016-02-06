import copy, json, os
from tokenizer import *



tokenizer = Tokenizer()



#Container for a single BNF rule
class Rule(object):
	def __init__(self, lhs, rhs, num):
		self.lhs = lhs
		self.rhs = rhs
		self.num = num
		self.valid_lookaheads = []

	def __str__(self):
		return "Rule " +str(self.num) + ": " + self.lhs + " --> " + str(self.rhs) + "\nStarts with: " + str(self.valid_lookaheads)

rules = []
EBNF_EXPANSION = 0



#Turn BNF input into a list of rules with all definitions
def parseBNF(bnf):
	global rules
	global EBNF_EXPANSION

	cnt = 0

	for line in bnf:
		line = line.split("//")[0]
		parts = line.split("::=")

		if len(parts) == 2:
			lhs = parts[0].strip(" \t<>") #Separate out LHS
			rhs = parts[1].split(' | ')

			#Format RHS
			for i,format in enumerate(rhs):
				rhs[i] = format.split()
				for j,term in enumerate(rhs[i]):
					rhs[i][j] = term.strip()

			#Get all RHSs from EBNF
			while True:
				for definition in rhs:
					for i,term in enumerate(definition):

						if term[0] == '[' and term[-1] == ']':
							definition[i] = term[1:-1].strip()
							new_definition = copy.copy(definition)
							new_definition = new_definition[:i] + new_definition[i + 1:]
							rhs.append(new_definition)
							break;
							
						if term[0] == '{' and term[-1] == '}':
							nlhs = "ebnf_" + str(EBNF_EXPANSION)
							EBNF_EXPANSION += 1
							nrhs = [[term[1:-1]], ["<" + nlhs + ">", term[1:-1]]]
							rules.append(Rule(nlhs, nrhs, cnt))
							cnt += 1
							definition[i] = "<" + nlhs + ">"
					else:
						continue;
					break;
				else:
					break;

			rules.append(Rule(lhs, rhs, cnt))
			cnt+= 1

#Split literals as tokenizer will
def tokenizeRules():
	global rules

	for rule in rules:
		for i,definition in enumerate(rule.rhs):
			new_definition = []
			for term in definition:
				if term[0] == '"' and term[-1] == '"':
					tokens = tokenizer.tokenizeString(term[1:-1]) #Tokenize literal
					for token in tokens:
						new_definition.append('"' + token + '"')

				else:
					new_definition.append(term)
			rule.rhs[i] = new_definition #Update definition

#Replace reference to rules with their IDs
def linkRules():
	global rules
	for rule in rules:
		for definition in rule.rhs:
			for i,term in enumerate(definition):

				if term[0] == '<' and term[-1] == '>':
					name = term[1:-1].strip()

					for nrule in rules:
						if nrule.lhs == name:
							definition[i] = nrule.num
							break;

#Get any possible lookaheads for certain rules
def getLookAheads():
	global rules
	done = False
	while not done:
		done = True

		for rule in rules:
			if rule.valid_lookaheads == []:
				done = False #Some rules have not been done

				lookaheads = []
				for definition in rule.rhs:
					if isinstance(definition[0], int): #Does it point to another rule
						if definition[0] != rule.num:  #Is it a different rule
							if rules[definition[0]].valid_lookaheads != []:
								lookaheads += rules[definition[0]].valid_lookaheads
							else:
								break;
					else:
						lookaheads.append(definition[0])
				else:
					rule.valid_lookaheads = lookaheads #If all of the lookaheads are found add them



class Production(object):
	def __init__(self, lhs, rhs, pos, lookaheads):
		self.lhs = lhs
		self.rhs = rhs
		self.pos = pos
		self.lookaheads = lookaheads

	def __str__(self):
		string = self.lhs + " --> "
		for definition in self.rhs:
			string += "/ "
			for i,term in enumerate(definition):
				if i == self.pos:
					string += "* "
				string += str(term) + " "
			string += "\t"
			for l in self.lookaheads:
				string += str(l) + " / "
			string += "\n      "
		return string

	def __eq__(self, other):
		if self.lhs == other.lhs and self.rhs == other.rhs and self.pos == other.pos and self.lookaheads == other.lookaheads:
			return True
		else:
			return False



states = []

class State(object):
	def __init__(self, productions):
		self.productions = productions
		self.num = 0
		self.changes = {} #Dictionary containing transitions between states
		self.children = False

		self.getProductions()

	def __str__(self):
		string = "State " + str(self.num) + ":"
		for production in self.productions:
			string += "\n" + str(production)
		string += "\n"
		return string

	def __eq__(self, other):
		if len(self.productions) == len(other.productions):
			for production in self.productions:
				if not production in other.productions:
					break;
			else:
				return True
		return False

	#Get all possible productions that could occur
	def getProductions(self):
		done = False
		while not done:
			done = True

			for production in self.productions:
				for definition in production.rhs:
					if len(definition) > production.pos:
						if isinstance(definition[production.pos], int):
							ruleid = definition[production.pos]

							#Work out lookaheads
							lookaheads = []
							if len(definition) > production.pos + 1:
								if isinstance(definition[production.pos + 1], int):
									lookaheads = rules[definition[production.pos + 1]].valid_lookaheads
								else:
									lookaheads = [definition[production.pos + 1]]
							else:
								lookaheads = production.lookaheads

							#Test for whether production already exists
							nproduction = Production(rules[ruleid].lhs, rules[ruleid].rhs, 0, lookaheads)
							if not nproduction in self.productions:
								done = False
								self.productions.append(nproduction)
								break;
				else:
					continue;
				break;

	def generateChildren(self):

		#Create list of possible changes
		possibleChanges = []
		for production in self.productions:
			for definition in production.rhs:
				if len(definition) > production.pos:
					if not definition[production.pos] in possibleChanges:
						possibleChanges.append(definition[production.pos])

		#Get productions for each new state
		nstates = []
		for change in possibleChanges:
			root_productions = []
			for production in self.productions:
				nproduction = Production(production.lhs, [], production.pos + 1, production.lookaheads)
				for definition in production.rhs:
					if len(definition) > production.pos and definition[production.pos] == change:
						nproduction.rhs.append(definition)
				if nproduction.rhs != []:
					root_productions.append(nproduction)
			nstates.append(State(root_productions))

		for i,state in enumerate(nstates):
			if state in states:
				self.changes[possibleChanges[i]] = states.index(state)
			else:
				states.append(state)
				state.num = len(states) - 1
				self.changes[possibleChanges[i]] = len(states) - 1

		self.children = True

	def isLeaf(self):
		for production in self.productions:
			for definition in production.rhs:
				if len(definition) == production.pos:
					return True
		return False

def generateAllStates():
	global states
	done = False
	while not done:
		done = True

		for state in states:
			if not state.children:
				done = False
				state.generateChildren()
				break;

parse_table = {"reductions":[], "actions":{}, "goto":{}}

def generateParseTable():
	global rules
	global states
	global parse_table
	
	#Get all terminals and non-terminals
	terminals = ["$"]
	non_terminals = []
	for rule in rules:
		non_terminals.append([rule.lhs, rule.num])
		for definition in rule.rhs:
			for term in definition:
				if not isinstance(term, int) and not term in terminals:
					terminals.append(term)

	for state in states:
		#Populate goto table
		for non_terminal in non_terminals:
			if non_terminal[1] in state.changes:
				if non_terminal[0] in parse_table["goto"]:
					parse_table["goto"][non_terminal[0]].append(state.changes[non_terminal[1]])
				else:
					parse_table["goto"][non_terminal[0]] = [state.changes[non_terminal[1]]]
			else:
				if non_terminal[0] in parse_table["goto"]:
					parse_table["goto"][non_terminal[0]].append("")
				else:
					parse_table["goto"][non_terminal[0]] = [""]

		#Populate actions table with shifts
		for terminal in terminals:
			if terminal in state.changes:
				if terminal in parse_table["actions"]:
					parse_table["actions"][terminal].append("S" + str(state.changes[terminal]))
				else:
					parse_table["actions"][terminal] = ["S" + str(state.changes[terminal])]
			else:
				if terminal in parse_table["actions"]:
					parse_table["actions"][terminal].append("")
				else:
					parse_table["actions"][terminal] = [""]

		#Handle reductions on leaf states
		if state.isLeaf():
			for production in state.productions:
				for definition in production.rhs:
					if len(definition) == production.pos:
						#There is a reduction to be handled
						reduction = [production.lhs, len(definition)]
						index = len(parse_table["reductions"])
						if reduction in parse_table["reductions"]:
							index = parse_table["reductions"].index(reduction)
						else:
							parse_table["reductions"].append(reduction)
						for lookahead in production.lookaheads:
							parse_table["actions"][lookahead][-1] = "R" + str(index)

path = os.path.join(os.path.dirname(__file__), "Compiler/Configuration/grammar.bnf")
with open(path, "r") as grammar:
	#Set up rules
	print("\nReading grammar.bnf...")
	lines = grammar.readlines()
	print("Read!\n")
	print("Setting up rules...")
	parseBNF(lines)
	print("\tGenerated " + str(len(rules)) + " rules!")
	tokenizeRules()
	print("\tTokenized rules!")
	linkRules()
	print("\tLinked rules!")
	getLookAheads()
	print("\tFound all lookaheads!")
	print("Done!\n")

	#Create tree of states
	print("Creating states...")
	states.append(State([Production(rules[0].lhs, copy.deepcopy(rules[0].rhs), 0, ["$"])]))
	generateAllStates()
	print("\tGenerated " + str(len(states)) + " states!")
	print("Done!\n")

	#Generate parse table
	print("Generating parse table...")
	generateParseTable()
	print("Done!\n")

	print("Writing table to parse_table.txt...")
	path = os.path.join(os.path.dirname(__file__), "Compiler/parse_table.txt")
	with open(path, "w") as out:
		out.write(json.dumps(parse_table, separators = (",", ":")))
	print("Done!")