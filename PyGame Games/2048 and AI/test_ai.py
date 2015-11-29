import pygame,random,time
from pygame.locals import *
from game import *
from ai import *

TESTS=20

class GameBoardTest(GameBoardAI):
    def __init__(self):
        self.board=self.newBoard()
        self.addNumber()
        self.done=False
        self.won=False
        self.score=0

    def getMove(self):
        self.board=AIMove(self)
        if self.board==self.u:
            self.score+=self.score_u
        elif self.board==self.d:
            self.score+=self.score_d
        elif self.board==self.l:
            self.score+=self.score_l
        elif self.board==self.r:
            self.score+=self.score_r

    def loop(self):
        while not self.done:

            self.addNumber()

            #Test for possible moves
            self.a,self.u,self.score_u=self.move("u",self.board)
            self.b,self.d,self.score_d=self.move("d",self.board)
            self.c,self.l,self.score_l=self.move("l",self.board)
            self.e,self.r,self.score_r=self.move("r",self.board)
            if not self.a and not self.b and not self.c and not self.e:
                self.done=True
                break;

            self.getMove()

            #Test for 2048
            for i in range(4):
                for j in range(4):
                    if self.board[i][j]=="2048":
                        self.won=True
                        self.done=True
                        break;

        return self.won,self.score

wins=0
for i in range(TESTS):
	game=GameBoardTest()
	a,b=game.loop()
	if a:
		wins+=1
	print("Game "+str(i)+" finished.")
win_rate=(wins/TESTS)*100
print("Win rate: "+str(int(win_rate))+"%")