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

#Removes all macros
def removeMacros(lines):
	macros = {}

	for i, line in enumerate(lines):
		#Add new macro
		if line[:7] == "#define":
			splitted = line.split()
			macros[splitted[1]] = ' '.join(splitted[2:])
			lines[i] = ""
			continue;

		#Remove macros
		for macro in macros:
			while macro in line:
				line = line[:line.find(macro)] + macros[macro] + line[line.find(macro) + len(macro):]

		lines[i] = line

	return lines

def preprocess(lines):
	lines = removeStrings(lines)
	lines = removeMacros(lines)

	return lines