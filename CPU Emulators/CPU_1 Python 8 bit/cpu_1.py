class RAM(object):
    def __init__(self):
        self.data = {}

    def set_prog(self,start,data):
        cnt=start
        for i in data:
            self.data[cnt]=i
            cnt+=1

    def set(self,byte,addr):
        self.data[addr]=byte

    def read(self,addr):
        return self.data[addr]

class CPU(object):
    def __init__(self):
        self.ram=RAM()
        self.IAR="00000000"
        self.IR="00000000"
        self.ACC="00000000"
        self.GPR=["00000000"]*4
        self.FLAGS=["0"]*4
        self.bus="00000000"
        self.done=False

    def start(self,prog):
        file_prog=open(prog,"r")
        program=[]
        while True:
            instruction=file_prog.readline()[:8]
            if instruction:
                program.append(instruction)
            else:
                break;
        self.ram.set_prog(0,program)
        self.loop()

    def loop(self):
        while not self.done:
            self.operation()
        print(self.ram.data)

    def operation(self):
        #Get new instruction
        self.IR=self.ram.read(int(self.IAR,2))
        if self.IR[0]=="1":
            a=int(self.IR[4:6],2)
            b=int(self.IR[6:8],2)

            if int(self.GPR[a],2)>int(self.GPR[b],2):
                self.FLAGS[1]="1"
            else:
                self.FLAGS[1]="0"

            if int(self.GPR[a],2)==int(self.GPR[b],2):
                self.FLAGS[2]="1"
            else:
                self.FLAGS[2]="0"

            if self.IR[1:4]=="000":
                val=int(self.GPR[a],2)+int(self.GPR[b],2)
                self.FLAGS[0]="1" if val>255 else "0"
                self.GPR[b]=bin(val%256)[2:]
            elif self.IR[1:4]=="001":
                new=self.FLAGS[0]+self.GPR[a]
                self.FLAGS[0]=new[8]
                self.GPR[b]=new[:8]
            elif self.IR[1:4]=="010":
                new=self.GPR[a]+self.FLAGS[0]
                self.FLAGS[0]=new[0]
                self.GPR[b]=new[1:]
            elif self.IR[1:4]=="011":
                self.GPR[b]=""
                for i in self.GPR[a]:
                    if i=="1":
                        self.GPR[b]+="0"
                    else:
                        self.GPR[b]+="1"
            elif self.IR[1:4]=="100":
                for j,i in enumerate(self.GPR[a]):
                    if i=="1" and self.GPR[b][j]=="1":
                        self.GPR[b][j]="1"
                    else:
                        self.GPR[b][j]="0"
            elif self.IR[1:4]=="101":
                for j,i in enumerate(self.GPR[a]):
                    if i=="1" or self.GPR[b][j]=="1":
                        self.GPR[b][j]="1"
                    else:
                        self.GPR[b][j]="0"
            elif self.IR[1:4]=="110":
                for j,i in enumerate(self.GPR[a]):
                    if (i=="1" or self.GPR[b][j]=="1") and not (i=="1" and self.GPR[b][j]=="1"):
                        self.GPR[b][j]="1"
                    else:
                        self.GPR[b][j]="0"
            if not self.IR[1:4]=="111":
                if self.GPR[b]=="00000000":
                    self.FLAGS[3]="1"
                else:
                    self.FLAGS[3]="0"


        else:
            if self.IR[1:4]=="000":
                self.GPR[int(self.IR[6:8],2)]=self.ram.read(int(self.GPR[int(self.IR[6:8],2)],2))
            elif self.IR[1:4]=="001":
                self.ram.set(self.GPR[int(self.IR[6:8],2)],int(self.GPR[int(self.IR[4:6],2)],2))
            elif self.IR[1:4]=="010":
                self.GPR[int(self.IR[6:8],2)]=self.ram.read(int(self.IAR,2)+1)
                self.IAR=bin(int(self.IAR,2)+1)[2:]
            elif self.IR[1:4]=="011":
                self.IAR=bin(int(self.GPR[int(self.IR[6:8],2)],2)-1)[2:]
            elif self.IR[1:4]=="100":
                self.IAR=bin(int(self.ram.read(int(self.IAR,2)+1),2)-1)[2:]
            elif self.IR[1:4]=="101" and (self.IR[4]==self.FLAGS[0] and self.IR[5]==self.FLAGS[1] and self.IR[6]==self.FLAGS[2] and self.IR[7]==self.FLAGS[3]):
                self.IAR=bin(int(self.ram.read(int(self.IAR,2)+1),2)-1)[2:]
            elif self.IR[1:4]=="110":
                self.FLAGS="0000"
            elif self.IR[1:4]=="111":
                self.done=True

        for i,j in enumerate(self.GPR):
            self.GPR[i]=("0"*(8-len(j)))+j
        self.IAR=bin(int(self.IAR,2)+1)[2:]



def main():
    prog=input("Which program do you wish to run? ")
    processor=CPU()
    processor.start(prog)

main()