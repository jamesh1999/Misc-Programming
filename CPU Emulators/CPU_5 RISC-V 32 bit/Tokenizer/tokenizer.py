class Tokenizer(object):
	def __init__(self):
		SPLIT_ON = [" ", "\t", "\n","{", "}", "(", ")", "[", "]", "<", ">", ",", ".", "/", ";", "~", "!", "?", "'", "%", "^", "&", "*", "-", "+", "=", "|"]
		REMOVE = [" ", "\t", "\n"]

	#Configure the tokenizer
	def configure(self, split = None, remove = None):
		if not split == None:
			self.SPLIT_ON = split
		if not remove == None:
			self.REMOVE = remove

	#Override for custom special rules
	#Default: 'x' denotes a character and should be converted to an int
	#		  '//' denotes a comment
	def removeSpecials(string):
		#Remove comments
		nstring = ""
		in_comment = False
		for i in range(len(string)):
			#Update state
			if string[i:i + 2] == "//":
				in_comment = True
			elif string[i] == '\n':
				in_comment = False

			if not in_comment:
				nstring += string[i]

		#Substitute chars
		done = False
		while not done:
			done = True

			for i,char in enumerate(nstring):
				if char == "'":
					done = False
					#Special chars
					if nstring[i + 1] == "\\":
						val = 0
						if nstring[i + 2] == 'n':
							val = ord('\n')
						elif nstring[i + 2] == 't':
							val = ord('\t')
						elif nstring[i + 2] == '\\':
							val = ord('\\')
						elif nstring[i + 2] == '\'':
							val = ord('\'')

						nstring = nstring[:i] + str(val) + nstring[i + 4]

					else:
						nstring = nstring[:i] + str(ord(nstring[i + 1])) + nstring[i + 3]

					break;

		return nstring

	def tokenizeString(input):
		input_string = removeSpecials(input_string)

		tokens = []

		buff = ""
		for char in input_string:
			if char in SPLIT_ON:
				if len(buff):
					tokens.append(buff)
					buff = ""

				if not char in REMOVE:
					tokens.append(char)
			elif not char in REMOVE:
				buff += char

		if len(buff):
			tokens.append(buff)

		return tokens

	def tokenizeFile(ifile):
		return tokenizeString('\n'.join(ifile.readlines()))