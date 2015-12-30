from assembler_data import *

#Split line into list of capitalised tokens
def tokenize(string):
	string = string.split('//')[0] #Remove comments

	#Strip unused punctuation
	filtered_string = ""
	for char in string:
		if not char in [',','.']: #Chars to remove
			filtered_string += char

	#Separate into tokens
	tokens = []
	mode = ""
	buf = ""
	for char in filtered_string:

		if char in [' ','\t','\n']:
			mode = ""
			tokens.append(buf)
			buf = ""
			continue;

		elif char.isalpha() or char in ['_','0','1','2','3','4','5','6','7','8','9', '$']:
			if mode=="" or mode=="a":
				buf += char
			else:
				tokens.append(buf)
				buf = char
			mode = "a"

		else:
			if mode=="" or mode=="p":
				buf += char
			else:
				tokens.append(buf)
				buf = char
			mode = "p"

	tokens.append(buf)


	#Remove blank tokens
	filtered_tokens = []
	for token in tokens:
		if not token == "":
			filtered_tokens.append(token)

	return filtered_tokens



#Read assembly from a .al file and tokenize
def readAssembly(filename):
	p("\n\nReading " + filename + "...")
	try:
		with open(filename, 'r') as ifile:

			assembly = []

			for line in ifile.readlines():
				nline = tokenize(line)
				if not nline==[]:
					if nline[0].upper() == "IMPORT":
						nline[0] = "IMPORT"
					else:
						for i,val in enumerate(nline):
							nline[i] = val.upper()
					assembly.append(nline)
					index = str(len(assembly) - 1)
					p("\t" + index + ":", e=" ")
					p(max(0, 4 - len(index)) * " ", e="")
					p(nline)

	except FileNotFoundError:
		raise AssemblerError("The file " + filename + " could not be read.")

	return assembly

#Link all of the necessary assembly files
def linkAssembly(assembly, filename):
	p("\n\nLinking " + filename + "...")

	#Get assembly for each dependant file
	dependancies = []
	for i,line in enumerate(assembly):
		if line[0] == "IMPORT":
			p("\t" + line[1] + ".al")
			to_import = readAssembly(line[1] + ".al")
			to_import = linkAssembly(to_import, line[1] + ".al")
			assembly[i][1] = str(len(dependancies))
			dependancies.append(to_import)

	initArbitraryRegisters(assembly)

	#Combine assembly
	new_assembly = []
	for line in assembly:
		if not line[0] == "IMPORT":
			new_assembly.append(line)
		else:
			new_assembly += setUniqueRegisters(dependancies[int(line[1])])

	p("Done linking " + filename + "!")

	return new_assembly

#Write assembly to a file
def writeAssembly(ofilename, assembly):
	p("\n\nWriting assembly to " + ofilename + "...")
	with open(ofilename, 'w') as ofile:
		for line in assembly:
			ofile.write(' '.join(line) + "\n")

#Write binary to .cpu file
def writeBinary(ofilename, binary):
	p("\n\nWriting binary to " + ofilename + "...")
	with open(ofilename, 'w') as ofile:
		for line in binary:
			bytes = [str(int(line[:8], 2)), str(int(line[8:16], 2)), str(int(line[16:24], 2)), str(int(line[24:], 2))]
			b = ' '.join(bytes)
			ofile.write(b + "\n")
			p("\t" + line + "  =>  " + b)