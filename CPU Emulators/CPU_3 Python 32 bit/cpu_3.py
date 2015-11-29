#cpu_3.py

#32 bit cpu with 255gp+ia registers (all addressable)

#--------------------INSTRUCTION SET--------------------#
#
#ALU instructions input ra and rb and output into ra (CMP has no output) (unary operations take rb as input)
#_______________________________________________________
#| OPCODE   |   Registers/Args   |   Flags   |  Assem.  |
#|----------|--------------------|-----------|----------|
# 00000000    ra        00000000    olcaezdi    ZERO     ||FLAGS-(o=On/Use flags, l=Logic 0:OR/1:AND, c=Carry, a=ra>rb, e=ra=rb, z=Output Zero, d=ZeroDivision Error, i=Interrupt)
# 00000001    ra        00000000    olcaezdi    ONE
# 00000010    ra        rb          olcaezdi    ADD
# 00000011    ra        rb          olcaezdi    ADDC
# 00000100    ra        00000000    olcaezdi    INC
# 00000101    ra        rb          olcaezdi    SUB
# 00000110    ra        rb          olcaezdi    SUBC
# 00000111    ra        rb          olcaezdi    RSUB     ||Reverse subtract e.g. ra=rb-ra
# 00001000    ra        rb          olcaezdi    RSUBC
# 00001001    ra        00000000    olcaezdi    DEC
# 00001010    ra        rb          olcaezdi    MUL
# 00001011    ra        rb          olcaezdi    DIV
# 00001100    ra        rb          olcaezdi    MOD
# 00001101    ra        00000000    olcaezdi    RAND     ||Set ra to a random binary number
# 00010000    ra        00000000    olcaezdi    SHL
# 00010001    ra        00000000    olcaezdi    SHLC
# 00010010    ra        00000000    olcaezdi    SHR
# 00010011    ra        00000000    olcaezdi    SHRC
# 00010100    ra        00000000    olcaezdi    NOT
# 00010101    ra        rb          olcaezdi    AND
# 00010110    ra        rb          olcaezdi    NAND
# 00010111    ra        rb          olcaezdi    OR
# 00011000    ra        rb          olcaezdi    NOR
# 00011001    ra        rb          olcaezdi    XOR
# 00011010    ra        rb          olcaezdi    XNOR
# 00011011    ra        rb          olcaezdi    BCLR     ||Set ra to ra AND NOT rb
# 00011111    ra        rb          olcaezdi    CMP
#
#
#
#RAM/Register operations
# 00100000    ra        rb          olcaezdi    READ     ||Set ra to data at rb
# 00100001    ra        00000000    olcaezdi    DATA     ||Set ra to data in next byte
# 00100010    ra        rb          olcaezdi    WRITE    ||Write ra to rb
# 00100011    ra        00000000    olcaezdi    WRITETO  ||Write ra to address in next byte
# 00100100    ra        rb          olcaezdi    CLONE    ||Set ra to rb
#
#
#
#Jump operations
# 01000000    00000000  ra          olcaezdi    JUMP     ||Jump to ra
# 01000001    00000000  00000000    olcaezdi    JUMPTO   ||Jump to address in next byte
#
#
#
#Output operations
# 10000000    ra        00000000    olcaezdi    IN       ||Gets input from current device
# 10000001    ra        00000000    olcaezdi    OUT      ||Outputs to current device
# 10000010    00000000  00000000    olcaezdi    DATAO    ||Output data in next byte
# 10000011    00000000  00000000    olcaezdi    OUTFRM   ||Output data from address in next byte
# 10000100    ra        00000000    olcaezdi    ADR      ||Sets current device on output bus
# 10000101    ra        00000000    olcaezdi    OUTB     ||Outputs ra in binary to console
# 10000110    ra        00000000    olcaezdi    OUTD     ||Outputs ra as a decimal to colsole
#
#
#
#Other operations
# 11100000    00000000  00000000    olcaezdi    CLF      ||Clear ALU flags
# 11100001    00000000  00000000    olcaezdi    CLI      ||Clear interrupt flag
# 11111111    00000000  00000000    olcaezdi    HALT     ||Halt system
#
#-------------------------------------------------------#





#Imports
import collections,time
from random import randrange
import pygame
from pygame.locals import *



#MAX = maximum integer value+1
#Could be used to increase bits
BITS=32
MAX=2**BITS
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
        self.interrupt="0"

    #Called by CPU for any i/o instruction
    def set(self,data,i,address):

        #If current device is being changed
        if address:
            self.__current_device=self.__devices[int(data,2)%MAX]

        #If input is expected
        elif i:
            return self.__current_device.getInput()

        #If output is being sent
        else:
            self.__current_device.getOutput(data)

    def addDevice(self,device,address):
        self.__devices[address]=device

    def update(self):
        for i in self.__devices:
            device=self.__devices.get(i)
            device.update()
            if device.interrupt=="1":
                self.interrupt="1"



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
        if val<0:
            self.flags[0]="1"
            val+=MAX
        else:
            self.flags[0]="0"
        a.data=bin(val)

    #a=a-b+c-1
    def subC(self,a,b):
        val=a.decimal()-b.decimal()+int(self.flags[0])-1
        if val<0:
            self.flags[0]="1"
            val+=MAX
        else:
            self.flags[0]="0"
        a.data=bin(val)

    #a=b-a
    def rSub(self,a,b):
        val=b.decimal()-a.decimal()
        if val<0:
            self.flags[0]="1"
            val+=MAX
        else:
            self.flags[0]="0"
        a.data=bin(val)

    #a=b-a+c-1
    def rSubC(self,a,b):
        val=b.decimal()-a.decimal()+int(self.flags[0])-1
        if val<0:
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
        if len(b.data)==BITS+2:
            self.flags[0]=b.data[2]

    def shiftLC(self,a,b):
        a.data=bin(((b.decimal()<<1)+int(self.flags[0]))%MAX)
        if len(b.data)==BITS+2:
            self.flags[0]=b.data[2]

    def shiftR(self,a,b):
        a.data=bin(b.decimal()>>1)
        self.flags[0]=b.data[-1]

    def shiftRC(self,a,b):
        a.data=bin((b.decimal()>>1)+int(self.flags[0])*MAX/2)
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
        self.registers={"iar":IAR,0:IAR}   #Register 0 is iar

        for i in range(255):
            self.registers[i+1]=Register()
            
        self.out_bus=OutputBus()

        self.done=False

    #Loads program to RAM and starts main loop
    def start(self,prog):

        prog_file=open(prog,"r")
        program=[]

        while True:
            instruction=prog_file.readline()[:BITS]

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
                self.out_bus.update()
                vsleep(max(1/HERTZ-(time.time()-start),0))

        else:
            while not self.done:
                self.operation()
                self.out_bus.update()

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
        self.IR.data=self.IR.data[2:].zfill(BITS)

        #Check flags
        perform=False
        if self.IR.data[24]=="0":
            perform=True
        else:
            #l=OR
            if self.IR.data[25]=="0":
                for i,j in zip(self.alu.flags,[self.IR.data[26],self.IR.data[27],self.IR.data[28],self.IR.data[29],self.IR.data[30]]):
                    if i=="1" and j=="1":
                        perform=True
                        break;
                else:
                    if self.out_bus.interrupt=="1" and self.IR.data[31]=="1":
                        perform=True

            #l=AND
            else:
                if self.alu.flags==[self.IR.data[26],self.IR.data[27],self.IR.data[28],self.IR.data[29],self.IR.data[30]]:
                    perform=True

        if perform:
            #Perform operation

            #ALU Operations
            if self.IR.data[:3]=="000":
                a=self.registers[int(self.IR.data[8:16],2)]
                b=self.registers[int(self.IR.data[16:24],2)]
                self.alu.operation(self.IR.data[3:8],a,b)

            #RAM/Register operations
            elif self.IR.data[:3]=="001":
                a=self.registers[int(self.IR.data[8:16],2)]
                b=self.registers[int(self.IR.data[16:24],2)]
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
                    self.jump(self.registers[int(self.IR.data[16:24],2)])
                elif self.IR.data[3:8]=="00001":
                    self.jumpTo()

            #Output operations
            elif self.IR.data[:3]=="100":
                if self.IR.data[3:8]=="00000":
                    self.getIn(self.registers[int(self.IR.data[8:16],2)])
                elif self.IR.data[3:8]=="00001":
                    self.setOut(self.registers[int(self.IR.data[8:16],2)])
                elif self.IR.data[3:8]=="00010":
                    self.dataOut(self.registers[int(self.IR.data[8:16],2)])
                elif self.IR.data[3:8]=="00011":
                    self.outFrom(self.registers[int(self.IR.data[8:16],2)])
                elif self.IR.data[3:8]=="00100":
                    self.address(self.registers[int(self.IR.data[8:16],2)])
                elif self.IR.data[3:8]=="00101":
                    self.outputB(self.registers[int(self.IR.data[8:16],2)])
                elif self.IR.data[3:8]=="00110":
                    self.outputD(self.registers[int(self.IR.data[8:16],2)])

            #Other operations
            elif self.IR.data[:3]=="111":
                if self.IR.data[3:8]=="00000":
                    self.clearFlags()
                elif self.IR.data[3:8]=="00001":
                    self.clearInterrupt()
                elif self.IR.data[3:8]=="11111":
                    self.halt()

        else:
            if self.IR.data[:8] in ["00100001","00100011","01000001","10000010","10000011"]:
                self.registers["iar"].increment()

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

    def clearInterrupt(self):
        self.out_bus.interrupt="0"

    def halt(self):
        self.done=True



#---Device Classes---#



class Display(object):

    def __init__(self):
        self.screen=pygame.display.set_mode((511,509))
        self.interrupt="0"

    def update(self):
        pass

    def getInput(self):
        return bin(0)

    def getOutput(self,data):
        data=data[2:].zfill(32)[16:]
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

class Keyboard(object):

    def __init__(self):
        self.interrupt="0"
        self.log=[]

    def update(self):
        for event in pygame.event.get():
            if event.type == pygame.KEYDOWN:
                self.log.append(bin(event.key))

    def getInput(self):
        try:
            return self.log.pop(0)
        except:
            return bin(0)

    def getOutput(self,data):
        pass



#Prompt user for program and start CPU
def main():
    prog=input("Boot --> ")
    processor=CPU()
    processor.addDevice(Display(),0)
    processor.addDevice(Keyboard(),1)
    processor.start(prog)



main()