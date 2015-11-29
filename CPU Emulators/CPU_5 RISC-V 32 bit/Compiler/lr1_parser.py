import json

class ParserError(Exception):
	pass

SPECIAL_TOKENS = ["$", "INT", "STRING"]

working_stack = []
output_stack = []


#Read parse table
parse_table = {}
with open("parse_table.txt", "r") as table:
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

	return ["PROGRAM", output_stack]