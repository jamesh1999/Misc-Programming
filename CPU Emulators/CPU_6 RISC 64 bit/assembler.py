import sys

#Instruction codes and register info
from Assembler.assembler_data import *
#Read/Write and tokenizer
from Assembler.assembler_io import *
#Parse macros
from Assembler.assembler_macros import *
#Final parse for jumps/globals
from Assembler.assembler_finals import *
#Convert to binary
from Assembler.assembler_binary import *



#Convert to low level assembly
def assembleHighLevel(assembly):
	p("\n\nConverting high-level to low-level assembly:\nExpanding macros...")
	assembly = macrosAssembly(assembly)
	p("\n\nFinal pass...")
	assembly = finalPassAssembly(assembly)
	return assembly

#Convert low-level assembly to binary
def assembleLowLevel(assembly):
	p("\n\nPerforming low-level assembly:\nConverting to binary...")
	binary = convertAssembly(assembly)
	return binary

#Main function
def assemble(filename, ofilename, doHighLevel, doLowLevel):
	try:
		p("Assembling: " + filename + " into " + ofilename)
		assembly = readAssembly(filename)
		assembly = linkAssembly(assembly, filename)
		
		if doHighLevel:
			assembly = assembleHighLevel(assembly)

		if doLowLevel:
			binary = assembleLowLevel(assembly)
			writeBinary(ofilename, binary)
		else:
			writeAssembly(ofilename, assembly)

		print("Writing to log...")
		saveLog()
		print("Done!")

	#For AssemblerErrors just print message
	except AssemblerError as e:
		print(e)
	#Print stack trace for unhandled errors
	except:
		print("\n\n\n\nWhat did you do?!?!?\nUnhandled error in assembler: \n\n")
		raise



#Automatically assemble if main
if __name__ == "__main__":
	args = sys.argv

	if len(args) == 1:
		args = [None] + input("Usage: <input filename> [flags]\n\n-O<output filename>   - Specify an alternative output filename\n-H   - Treat input as high-level assembly\n-L   - Output low level assembly\n\n>>> ").split()

	#Switches
	ofilename = args[1][:args[1].index('.')] + ".cpu"
	for arg in args[2:]:
		if arg[:2] == "-O":
			ofilename = arg[2:]
			break;

	if "-H" in args[2:]:
		high = True
	else:
		high = False

	if "-L" in args[2:]:
		low = False
	else:
		low = True

	p("Running: assembler.py " + ' '.join(args[1:]))

	#Assemble
	assemble(args[1], ofilename, high, low)