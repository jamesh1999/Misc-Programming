import sys

from tokeniser import *
from lr1_parser import *
from symbol_generator import *
from code_generator import *


def compile_file(filename):
	with open(filename) as inputf:

		tokens = tokenise('\n'.join(inputf.readlines()))
		tree = parse(tokens)
		symbols = generateSymbolTable(tree)
		#assembly = generateCode(tree, symbols)
		print(tree)
		print("\n\n\n")
		print(symbols)
		#print("\n\n\n")
		#print(assembly)

#Automatically compile if main
if __name__ == "__main__":
	compile_file(sys.argv[1])