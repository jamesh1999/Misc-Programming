from Optimizer.base import *

#Remove all unnecessary MOV instructions
def redundantMov(assembly):

	replacements = {}
	section = 0
	assembly_lines = assembly.split('\n')
	cnt = 0
	while True:

		for i, line in enumerate(assembly_lines[section:]):

			#Perform replacements
			tokens = tokenizer.tokenizeString(line)
			for j in range(len(tokens)):
				while tokens[j] in replacements:
					tokens[j] = replacements[tokens[j]]

			#Update assembly with replaced instruction
			assembly_lines[section + i] = ' '.join(tokens)

			if getOp(tokens) == "MOV":

				#Get registers used
				splitted = tokens[1:3]

				#Skip if not arbitrary registers
				if splitted[0][0] != '$' or splitted[1][0] != '$':
					continue;
				
				#Check if source is used again
				assembly_after = '\n'.join(assembly_lines[section + i + 1:])
				if splitted[1] in tokenizer.tokenizeString(assembly_after):
					continue;

				#Add replacement
				replacements[splitted[0]] = splitted[1]

				assembly_lines = assembly_lines[:section + i] + assembly_lines[section + i + 1:]
				section += i
				cnt += 1
				break;

		else:
			break;

	printLog("\tRemoved " + str(cnt) + " MOV instructions")

	return '\n'.join(assembly_lines)

#Removes instructions whose return values are never used
def redundantInstruction(assembly):
	assembly_lines = assembly.split('\n')

	cnt = 0
	for i, line in enumerate(assembly_lines):
		tokens = tokenizer.tokenizeString(line)
		dest = getDest(tokens)
		if dest == None or dest[0] != '$': 
			continue;

		#Check if destination in following assembly
		assembly_after = '\n'.join(assembly_lines[i + 1:])
		if not dest in tokenizer.tokenizeString(assembly_after):

			#Clear line
			assembly_lines[i] = ""
			cnt += 1


	#Remove empty lines (also serves to clean lines that used to contain comments)
	while "" in assembly_lines:
		assembly_lines.remove("")

	printLog("\tRemoved " + str(cnt) + " instructions")

	return '\n'.join(assembly_lines)



def basic(assembly):
	printLog("Starting basic optimizations...")

	printLog("Removing redundant MOV instructions...")
	assembly = redundantMov(assembly)
	printLog("Done\n")

	printLog("Removing unused instructions...")
	assembly = redundantInstruction(assembly)
	printLog("Done\n")

	printLog("Finished\n")

	return assembly