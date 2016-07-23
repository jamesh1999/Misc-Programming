clock1 = int(input("How many minutes fast per hour is clock 1? "))
clock2 = int(input("How many minutes fast per hour is clock 2? "))

new1 = 0
new2 = 0

while new1 % 60 == new2 % 60 and new1//60 == new2//60:
	new1+= 60+clock1
	new2+= 60+clock2

print(str(new1//60) + ":" + str(new1%60))