import assembler,re

varis = {}



class CompilerError(Exception):
	pass



def load(filename):

	src = ""

	with open(filename,'r') as new_file:
		for line in new_file.readlines():
			src+=line

	return src





def compileFile(filename):
	#Load source
	src = load(filename)

	#Lexer
	

	print(src)



if __name__ == "__main__":
	compileFile(input("Compile: "))