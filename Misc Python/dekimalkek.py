import time

seconds = 0
minutes = 0
hours = 0

def printTime():
	sseconds = str(seconds).zfill(2)
	sminutes = str(minutes).zfill(2)
	print(str(hours) + ":" + str(sminutes) + ":" + str(sseconds))


#Loop
while True:

	#Get time
	t = time.strftime("%H %M %S").split()
	s = (int(t[0]) * 3600 + int(t[1]) * 60 + int(t[2])) / 0.864
	nseconds = int(s) % 100
	minutes = int(s / 100) % 100
	hours = int(s / 10000)

	if nseconds != seconds:
		seconds = nseconds
		printTime()