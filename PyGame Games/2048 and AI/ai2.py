import pygame,random,time,copy,threading
from pygame.locals import *
from game import *

DEPTH=4

VALS=[[0,0],[0,0],[0,0],[0,0]]

def AIMove(game):
	boards=[game.u,game.d,game.l,game.r]
	threads=[None,None,None,None]
	
	for i in range(4):

		#Eliminate impossible moves
		if i==0:
			if not game.a:
				VALS[0][0]=-999999
				continue;
		elif i==1:
			if not game.b:
				VALS[1][0]=-999999
				continue;
		elif i==2:
			if not game.c:
				VALS[2][0]=-999999
				continue;
		elif i==3:
			if not game.e:
				VALS[3][0]=-999999
				continue;

		threads[i]=Worker(boards[i],game,i)
		threads[i].start()

	for i in threads:
		try:
			i.join()
		except:
			pass

	val=[a[0] for a in VALS]
	death=[b[1] for b in VALS]
	maximum=max(val)
	if val.count(maximum)>1:
		cnt=0
		indexes=[]
		for i in val:
			if i==maximum:
				indexes.append(cnt)
			cnt+=1

		minimum=1
		for i in indexes:
			if death[i]<=minimum:
				minimum=death[i]
				choice=i

	else:
		choice=val.index(maximum)

	return boards[choice]

class Worker(threading.Thread):
	def __init__(self,board,game,num):
		threading.Thread.__init__(self)
		self.args=(board,game)
		self.num=num

	def run(self):
		node=Node("min",self.args[0],self.args[1])
		global VALS
		VALS[self.num]=[node.evaluation,node.death_chance]


class Node(object):

	def __init__(self,minmax,board,game,alpha=-99999,beta=99999,depth=0):
		self.minmax=minmax
		self.children=[]
		self.board=board
		self.game=game
		self.depth=depth
		self.alpha=alpha
		self.beta=beta
		self.death_chance=0
		if depth==DEPTH:
			self.evaluation=board[0].count("")+board[1].count("")+board[2].count("")+board[3].count("")
		else:
			self.eval_branch()

	def getChild(self):
		cnt=len(self.children)
		game=self.game
		if self.minmax=="max":
			a,b,c=game.move(["u","d","l","r"][cnt],self.board)
			if a:
				self.children.append(Node("min",b,game,alpha=self.alpha,beta=self.beta,depth=self.depth+1))
			else:
				self.children.append(None)			
		else:
			done=False
			i,j=divmod(cnt,2)
			for x in range(4):
				for y in range(4):
					if self.board[y][x]=="":
						if not i:
							tboard=copy.deepcopy(self.board)
							tboard[y][x]=str(((j+1)*2))
							self.children.append(Node("max",tboard,game,depth=self.depth+1))
							done=True
							break;
						else:
							i-=1
				if done:
					break;
			return done

	def eval_branch(self):
		if self.minmax=="min":
			while self.getChild():
				val=self.children[-1].evaluation
				self.death_chance+=self.children[-1].death_chance
				if val<self.beta:
					self.beta=val
					if self.beta<=self.alpha:
						break;
			self.evaluation=self.beta
			self.death_chance/=len(self.children)
		elif self.minmax=="max":
			self.death_chance=1
			for i in range(4):
				self.getChild()
				if not self.children[i]==None:
					val=self.children[i].evaluation
					if val>self.alpha or (val==self.alpha and self.children[i].death_chance<self.death_chance):
						self.alpha=val
						self.death_chance=self.children[i].death_chance
						if self.alpha>=self.beta:
							break;
			self.evaluation=self.alpha

class GameBoardAI(GameBoard):
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

            #Test for 2048
            for i in range(4):
                for j in range(4):
                    if self.board[i][j]=="2048":
                        self.won=True
                        self.done=True
                        break;

        time.sleep(0.5)
        if self.won:
            print("You win!")
        else:
            print("You lost!")
        print("Your score was: "+str(self.score))
        input("Press enter to exit ")


if __name__=="__main__":
    game=GameBoardAI()
    game.loop()