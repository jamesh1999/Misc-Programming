import os

class PreprocessorError(Exception):
	pass



#Replaces "" strings with char arrays
def removeStrings(lines):
	for i, line in enumerate(lines):
		in_string = False
		escaped = False

		nline = ""

		for char in line:
			#Handle escaped characters
			if escaped and in_string:
				nline += "'\\" + char + "',"
				continue;

			#Handle string beginnings/ends
			if char == '\"':
				if in_string:
					#Remove trailing comma
					if nline[-1] == ',':
						nline = nline[:-1]

					nline += '}'
					in_string = False
				else:
					nline += '{'
					in_string = True
				continue;

			#Skip if not in string
			if not in_string:
				nline += char
				continue;

			#Handle escaped characters
			if char == '\\':
				escaped = True
				continue;

			nline += "'" + char + "',"

		lines[i] = nline

	return lines

macros = {}
#Removes all preprocessor statements (#s)
def removePreprocessorStatements(lines):
	global macros
	if_values = []

	for i, line in enumerate(lines):
		#Ignore errors if line too short
		try:
			if line[:6] == "#endif":
				if_values.pop()
				lines[i] = ""
				continue;

			if line[:6] == "#ifdef":
				try:
					splitted = line.split()
					if_values.append(splitted[1] in macros)
				except IndexError:
					raise PreprocessorError(line + " is an invalid preprocessor statement.")
				lines[i] = ""
				continue;

			if line[:7] == "#ifndef":
				try:
					splitted = line.split()
					if_values.append(not splitted[1] in macros)
				except IndexError:
					raise PreprocessorError(line + " is an invalid preprocessor statement.")
				lines[i] = ""
				continue;

			#Ignore line if if failed
			if False in if_values:
				lines[i] = ""
				continue;

			#Include other .cm files
			if line[:8] == "#include":
				try:
					splitted = line.split()
					stdlib = splitted[1][0] == "<" and splitted[1][-1] == ">"
					lines[i] = preprocess(splitted[1].strip('"<>'), stdlib = stdlib)
				except IndexError:
					raise PreprocessorError(line + " is an invalid preprocessor statement.")
				continue;

			if line[:] == "#undefine":
				try:
					splitted = line.split()
					macros.pop(splitted[1], None)
				except IndexError:
					raise PreprocessorError(line + " is an invalid preprocessor statement.")

		except IndexError:
			pass

		#Redo in case try failed
		if False in if_values:
			lines[i] = ""
			continue;

		#Remove macros
		for macro in macros:
			while macro in line:
				line = line[:line.find(macro)] + macros[macro] + line[line.find(macro) + len(macro):]

		#Ignore errors if line too short
		try:
			#Add new macro
			if line[:7] == "#define":
				splitted = line.split()

				try:
					if len(splitted) > 2:
						macros[splitted[1]] = ' '.join(splitted[2:])
					else:
						macros[splitted[1]] = ""
				except IndexError:
					raise PreprocessorError(line + " is an invalid preprocessor statement.")

				lines[i] = ""
				continue;

		except IndexError:
			pass

		#Add line
		lines[i] = line

	#Expand all includes
	done = False
	while not done:
		done = True

		for i, line in enumerate(lines):
			if not isinstance(line, str):
				done = False

				lines = lines[:i] + lines[i] + lines[i + 1:]
				break;

	return lines

def preprocess(filename, stdlib = False):
	lines = []

	#Use default folder for stdlib
	path = os.path.join(os.path.dirname(__file__), "../C - Code/" + filename)
	with open(path if stdlib else filename) as inputf:
		lines = removePreprocessorStatements(inputf.readlines())
		lines = removeStrings(lines)

	return lines