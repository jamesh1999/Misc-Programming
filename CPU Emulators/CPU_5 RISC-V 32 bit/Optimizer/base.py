import os
from tokenizer import *

tokenizer = Tokenizer()
tokenizer.configure(combine = True, remove = [' ', '\t', '\n', ',', '(', ')', '#'], split = [" ", "\t", "\n","{", "}", "(", ")", "[", "]", "<", ">", ",", ".", "/", ";", "~", "!", "?", "'", "%", "^", "&", "*", "-", "+", "=", "|", ":", "@", '#'])

OPTIMIZER_LOG = ""

def printLog(text, e = "\n"):
	global OPTIMIZER_LOG
	print(text, end = e)
	OPTIMIZER_LOG += str(text) + e

def saveLogFile():
	path = os.path.join(os.path.dirname(__file__), "log.txt")
	with open(path, "w") as log_file:
		log_file.write(OPTIMIZER_LOG)

def getOp(tokens):
	if len(tokens):
		return tokens[0].upper()

def getDest(tokens):
	#Instructions with no real destination
	if getOp(tokens) in ["SW", "SH", "SB", "OUT", "BRANCH", "BNE", "BEQ", "BLT", "BLTU", "BLE", "BLEU", "BGT", "BGTU", "BGE", "BGEU", "JAL", "JALR", "GOTO", "PUSH", "POP", "DEL", "TOP", "CALL", "RET", "IMPORT"]:
		return None

	if len(tokens) > 1:
		return tokens[1].upper()