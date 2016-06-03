from Assembler.assembler_data import *
from tokenizer import *



tokenizer = Tokenizer()
tokenizer.configure(combine = True, remove = [' ', '\t', '\n', ',', '(', ')', '#'], split = [" ", "\t", "\n","{", "}", "(", ")", "[", "]", "<", ">", ",", ".", "/", ";", "~", "!", "?", "'", "%", "^", "&", "*", "-", "+", "=", "|", ":", "@", '#'])



#Read assembly from a .al file and tokenize
def readAssembly(filename):
	p("\n\nReading " + filename + "...")
	#Nested try excepts to check both directories for file
	try:
		ifile = open(filename, 'r')
	except IOError:
		try:
			path = os.path.join(os.path.dirname(__file__), "../Assembly Code/" + filename)
			ifile = open(path, 'r')
		except IOError:
			raise AssemblerError("The file " + filename + " could not be read.")

	assembly = []

	for line in ifile.readlines():
		nline = tokenizer.tokenizeString(line)

		if not nline == []:
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

	ifile.close()

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
			p("\t" + line + "  =>  ", e="")
			for val in bytes:
				p(' ' * (4 - len(val)) + val, e="")
			p("")