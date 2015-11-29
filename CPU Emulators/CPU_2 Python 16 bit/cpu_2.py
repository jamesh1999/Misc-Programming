#cpu_2.py

#16 bit cpu with 15gp+ia registers (all addressable)

#--------------------INSTRUCTION SET--------------------#
#
#ALU instructions input ra and rb and output into ra (CMP has no output) (unary operations take rb as input)
#
#00000000ra0000   ZERO
#00000001ra0000   ONE
#00000010rarb     ADD
#00000011rarb     ADDC
#00000100ra0000   INC
#00000101rarb     SUB
#00000110rarb     SUBC
#00000111rarb     RSUB     ||Reverse subtract e.g. ra=rb-ra
#00001000rarb     RSUBC
#00001001ra0000   DEC
#00001010rarb     MUL
#00001011rarb     DIV
#00001100rarb     MOD
#00001101ra0000   RAND     ||Set ra to a random binary number
#00010000ra0000   SHL
#00010001ra0000   SHLC
#00010010ra0000   SHR
#00010011ra0000   SHRC
#00010100ra0000   NOT
#00010101rarb     AND
#00010110rarb     NAND
#00010111rarb     OR
#00011000rarb     NOR
#00011001rarb     XOR
#00011010rarb     XNOR
#00011011rarb     BCLR     ||Set ra to ra AND NOT rb
#00011111rarb     CMP
#
#
#
#RAM/Register operations
#00100000rarb     READ     ||Set ra to data at rb
#00100001ra0000   DATA     ||Set ra to data in next byte
#00100010rarb     WRITE    ||Write ra to rb
#00100011ra0000   WRITETO  ||Write ra to address in next byte
#00100100rarb     CLONE    ||Set ra to rb
#
#
#
#Jump operations
#010000000000ra   JUMP     ||Jump to ra
#0100000100000000 JUMPTO   ||Jump to address in next byte
#01000010000caezd JUMPIF   ||Jump if caezd (Carry, A Greater, Equal, Output Zero (A=0 after operation), Zero Division) OR
#01000011000caezd JUMPIFA  ||Jump if caezd AND
#
#
#
#Output operations
#10000000ra0000   IN       ||Gets input from current device
#10000001ra0000   OUT      ||Outputs to current device
#1000001000000000 DATAO    ||Output data in next byte
#1000001100000000 OUTFRM   ||Output data from address in next byte
#10000100ra0000   ADR      ||Sets current device on output bus
#10000101ra0000   OUTB     ||Outputs ra in binary to console
#10000110ra0000   OUTD     ||Outputs ra as a decimal to colsole
#
#
#
#Other operations
#1110000000000000 CLF      ||Clear all flags
#1111111100000000 HALT     ||Halt system
#
#-------------------------------------------------------#





#Imports
import collections,time
from random import randrange
import pygame
from pygame.locals import *



#MAX = maximum integer value+1
#Could be used to increase bits
MAX=65536
HERTZ=600



class RAM(object):

    def __init__(self):
        self.data = collections.defaultdict(lambda: bin(0)) #All RAM cells default to 0

    #Load an entire program into RAM (called outside of CPU)
    def loadProgram(self,data,start=0):
        for i,j in enumerate(data):
            self.data[start+i]=j

    #Ram set and read functions
    def set(self,data,addr):
        self.data[addr]=data

    def read(self,addr):
        return self.data[addr]



#Register class contains data only
class Register(object):

    def __init__(self):
        self.data=bin(0)

    def clear(self):
        self.data=bin(0)

    def decimal(self):
        return int(self.data.split("b")[-1],2)



#Used only for IAR
class InstructionAddressRegister(Register):

    #Increment is used for two line instructions and to go to next instruction
    def increment(self):
        self.data=bin((self.decimal()+1)%MAX)



#Output bus for connecting devices to
class OutputBus(object):

    def __init__(self):
        self.__devices={}
        self.__current_device=None

    #Called by CPU for any i/o instruction
    def set(self,data,i,address):

        #If current device is being changed
        if address:
            self.__current_device=self.__devices[int(data,2)%MAX]

        #If input is expected
        elif i:
            return self.__current_device.getInput(self)

        #If output is being sent
        else:
            self.__current_device.getOutput(data)

    def addDevice(self,device,address):
        self.__devices[address]=device



class ALU(object):

    def __init__(self):
        self.flags=["0"]*5 #Carry,A greater,Equal,Output=0 (A=0 after operation),ZeroDivision

    #Perform the operation given by the CPU
    def operation(self,code,a,b):

        #Reset all flags that aren't required
        self.flags[1]="0"
        self.flags[2]="0"
        self.flags[3]="0"
        self.flags[4]="0"

        #Compare a and b if possible and set flags
        if not code in ["00000","00001","00100","01001","01101","10000","10001",
                        "10010","10011","10100"]: #List of codes for unary operations

            if a.decimal()>b.decimal():
                self.flags[1]="1"

            elif a.decimal()==b.decimal():
                self.flags[2]="1"

        #Perform operation based on code (sets carry flag to 0 if it isn't set in the operation)
        if code=="00000":
            self.zero(a,b)
            self.flags[0]="0"
        elif code=="00001":
            self.one(a,b)
            self.flags[0]="0"
        elif code=="00010":
            self.add(a,b)
        elif code=="00011":
            self.addC(a,b)
        elif code=="00100":
            self.inc(a,b)
        elif code=="00101":
            self.sub(a,b)
        elif code=="00110":
            self.subC(a,b)
        elif code=="00111":
            self.rSub(a,b)
        elif code=="01000":
            self.rSubC(a,b)
        elif code=="01001":
            self.dec(a,b)
        elif code=="01010":
            self.mul(a,b)
        elif code=="01011":
            self.div(a,b)
            self.flags[0]="0"
        elif code=="01100":
            self.mod(a,b)
            self.flags[0]="0"
        elif code=="01101":
            self.rand(a,b)
            self.flags[0]="0"
        elif code=="10000":
            self.shiftL(a,b)
        elif code=="10001":
            self.shiftLC(a,b)
        elif code=="10010":
            self.shiftR(a,b)
        elif code=="10011":
            self.shiftRC(a,b)
        elif code=="10100":
            self.Not(a,b)
            self.flags[0]="0"
        elif code=="10101":
            self.And(a,b)
            self.flags[0]="0"
        elif code=="10110":
            self.Nand(a,b)
            self.flags[0]="0"
        elif code=="10111":
            self.Or(a,b)
            self.flags[0]="0"
        elif code=="11000":
            self.Nor(a,b)
            self.flags[0]="0"
        elif code=="11001":
            self.Xor(a,b)
            self.flags[0]="0"
        elif code=="11010":
            self.Xnor(a,b)
            self.flags[0]="0"
        elif code=="11011":
            self.bitClear(a,b)
            self.flags[0]="0"
        elif code=="11111":
            self.flags[0]="0"

        #Check if output is 0 and set flag accordingly
        if not a.decimal():
            self.flags[3]="1"

    #-----Operations-----#
    #Data is saved to a
    #Unary operators use b as input
    #AddC,SubC,RSubC add 1 to answer if Carry flag is on

    #a=0
    def zero(self,a,b):
        a.clear()

    #a=1
    def one(self,a,b):
        a.data=bin(1)

    #a=a+b
    def add(self,a,b):
        val=a.decimal()+b.decimal()
        if val>=MAX:
            self.flags[0]="1"
        else:
            self.flags[0]="0"
        a.data=bin(val%MAX)

    #a=a+b+c
    def addC(self,a,b):
        val=a.decimal()+b.decimal()+int(self.flags[0])
        if val>=MAX:
            self.flags[0]="1"
        else:
            self.flags[0]="0"
        a.data=bin(val%MAX)

    #a=a+1
    def inc(self,a,b):
        val=a.decimal()+1
        if val>=MAX:
            self.flags[0]="1"
        else:
            self.flags[0]="0"
        a.data=bin(val%MAX)

    #a=a-b
    def sub(self,a,b):
        val=a.decimal()-b.decimal()
        if val>0:
            self.flags[0]="1"
            val+=MAX
        else:
            self.flags[0]="0"
        a.data=bin(val)

    #a=a-b+c-1
    def subC(self,a,b):
        val=a.decimal()-b.decimal()+int(self.flags[0])-1
        if val>0:
            self.flags[0]="1"
            val+=MAX
        else:
            self.flags[0]="0"
        a.data=bin(val)

    #a=b-a
    def rSub(self,a,b):
        val=b.decimal()-a.decimal()
        if val>0:
            self.flags[0]="1"
            val+=MAX
        else:
            self.flags[0]="0"
        a.data=bin(val)

    #a=b-a+c-1
    def rSubC(self,a,b):
        val=b.decimal()-a.decimal()+int(self.flags[0])-1
        if val>0:
            self.flags[0]="1"
            val+=MAX
        else:
            self.flags[0]="0"
        a.data=bin(val)

    #a=a-1
    def dec(self,a,b):
        val=a.decimal()-1
        if val<0:
            self.flags[0]="1"
            val+=MAX
        else:
            self.flags[0]="0"
        a.data=bin(val)

    #a=a*b
    def mul(self,a,b):
        val=a.decimal()*b.decimal()
        if val>=MAX:
            self.flags[0]="1"
        else:
            self.flags[0]="0"
        a.data=bin(val%MAX)

    #a=a/b
    def div(self,a,b):
        if b.decimal():
            val=a.decimal()//b.decimal()
            a.data=bin(val%MAX)
            self.flags[4]="0"
        else:
            a.data=bin(MAX-1)
            self.flags[4]="1"

    #a=a%b
    def mod(self,a,b):
        if b.decimal():
            val=(a.decimal()%b.decimal())+int(self.flags[0])
            a.data=bin(val)
            self.flags[4]="0"
        else:
            a.data=bin(MAX-1)
            self.flags[4]="1"

    def rand(self,a,b):
        a.data=bin(randrange(0,MAX))

    #Binary operations
    def shiftL(self,a,b):
        a.data=bin((b.decimal()<<1)%MAX)
        if len(b.data)==18:
            self.flags[0]=b.data[2]

    def shiftLC(self,a,b):
        a.data=bin(((b.decimal()<<1)+int(self.flags[0]))%MAX)
        if len(b.data)==18:
            self.flags[0]=b.data[2]

    def shiftR(self,a,b):
        a.data=bin(b.decimal()>>1)
        self.flags[0]=b.data[-1]

    def shiftRC(self,a,b):
        a.data=bin((b.decimal()>>1)+int(self.flags[0])*32768)
        self.flags[0]=b.data[-1]

    def Not(self,a,b):
        a.data=bin(~b.decimal())

    def And(self,a,b):
        a.data=bin(a.decimal()&b.decimal())

    def Nand(self,a,b):
        a.data=bin(~(a.decimal()&b.decimal()))

    def Or(self,a,b):
        a.data=bin(a.decimal()|b.decimal())

    def Nor(self,a,b):
        a.data=bin(~(a.decimal()|b.decimal()))

    def Xor(self,a,b):
        a.data=bin(a.decimal()^b.decimal())

    def Xnor(self,a,b):
        a.data=bin(~(a.decimal()^b.decimal()))

    def bitClear(self,a,b):
        a.data=bin(a.decimal()&(~b.decimal()))



class CPU(object):

    #Set up RAM, ALU, Output bus and registers and starts CPU
    def __init__(self):

        self.ram=RAM()

        self.alu=ALU()

        self.IR=Register()
        IAR=InstructionAddressRegister()
        self.registers={
                        "iar":IAR,   #Register 0 is iar
                        0:IAR,       #"
                        1:Register(),
                        2:Register(),
                        3:Register(),
                        4:Register(),
                        5:Register(),
                        6:Register(),
                        7:Register(),
                        8:Register(),
                        9:Register(),
                        10:Register(),
                        11:Register(),
                        12:Register(),
                        13:Register(),
                        14:Register(),
                        15:Register()
                        }

        self.out_bus=OutputBus()

        self.done=False

    #Loads program to RAM and starts main loop
    def start(self,prog):

        prog_file=open(prog,"r")
        program=[]

        while True:
            instruction=prog_file.readline()[:16]

            if instruction:
                program.append("0b"+instruction)
            else:
                break;

        self.ram.loadProgram(program)
        self.loop()

    #Main loop
    def loop(self):

        if HERTZ:

            vtime=time.time
            vsleep=time.sleep

            while not self.done:

                start=vtime()
                self.operation()
                vsleep(max(1/HERTZ-(time.time()-start),0))

        else:
            while not self.done:
                self.operation()

        input("The program has finished. Press enter to exit. ")

        #print(self.ram.data) #Uncomment to show contents of RAM when the program ends

    #Add a device to the output bus
    def addDevice(self,device,address):
        self.out_bus.addDevice(device,address)

    #Run every loop and gets/carries out each instruction
    def operation(self):

        #Get new instruction
        self.IR.data=self.ram.read(self.registers["iar"].decimal())

        #Format instruction e.g. 0b1010 --> "0000000000001010"
        self.IR.data=self.IR.data[2:].zfill(16)

        #Perform operation

        #ALU Operations
        if self.IR.data[:3]=="000":
            a=self.registers[int(self.IR.data[8:12],2)]
            b=self.registers[int(self.IR.data[12:],2)]
            self.alu.operation(self.IR.data[3:8],a,b)

        #RAM/Register operations
        elif self.IR.data[:3]=="001":
            a=self.registers[int(self.IR.data[8:12],2)]
            b=self.registers[int(self.IR.data[12:],2)]
            if self.IR.data[3:8]=="00000":
                self.read(a,b)
            elif self.IR.data[3:8]=="00001":
                self.data(a,b)
            elif self.IR.data[3:8]=="00010":
                self.write(a,b)
            elif self.IR.data[3:8]=="00011":
                self.writeTo(a,b)
            elif self.IR.data[3:8]=="00100":
                self.clone(a,b)

        #Jump operations
        elif self.IR.data[:3]=="010":
            if self.IR.data[3:8]=="00000":
                self.jump(self.registers[int(self.IR.data[12:],2)])
            elif self.IR.data[3:8]=="00001":
                self.jumpTo()
            elif self.IR.data[3:8]=="00010":
                self.jumpIf(self.IR.data[11:])
            elif self.IR.data[3:8]=="00011":
                self.jumpIfAnd(self.IR.data[11:])

        #Output operations
        elif self.IR.data[:3]=="100":
            if self.IR.data[3:8]=="00000":
                self.getIn(self.registers[int(self.IR.data[8:12],2)])
            elif self.IR.data[3:8]=="00001":
                self.setOut(self.registers[int(self.IR.data[8:12],2)])
            elif self.IR.data[3:8]=="00010":
                self.dataOut(self.registers[int(self.IR.data[8:12],2)])
            elif self.IR.data[3:8]=="00011":
                self.outFrom(self.registers[int(self.IR.data[8:12],2)])
            elif self.IR.data[3:8]=="00100":
                self.address(self.registers[int(self.IR.data[8:12],2)])
            elif self.IR.data[3:8]=="00101":
                self.outputB(self.registers[int(self.IR.data[8:12],2)])
            elif self.IR.data[3:8]=="00110":
                self.outputD(self.registers[int(self.IR.data[8:12],2)])

        #Other operations
        elif self.IR.data[:3]=="111":
            if self.IR.data[3:8]=="00000":
                self.clearFlags()
            elif self.IR.data[3:8]=="11111":
                self.halt()

        #Increment Instruction Address Register
        self.registers["iar"].increment()


    #-----Operations-----#


    #RAM/Register operations

    
    #Read operations (save to a, any addresses in b)
    def read(self,a,b):
        a.data=self.ram.read(b.decimal())

    def data(self,a,b):
        a.data=self.ram.read(self.registers["iar"].decimal()+1)
        self.registers["iar"].increment()

    #Write operations (data from a, any addresses in b)
    def write(self,a,b):
        self.ram.set(a.data,b.decimal())

    def writeTo(self,a,b):
        self.ram.set(a.data,int(self.ram.read(self.registers["iar"].decimal()+1),2))
        self.registers["iar"].increment()

    #Register operations
    def clone(self,a,b):
        a.data=b.data


    #Jump operations (always subtract 1 to account for increment at the end of the cycle)


    def jump(self,a):
        self.registers["iar"].data=bin(a.decimal()-1)

    def jumpTo(self):
        self.registers["iar"].data=bin(int(self.ram.read(self.registers["iar"].decimal()+1),2)-1)

    def jumpIf(self,flags):
        for i,j in zip(self.alu.flags,[flags[0],flags[1],flags[2],flags[3],flags[4]]):
            if i=="1" and j=="1":
                self.registers["iar"].data=bin(int(self.ram.read(self.registers["iar"].decimal()+1),2)-1)
                break;
        else:
            self.registers["iar"].increment()

    def jumpIfAnd(self,flags):
        if self.alu.flags==[flags[0],flags[1],flags[2],flags[3],flags[4]]:
            self.registers["iar"].data=bin(int(self.ram.read(self.registers["iar"].decimal()+1),2)-1)
        else:
            self.registers["iar"].increment()


    #Output operations


    def getIn(self,a):
        a.data=self.out_bus.set(None,True,False)

    def setOut(self,a):
        self.out_bus.set(a.data,False,False)

    def dataOut(self,a):
        self.out_bus.set(self.ram.read(self.registers["iar"].decimal()+1),False,False)
        self.registers["iar"].increment()

    def outFrom(self,a):
        self.out_bus.set(self.ram.read(int(self.ram.read(self.registers["iar"].decimal()+1),2)),False,False)
        self.registers["iar"].increment()

    def address(self,a):
        self.out_bus.set(a.data,False,True)

    def outputD(self,a):
        print(int(a.decimal()))

    def outputB(self,a):
        print(a.data)



    #Other operations


    def clearFlags(self):
        self.alu.flags=["0"]*5

    def halt(self):
        self.done=True



#---Device Classes---#



class Display(object):

    def __init__(self):
        self.screen=pygame.display.set_mode((511,509))

    def getInput(self,bus):
        return bin(0)

    def getOutput(self,data):
        data=data[2:].zfill(16)
        x=int(data[8:],2)
        y=int(data[:8],2)

        if y==255:
            pygame.display.flip()
            self.screen.fill((0,0,0))

        else:
            self.screen.set_at((x*2,y*2),(255,255,255))
            self.screen.set_at((x*2+1,y*2),(255,255,255))
            self.screen.set_at((x*2,y*2+1),(255,255,255))
            self.screen.set_at((x*2+1,y*2+1),(255,255,255))



#Prompt user for program and start CPU
def main():
    prog=input("Boot --> ")
    processor=CPU()
    processor.addDevice(Display(),0)
    processor.start(prog)



main()