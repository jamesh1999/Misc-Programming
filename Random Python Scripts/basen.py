import math

def convertToBaseN(integer):
	output = []
	n = 2
	while integer > 0:
		digit = integer % n
		integer = (integer - digit) // n
		output = [digit] + output
		n += 1
	return output

primes = [2]

t = 3
while True:
	for p in primes:
		if p <= math.sqrt(t) + 1 and not t % p:
			break;
	else:
		primes.append(t)

		a = convertToBaseN(primes[-2])
		b = convertToBaseN(primes[-1])

		for i,j in enumerate(b):
			ai = i - (len(b) - len(a))
			if ai < 0:
				print(str(i) + " " + str(j))
			else:
				if not a[ai] == b[i]:
					print(str(i) + " " + str(b[i] - a[ai]))
		print("===")

	t += 1
