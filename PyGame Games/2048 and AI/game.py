import pygame,random,time
from pygame.locals import *

IMG_2=pygame.image.load("two.png")
IMG_4=pygame.image.load("four.png")
IMG_8=pygame.image.load("eight.png")
IMG_16=pygame.image.load("sixteen.png")
IMG_32=pygame.image.load("thirty-two.png")
IMG_64=pygame.image.load("sixty-four.png")
IMG_128=pygame.image.load("one-hundred-and-twenty-eight.png")
IMG_256=pygame.image.load("two-hundred-and-fifty-six.png")
IMG_512=pygame.image.load("five-hundred-and-twelve.png")
IMG_1024=pygame.image.load("one-thousand-and-twenty-four.png")
IMG_2048=pygame.image.load("two-thousand-and-forty-eight.png")
IMG_4096=pygame.image.load("four-thousand-and-ninety-six.png")

class GameBoard(object):
    def __init__(self):
        self.board=self.newBoard()
        self.addNumber()
        self.done=False
        self.won=False
        self.screen=pygame.display.set_mode((256,256))
        self.score=0
        self.ever_finish=True

    def move(self,move,board):
        if move=="u":
            tboard=self.newBoard()
            col=[0,0,0,0]
            score=0
            for x in range(4):
                for y in range(4):
                    if board[y][x]=="":
                        col[x]+=1
                    elif not y==0:
                        tboard[y-col[x]][x]=board[y][x]
                        if not (y-col[x])-1<0:
                            if board[y][x]==tboard[(y-col[x])-1][x]:
                                tboard[(y-col[x])-1][x]=str(int(board[y][x])*2)
                                tboard[y-col[x]][x]=""
                                col[x]+=1
                                score+=int(board[y][x])*2
                    else:
                        tboard[y][x]=board[y][x]
            if not board==tboard:
                return True,tboard,score
            else:
                return False,tboard,0

        if move=="d":
            tboard=self.newBoard()
            col=[0,0,0,0]
            score=0
            for x in range(4):
                for y in range(3,-1,-1):
                    if board[y][x]=="":
                        col[x]+=1
                    elif not y==3:
                        tboard[y+col[x]][x]=board[y][x]
                        if not (y+col[x])+1>3:
                            if board[y][x]==tboard[(y+col[x])+1][x]:
                                tboard[(y+col[x])+1][x]=str(int(board[y][x])*2)
                                tboard[y+col[x]][x]=""
                                col[x]+=1
                                score+=int(board[y][x])*2
                    else:
                        tboard[y][x]=board[y][x]
            if not board==tboard:
                return True,tboard,score
            else:
                return False,tboard,0

        if move=="l":
            tboard=self.newBoard()
            row=[0,0,0,0]
            score=0
            for y in range(4):
                for x in range(4):
                    if board[y][x]=="":
                        row[y]+=1
                    elif not x==0:
                        tboard[y][x-row[y]]=board[y][x]
                        if not x-row[y]-1<0:
                            if board[y][x]==tboard[y][(x-row[y])-1]:
                                tboard[y][(x-row[y])-1]=str(int(board[y][x])*2)
                                tboard[y][x-row[y]]=""
                                row[y]+=1
                                score+=int(board[y][x])*2
                    else:
                        tboard[y][x]=board[y][x]
            if not board==tboard:
                return True,tboard,score
            else:
                return False,tboard,0

        if move=="r":
            tboard=self.newBoard()
            row=[0,0,0,0]
            score=0
            for y in range(4):
                for x in range(3,-1,-1):
                    if board[y][x]=="":
                        row[y]+=1
                    elif not x==3:
                        tboard[y][x+row[y]]=board[y][x]
                        if not x+row[y]+1>3:
                            if board[y][x]==tboard[y][(x+row[y])+1]:
                                tboard[y][(x+row[y])+1]=str(int(board[y][x])*2)
                                tboard[y][x+row[y]]=""
                                row[y]+=1
                                score+=int(board[y][x])*2
                    else:
                        tboard[y][x]=board[y][x]
            if not board==tboard:
                return True,tboard,score
            else:
                return False,tboard,0

    def addNumber(self):
        spaces=[]
        for x in range(4):
            for y in range(4):
                if self.board[y][x]=="":
                    spaces.append((y,x))
        if len(spaces):
            space=spaces[random.randrange(len(spaces))]
            num=2
            if not random.randrange(10):
                num=4
            self.board[space[0]][space[1]]=str(num)


    def newBoard(self):
        board=[]
        for i in range(4):
            board.append(["","","",""])
        return board

    def getMove(self):
        fin=False
        while not fin:
            for event in pygame.event.get():
                if event.type==pygame.QUIT:
                    self.done=True
                    fin=True
                elif event.type==pygame.KEYDOWN:
                    if event.key==K_UP and self.a:
                        self.board=self.u[:]
                        self.score+=self.score_u
                        fin=True
                    elif event.key==K_DOWN and self.b:
                        self.board=self.d[:]
                        self.score+=self.score_d
                        fin=True
                    elif event.key==K_LEFT and self.c:
                        self.board=self.l[:]
                        self.score+=self.score_l
                        fin=True
                    elif event.key==K_RIGHT and self.e:
                        self.board=self.r[:]
                        self.score+=self.score_r
                        fin=True

                    if event.key==K_ESCAPE:
                        pygame.display.iconify()

    def draw(self):
        self.screen.fill((0,0,0))

        y=0
        for i in self.board:
            x=0
            for j in i:
                image=None
                if j=="2":
                    image=IMG_2
                elif j=="4":
                    image=IMG_4
                elif j=="8":
                    image=IMG_8
                elif j=="16":
                    image=IMG_16
                elif j=="32":
                    image=IMG_32
                elif j=="64":
                    image=IMG_64
                elif j=="128":
                    image=IMG_128
                elif j=="256":
                    image=IMG_256
                elif j=="512":
                    image=IMG_512
                elif j=="1024":
                    image=IMG_1024
                elif j=="2048":
                    image=IMG_2048
                elif j=="4096":
                    image=IMG_4096
                if not image==None:
                    self.screen.blit(image,(x,y,x+64,y+64))
                x+=64
            y+=64           

        pygame.display.flip()

    def loop(self):
        while not self.done:

            self.addNumber()
            self.draw()

            #Test for possible moves
            self.a,self.u,self.score_u=self.move("u",self.board)
            self.b,self.d,self.score_d=self.move("d",self.board)
            self.c,self.l,self.score_l=self.move("l",self.board)
            self.e,self.r,self.score_r=self.move("r",self.board)
            if not self.a and not self.b and not self.c and not self.e:
                self.done=True
                break;

            self.getMove()
            self.draw()
            time.sleep(0.2)

            if self.ever_finish:
                #Test for 2048
                for i in range(4):
                    for j in range(4):
                        if self.board[i][j]=="2048":
                            self.won=True
                            self.done=True
                            break;

        self.draw()
        time.sleep(0.5)
        if self.won:
            print("You win!")
        else:
            print("You lost!")
        print("Your score was: "+str(self.score))
        input("Press enter to exit ")

if __name__=="__main__":
    game=GameBoard()
    game.loop()