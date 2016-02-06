import sys

from tokenizer import *
from Compiler.lr1_parser import *
from Compiler.symbol_generator import *
from Compiler.code_generator import *
from assembler import *


def compile_file(ifilename, ofilename):
	with open(ifilename) as inputf:

		tokenizer = Tokenizer()
		tokens = tokenizer.tokenizeFile(inputf)

		print(tokens)
		print("\n\n\n")

		tree = parse(tokens)

		print(tree)
		print("\n\n\n")

		symbols = generateSymbolTable(tree)

		print(symbols)
		print("\n\n\n")

		assembly_list = generateCode(tree, symbols)

		#Stringify assembly
		assembly = ""
		for line in assembly_list:
			string = ""
			#Concatenate punctuation tokens
			for token in line:
				if len(string) and len(token) == 1 and not token.isalnum() and not string[-1].isalnum():
					string += token
				else:
					string += " " + token
			assembly += string[1:] + "\n"
		print(assembly)

		root_name = ifilename.split('.')[0]
		with open(root_name + ".al", 'w') as outputf:
			outputf.write(assembly)

		assemble(root_name + ".al", ofilename, True, True)
		


#Automatically compile if main
if __name__ == "__main__":
	args = sys.argv

	if len(args) == 1:
		args = [None] + input("<input filename> [<output filename>]\n\n>>> ").split()

	if len(args) == 2:
		args.append(args[1].split('.')[0] + ".cpu")

	#Compile
	compile_file(args[1], args[2])