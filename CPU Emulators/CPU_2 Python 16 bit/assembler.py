import sys

prog=open(sys.argv[1])
out=open(sys.argv[2],"w")

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
		"JUMP":"010000000000",
		"JUMPTO":"0100000100000000",
		"JUMPIF":"01000010",
		"JUMPIFA":"01000011",
		"IN":"10000000",
		"OUT":"10000001",
		"DATAO":"10000010",
		"OUTFRM":"10000011",
		"ADR":"10000100",
		"OUTB":"10000101",
		"OUTD":"10000110",
		"CLF":"1110000000000000",
		"HALT":"1111111100000000",
}

REGISTERS={
		"IAR":"0000",
		"R1":"0001",
		"R2":"0010",
		"R3":"0011",
		"R4":"0100",
		"R5":"0101",
		"R6":"0110",
		"R7":"0111",
		"R8":"1000",
		"R9":"1001",
		"R10":"1010",
		"R11":"1011",
		"R12":"1100",
		"R13":"1101",
		"R14":"1110",
		"R15":"1111"
}



print("-----Assembling Code-----")
print("Input: "+sys.argv[1])
print("Output: "+sys.argv[2])
print("\n")



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
			for i in range(16-len(binary)):
				binary="0"+binary
			new=binary

		#If it's an instruction then get the code
		except:

			new=MAIN[instruction[0]]

			#Instructions with no parameters
			if instruction[0] in ["JUMPTO","DATAO","OUTFRM","CLF","HALT"]:
				pass

			#Instructions with flag parameters
			elif instruction[0] in ["JUMPIF","JUMPIFA"]:
				new+="000"

				try:
					#Flags present
					new+="1" if "C" in instruction[1] else "0"
					new+="1" if "A" in instruction[1] else "0"
					new+="1" if "E" in instruction[1] else "0"
					new+="1" if "Z" in instruction[1] else "0"
					new+="1" if "D" in instruction[1] else "0"

				except:
					#No flags present
					new+="00000"

			#Instructions with one parameter at the end
			elif instruction[0] in ["JUMP"]:
				new+=REGISTERS[instruction[1]]

			#Instructions with one parameter in the middle
			elif instruction[0] in ["ZERO","ONE","INC","DEC","RAND","SHL","SHLC","SHR","SHRC","NOT","DATA","WRITETO","IN","OUT","ADR","OUTB","OUTD"]:
				new+=REGISTERS[instruction[1]]+"0000"

			#Instructions with two parameters
			else:
				new+=REGISTERS[instruction[1]]+REGISTERS[instruction[2]]

		out.write(new+"\n")
		print("-->                "+new)

	else:
		break;



print("\n")
print("Finished!")
print("Writing to file...")
print("Done")
print("Closing file...")
out.close()
print("Done")