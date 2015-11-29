import sys

#Instruction codes and register info
from assembler_data import *
#Read/Write and tokenizer
from assembler_io import *
#Parse macros
from assembler_macros import *
#Final parse for jumps/globals
from assembler_finals import *
#Convert to binary
from assembler_binary import *



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
	p("Assembling: " + filename + " into " + ofilename)
	p("\n\nReading " + filename + "...")
	assembly = readAssembly(filename)
	
	if high:
		assembly = assembleHighLevel(assembly)

	if low:
		binary = assembleLowLevel(assembly)
		p("\n\nWriting to " + ofilename + "...")
		writeBinary(ofilename, binary)
	else:
		p("\n\nWriting to " + ofilename + "...")
		writeAssembly(ofilename, assembly)

	saveLog()



#Automatically assemble if main
if __name__ == "__main__":
	args = sys.argv

	if len(args) == 1:
		args = [None] + input("<input filename> <output filename> -flags\n\n-H   - Treat input as high-level assembly\n-L   - Output binary\n\n>>> ").split()

	#Switches
	if "-H" in args:
		high = True
	else:
		high = False

	if "-L" in args:
		low = True
	else:
		low = False

	#Assemble
	assemble(args[1], args[2], high, low)