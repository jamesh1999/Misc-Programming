import sys,time

from Compiler.preprocessor import *
from tokenizer import *
from Compiler.lr1_parser import *
from Compiler.symbol_generator import *
from Compiler.code_generator import *
import assembler
import optimizer



COMPILER_LOG = ""

def printLog(text, e = "\n"):
	global COMPILER_LOG
	print(text, end = e)
	COMPILER_LOG += str(text) + e

def saveLogFile():
	path = os.path.join(os.path.dirname(__file__), "Compiler/log.txt")
	with open(path, "w") as log_file:
		log_file.write(COMPILER_LOG)



def compile_file(ifilename, ofilename):
	start = time.clock()

	printLog("Compiling " + ifilename + " into " + ofilename + "...")

	#Preprocess file & Handle includes/strings
	printLog("Preprocessing...")
	preprocessed = preprocess(ifilename)
	printLog("Done\n")

	#Split into tokens
	printLog("Tokenizing...")
	tokenizer = Tokenizer()
	tokens = tokenizer.tokenizeString('\n'.join(preprocessed))

	printLog("Token stream:")
	printLog(tokens)
	printLog("\n")

	#Create parse tree
	printLog("Parsing...")
	tree = parse(tokens)

	printLog("Generated parse tree:")
	printLog(tree)
	printLog("\n")

	#Generate symbol table
	printLog("Locating symbols...")
	symbols = generateSymbolTable(tree)

	printLog("Symbol table:")
	printLog(symbols)
	printLog("\n")

	#Generate code
	printLog("Generating code...")
	assembly = generateCode(tree, symbols)

	printLog("Constructed high level assembly:")
	for i,line in enumerate(assembly.split('\n')):
		printLog(str(i) + max(5 - len(str(i)), 0) * " " + ": " + line)

	#Save to output file
	root_name = ifilename.split('.')[0]
	printLog("\nSaving to " + root_name + ".al...")
	with open(root_name + ".al", 'w') as outputf:
		outputf.write(assembly)
	printLog("Saved")

	#Save log
	print("Saving log...")
	saveLogFile()
	print("Done!")

	#Optimize code
	print("Switching to optimizer...")
	optimizer.optimize(root_name + ".al", optimizer.OPTIMIZATION_FULL, optimizer.PRIORITY_BALANCED)

	#Run assembler
	print("Switching to assembler...")
	assembler.assemble(root_name + ".al", ofilename, True, True)
	print("Done assembling!")
	print("Finished compiling in " + str(time.clock() - start) + "s")
		


#Automatically compile if main
if __name__ == "__main__":
	args = sys.argv

	if len(args) == 1:
		args = [None] + input("<input filename> [<output filename>]\n\n>>> ").split()

	if len(args) == 2:
		args.append(args[1].split('.')[0] + ".cpu")

	#Compile
	compile_file(args[1], args[2])