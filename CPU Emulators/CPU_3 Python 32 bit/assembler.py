import sys

MAIN={
		"ZERO":"00000000",
		"ONE":"00000001",
		"ADD":"00000010",
		"ADDC":"00000011",
		"INC":"00000100",
		"SUB":"00000101",
		"SUBC":"00000110",
		"RSUB":"00000111",
		"RSUBC":"00001000",
		"DEC":"00001001",
		"MUL":"00001010",
		"DIV":"00001011",
		"MOD":"00001100",
		"RAND":"00001101",
		"SHL":"00010000",
		"SHLC":"00010001",
		"SHR":"00010010",
		"SHRC":"00010011",
		"NOT":"00010100",
		"AND":"00010101",
		"NAND":"00010110",
		"OR":"00010111",
		"NOR":"00011000",
		"XOR":"00011001",
		"XNOR":"00011010",
		"BCLR":"00011011",
		"CMP":"00011111",
		"READ":"00100000",
		"DATA":"00100001",
		"WRITE":"00100010",
		"WRITETO":"00100011",
		"CLONE":"00100100",
		"JUMP":"0100000000000000",
		"JUMPTO":"010000010000000000000000",
		"IN":"10000000",
		"OUT":"10000001",
		"DATAO":"100000100000000000000000",
		"OUTFRM":"100000110000000000000000",
		"ADR":"10000100",
		"OUTB":"10000101",
		"OUTD":"10000110",
		"CLF":"111000000000000000000000",
		"HALT":"111111110000000000000000",
		"SCR":"0000000000000000" #Not actually an instruction
}

DEFINED={}

def getRegister(reg):
	vals=reg.split("-")
	if len(vals)==2:
		DEFINED[vals[1]]=str(bin(int(vals[0]))[2:]).zfill(8)
	if vals[0]=="IAR":
		return "00000000"
	elif vals[0] in DEFINED:
		return DEFINED[vals[0]]
	else:
		vals[0]=vals[0][1:]
		return str(bin(int(vals[0]))[2:]).zfill(8)



def assemble(filename,outname):
	print("-----Assembling Code-----")
	print("Input: "+filename)
	print("Output: "+outname)
	print("\n")

	prog=open(filename)
	print("Opened file: "+filename)
	out=open(outname)
	print("Opened file: "+outname)



	while True:

		#Get next line
		line=prog.readline().upper()

		if not line=="":

			if "#" in line:
				line=line.split('#')
				if len(line)==1:
					continue;
				else:
					line=line[0]

			instruction=line.split()
			length=0
			for i in instruction:
				length+=len(i)+1
				print(i,end=" ")
			print(" "*max(0,20-length),end=" ")

			#If it is a number then convert it to binary
			try:

				binary=bin(int(instruction[0]))[2:]
				new=binary.zfill(32)

			#If it's an instruction then get the code
			except:

				new=MAIN[instruction[0]]

				#Instructions with no parameters
				if instruction[0] in ["JUMPTO","DATAO","OUTFRM","CLF","HALT"]:
					pass

				#Instructions with one parameter at the end
				elif instruction[0] in ["JUMP"]:
					new+=getRegister(instruction[1])

				#Instructions with one parameter in the middle
				elif instruction[0] in ["ZERO","ONE","INC","DEC","RAND","SHL","SHLC","SHR","SHRC","NOT","DATA","WRITETO","IN","OUT","ADR","OUTB","OUTD"]:
					new+=getRegister(instruction[1])+"00000000"

				#Screen co-ords
				elif instruction[0] in ["SCR"]:
					coords=instruction[1].split(",")
					x=bin(max(coords[0],255))[2:]
					y=bin(max(coords[1],254))[2:]
					new+=y.zfill(8)
					new+=x.zfill(8)



				#Instructions with two parameters
				else:
					new+=getRegister(instruction[1])+getRegister(instruction[2])


				#Add flags
				if not instruction[-1][0]=="R" and not instruction[-1] in ["JUMPTO","DATAO","OUTFRM","CLF","HALT"] and not instruction[-1] in DEFINED:
					new+="1"
					new+="1" if "L" in instruction[1] else "0"
					new+="1" if "C" in instruction[1] else "0"
					new+="1" if "A" in instruction[1] else "0"
					new+="1" if "E" in instruction[1] else "0"
					new+="1" if "Z" in instruction[1] else "0"
					new+="1" if "D" in instruction[1] else "0"
					new+="1" if "I" in instruction[1] else "0"
				else:
					new+="00000000"


			out.write(new+"\n")
			print("-->                "+new)

		else:
			break;

	print("\n")
	print("Finished!")
	print("Writing to file...")
	print("Done")
	print("Closing file: "+outname)
	out.close()
	print("Done")

if __name__ == "__main__":
	assemble(sys.argv[1], sys.argv[2])