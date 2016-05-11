PARSE_TREE = None
SYMBOL_TABLE = None
BASE_TEMPLATE = None

def printiMacro():
	return BASE_TEMPLATE + [["Call", ":print_d"]]

def printfMacro():
	return [["Push", "ZERO"],["Call", ":print_str"]]