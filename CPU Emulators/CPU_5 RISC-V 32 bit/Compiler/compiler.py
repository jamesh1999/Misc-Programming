import sys

from tokeniser import *
from lr1_parser import *
from symbol_generator import *
from code_generator import *


def compile_file(filename, ofilename):
	with open(filename) as inputf:

		tokens = tokenise('\n'.join(inputf.readlines()))

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
			assembly += ' '.join(line) + "\n"
		
		print(assembly)

		with open(ofilename, 'w') as outputf:
			outputf.write(assembly)
		

#Automatically compile if main
if __name__ == "__main__":
	compile_file(sys.argv[1], sys.argv[2])