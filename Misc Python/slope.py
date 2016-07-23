
while True:
	funct = input("f(x) = ")
	x = int(input("At what value of x do you want to find the slope? "))

	gradient_funct = []

	buff = ""
	for char in funct:
		if char=="+" or char=="-":
			gradient_funct.append(buff)
			buff=char
		elif char==" " or char=="^":
			pass
		else:
			buff+=char

	gradient_funct.append(buff)

	for i,j in enumerate(gradient_funct):
		if not "x" in j:
			multiplier = 0
			power = 0
		else:
			multiplier,power = j.split('x')
			if power=="":
				power=1
			if multiplier=="":
				multiplier=1

		gradient_funct[i] = [float(multiplier) * int(power), int(power) - 1]

	m = 0

	for i in gradient_funct:
		m+= (x**i[1])*i[0]

	print(m)