from Optimizer.base import *
from Optimizer.basic import *

PRIORITY_SPEED = 0
PRIORITY_BALANCED = 1
PRIORITY_SIZE = 2

OPTIMIZATION_NONE = -1
OPTIMIZATION_BASIC = 0
OPTIMIZATION_STANDARD = 1
OPTIMIZATION_FULL = 2

def optimizeAssembly(filename, optimisation_level, priority_level):

	printLog("Optimizer starting...\n")
	printLog("Opening " + filename)
	ifile = open(filename, 'r')
	printLog("Opened\n")

	printLog("Reading file:")
	assembly = ifile.read()
	for i,line in enumerate(assembly.split('\n')):
		printLog(str(i) + max(5 - len(str(i)), 0) * " " + ": " + line)
	printLog("Read\n")

	ifile.close()

	start_lines = len(assembly.split('\n'))

	printLog("Beginning optimizations...\n")

	#Basic level optimizations
	if optimisation_level >= 1:
		assembly = basic(assembly)

	printLog("Finished optimizing:")
	for i,line in enumerate(assembly.split('\n')):
		printLog(str(i) + max(5 - len(str(i)), 0) * " " + ": " + line)

	end_lines = len(assembly.split('\n'))
	diff = start_lines - end_lines
	printLog("\nReduced assembly by " + str(diff) + " instructions (" + str(100 * diff / start_lines)[:5] + "%)")

	printLog("\nSaving " + filename)
	ofile = open(filename, 'w')
	ofile.write(assembly)
	ofile.close()
	printLog("Saved\n")

	print("Saving log...")
	saveLogFile()
	print("Done")

	return assembly