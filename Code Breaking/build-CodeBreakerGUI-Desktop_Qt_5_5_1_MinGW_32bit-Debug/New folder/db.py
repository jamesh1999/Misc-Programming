import sqlite3, os

ofile = open("words.txt", "w")

filenames = []
for name in os.listdir():
	if '.' in name:
		if name.split('.')[1].isdigit():
			filenames.append(name)

for f in filenames:
	with open(f) as ifile:
		for line in ifile.readlines():
			line = line.upper()
			nline = ""
			for ch in line:
				if 64< ord(ch) < 91:
					nline += ch
			if(len(nline) == 1 and not nline in ["I", "A"]):
				continue;
			ofile.write(nline + "\n")
ofile.close()
