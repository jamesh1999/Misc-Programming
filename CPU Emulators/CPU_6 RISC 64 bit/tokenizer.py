class Tokenizer(object):
	def __init__(self):
		self.__SPLIT_ON = [" ", "\t", "\n","{", "}", "(", ")", "[", "]", "<", ">", ",", ".", "/", ";", "~", "!", "?", "'", "%", "^", "&", "*", "-", "+", "=", "|", ":"]
		self.__REMOVE = [" ", "\t", "\n"]
		self.__COMBINE = False
		self.__cache = {}

	#Configure the tokenizer
	def configure(self, split = None, remove = None, combine = False):
		if not split == None:
			self.__SPLIT_ON = split
		if not remove == None:
			self.__REMOVE = remove
		self.__COMBINE = combine

		#Clear cache
		self.__cache = {}

	#Override for custom special rules
	#Default: 'x' denotes a character and should be converted to an int
	#		  '//' denotes a comment
	def removeSpecials(self, string):
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
		while True:

			for i,char in enumerate(nstring):
				if char == "'":
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

						nstring = nstring[:i] + str(val) + nstring[i + 4:]

					else:
						nstring = nstring[:i] + str(ord(nstring[i + 1])) + nstring[i + 3:]

					break;

			else:
				break;

		return nstring

	def tokenizeString(self, input_string):
		if input_string in self.__cache: 
			return self.__cache[input_string][:]

		input_string = self.removeSpecials(input_string)
		if input_string == "": return []

		tokens = []

		buff = ""
		mode = input_string[0] in self.__SPLIT_ON

		for char in input_string:

			if char in self.__SPLIT_ON:
				#Flush buffer
				if not self.__COMBINE or not mode or char in self.__REMOVE:
					if len(buff):
						tokens.append(buff)
						buff = ""

				mode = True

			elif mode:
				if len(buff):
					tokens.append(buff)	
					buff = ""

				mode = False

			if not char in self.__REMOVE:
				buff += char

		if len(buff):
			tokens.append(buff)

		self.__cache[input_string] = tokens[:]

		return tokens

	def tokenizeFile(self, ifile):
		return self.tokenizeString('\n'.join(ifile.readlines()))