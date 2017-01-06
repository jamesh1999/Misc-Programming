import math
lookup_file = open("trig_lookup.txt","r")
table = {}
stable = []
ctable = []
ttable = []

for line in lookup_file.readlines():
	vals = line.split()
	table[int(round(float(vals[0])))] = vals[1:]
	stable.append((float(vals[1]),float(vals[0])))
	ctable.append((float(vals[2]),float(vals[0])))
	ttable.append((float(vals[3]),float(vals[0])))

MUL = len(stable)/math.radians(360)

def sin(x):
	return math.sin(x)

def cos(x):
	return math.cos(x)

def tan(x):
	return math.tan(x)

def asin2(x,y):
	try:
		var = x/y
		close = None
		cval = None
		for pair in stable:
			if close==None or (pair[0]-var)**2<close:
				close = (pair[0]-var)**2
				cval = pair[1]
		return cval
	except:
		return 0

def acos2(x,y):
	try:
		var = x/y
		close = None
		cval = None
		for pair in ctable:
			if close==None or (pair[0]-var)**2<close:
				close = (pair[0]-var)**2
				cval = pair[1]
		return cval
	except:
		return 0

def atan2(x,y):
	try:
		var = x/y
		close = None
		cval = None
		for pair in ttable:
			if close==None or (pair[0]-var)**2<close:
				close = (pair[0]-var)**2
				cval = pair[1]
		return cval
	except:
		return 0

def main():
	write = open("trig_lookup.txt","w")
	accuracy = float(input("What accuracy do you want? (degrees) "))

	iterations = int(360/accuracy)
	if accuracy*iterations>360:
		iterations-=1

	for i in range(iterations):
		angle = math.radians(accuracy*i)
		write.write(str(angle)+" "+str(math.sin(angle))+" "+str(math.cos(angle))+" "+str(math.tan(angle))+"\n")

	write.close()


if __name__=="__main__":
	import math
	main()