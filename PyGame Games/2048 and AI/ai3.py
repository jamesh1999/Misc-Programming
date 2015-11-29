import pygame,random,time,math,copy
from pygame.locals import *
from game import *

#Blank square values
USE_BLANK_CHECK=True
BLANK_VAL=3

#Smoothness values
USE_SMOOTH_CHECK=True
SMOOTH_VAL=0.22

#Max num values
USE_MAX=False
MAX_VAL=1

#Monotonicity values
USE_MONOTONICITY_CHECK=True
MONOTONICITY_VAL=4.9

#Score values
USE_SCORE=False
SCORE_VAL=0.5

#Moves values
USE_MOVES=False
MOVES_VAL=1

#Ave. val values
USE_AVE=False
AVE_VAL=0.6

USE_CORNER=True
CORNER_VAL=10

#Other values
USE_2048=False

#Minimax search and pruning values
MIN_DEPTH=0
MAX_DEPTH=2
CLEARANCE=10

cur_depth=None
prev_alpha=None
prev_beta=None

CHEAT=True

def eval_board(board,game):
	evaluation=0

	#Check board monotonicity
	if USE_MONOTONICITY_CHECK:
		directions=[0,0,0,0]
		
		#Up/down
		for x in range(4):
			cur=0
			next=cur+1
			while next<4:
				while next<3 and board[next][x]=="":
					next+=1
				try:
					val=math.log(int(board[cur][x]),2)
				except:
					val=0
				try:
					next_val=math.log(int(board[next][x]),2)
				except:
					next_val=0
				if val>next_val:
					directions[0]+=next_val-val
				else:
					directions[1]+=val-next_val
				cur=next
				next+=1

		#Left/right
		for y in range(4):
			cur=0
			next=cur+1
			while next<4:
				while next<3 and board[y][next]=="":
					next+=1
				try:
					val=math.log(int(board[y][cur]),2)
				except:
					val=0
				try:
					next_val=math.log(int(board[y][next]),2)
				except:
					next_val=0
				if val>next_val:
					directions[2]+=next_val-val
				else:
					directions[3]+=val-next_val
				cur=next
				next+=1
		evaluation+=max(directions[0],directions[1])+max(directions[2],directions[3])*MONOTONICITY_VAL

	smoothness = 0;

	high=2

	cnt=0

	tot=0

	for x in range(4):
		for y in range(4):

			if board[y][x]=="":

				#Count blank squares
				if USE_BLANK_CHECK:
					cnt+=1

			else:

				#Check board smoothness
				if USE_SMOOTH_CHECK:
					val = math.log(int(board[y][x]),2)
					try:
						valy=math.log(int(board[y+1][x]),2)
						smoothness-=math.fabs(val-valy)
					except:
						pass
					try:
						valx=math.log(int(board[y][x-1]),2)
						smoothness-=math.fabs(val-valx)
					except:
						pass

				#Try and get a high max value
				if (USE_MAX or (USE_CORNER and (x==0 or x==3) and (y==0 or y==3))) and int(board[y][x])>high:
					high=int(board[y][x])

				#Checks for 2048
				if USE_2048 and board[y][x]=="2048":
						evaluation+=999999

				if USE_AVE:
					tot+=int(board[y][x])

		evaluation+=(smoothness*SMOOTH_VAL)+(math.log(high,2)*MAX_VAL*int(USE_MAX))+(tot/(16-cnt)*AVE_VAL)+(cnt*BLANK_VAL)+(math.log(high,2)*CORNER_VAL*int(USE_CORNER))

	#Counts probably available moves
	if USE_MOVES:
		a,u,w=game.move("u",board)
		b,d,x=game.move("d",board)
		c,l,y=game.move("l",board)
		e,r,z=game.move("r",board)
		evaluation+=(int(a)+int(b)+int(c)+int(e))*MOVES_VAL
		if not int(a)+int(b)+int(c)+int(e):
			evaluation-=999999

	return evaluation

def AIMove(game):

	global cur_depth
	global prev_alpha
	global prev_beta

	vals=[0,0,0,0]
	boards=[game.u,game.d,game.l,game.r]
	nodes=[None,None,None,None]

	blanks=0
	for x in range(4):
		for y in range(4):
			if game.board[y][x]=="":
				blanks+=1

	cur_depth=max(MIN_DEPTH,int((16-blanks)/2))
	if not cur_depth%2:
		cur_depth-=1
	cur_depth=min(cur_depth,MAX_DEPTH)
	
	for i in range(4):

		#Eliminate impossible moves
		if i==0:
			if not game.a:
				vals[0]=None
				continue;
		elif i==1:
			if not game.b:
				vals[1]=None
				continue;
		elif i==2:
			if not game.c:
				vals[2]=None
				continue;
		elif i==3:
			if not game.e:
				vals[3]=None
				continue;
		nodes[i]=Node("min",boards[i],game,alpha=prev_alpha,beta=prev_beta)

		vals[i]=nodes[i].evaluation

	#Chose random move with highest score
	num=0
	high=vals[0]
	for i in range(3):
		i+=1
		if not vals[i]==None:
			if not high==None:
				if vals[i]>high:
					num=i
					high=vals[i]
			else:
				num=i
				high=vals[i]

	prev_alpha=nodes[num].alpha
	prev_beta=nodes[num].beta
	try:
		prev_alpha-=CLEARANCE
		prev_beta+=CLEARANCE
	except:
		pass

	return boards[num]

class Node(object):

	def __init__(self,minmax,board,game,alpha=None,beta=None,depth=0,score=0):
		self.minmax=minmax
		self.children=[]
		self.board=board
		self.game=game
		self.depth=depth
		self.score=score
		self.evaluation=0
		self.alpha=alpha
		self.beta=beta
		if depth==cur_depth:
			self.eval_self()
		else:
			self.eval_branch()

	def getChild(self):
		cnt=len(self.children)
		game=self.game
		if self.minmax=="max":
			if not self.depth==cur_depth-2:
				if not cnt:
					a,b,c=game.move("u",self.board)
					if a:
						self.children.append(Node("min",b,game,alpha=self.alpha,beta=self.beta,depth=self.depth+1,score=self.score+c))
					else:
						self.children.append(None)
				elif cnt==1:
					a,b,c=game.move("d",self.board)
					if a:
						self.children.append(Node("min",b,game,alpha=self.alpha,beta=self.beta,depth=self.depth+1,score=self.score+c))
					else:
						self.children.append(None)
				elif cnt==2:
					a,b,c=game.move("l",self.board)
					if a:
						self.children.append(Node("min",b,game,alpha=self.alpha,beta=self.beta,depth=self.depth+1,score=self.score+c))
					else:
						self.children.append(None)
				elif cnt==3:
					a,b,c=game.move("r",self.board)
					if a:
						self.children.append(Node("min",b,game,alpha=self.alpha,beta=self.beta,depth=self.depth+1,score=self.score+c))
					else:
						self.children.append(None)
			else:
				if not cnt:
					a,b,c=game.move("u",self.board)
					if a:
						self.children.append(Node("chance",b,game,alpha=self.alpha,beta=self.beta,depth=self.depth+1,score=self.score+c))
					else:
						self.children.append(None)
				elif cnt==1:
					a,b,c=game.move("d",self.board)
					if a:
						self.children.append(Node("chance",b,game,alpha=self.alpha,beta=self.beta,depth=self.depth+1,score=self.score+c))
					else:
						self.children.append(None)
				elif cnt==2:
					a,b,c=game.move("l",self.board)
					if a:
						self.children.append(Node("chance",b,game,alpha=self.alpha,beta=self.beta,depth=self.depth+1,score=self.score+c))
					else:
						self.children.append(None)
				elif cnt==3:
					a,b,c=game.move("r",self.board)
					if a:
						self.children.append(Node("chance",b,game,alpha=self.alpha,beta=self.beta,depth=self.depth+1,score=self.score+c))
					else:
						self.children.append(None)				
		elif not CHEAT:
			done=False
			i,j=divmod(cnt,2)
			for x in range(4):
				for y in range(4):
					if self.board[y][x]=="":
						if not i:
							tboard=copy.deepcopy(self.board)
							tboard[y][x]=str(((j+1)*2))
							self.children.append(Node("max",tboard,game,depth=self.depth+1,score=self.score))
							done=True
							break;
						else:
							i-=1
				if done:
					break;
			return done
		else:
			i=cnt
			done=False
			for x in range(4):
				for y in range(4):
					if self.board[y][x]=="":
						if not i:
							tboard=copy.deepcopy(self.board)
							tboard[y][x]="2"
							self.children.append(Node("max",tboard,game,depth=self.depth+1,score=self.score))
							done=True
							break;
						else:
							i-=1
				if done:
					break;
			return done	

	def eval_self(self):
		self.evaluation=eval_board(self.board,self.game)
		if USE_SCORE:
			self.evaluation+=self.score*SCORE_VAL

	def eval_branch(self):
		if self.minmax=="min":
			while self.getChild():
				val=self.children[len(self.children)-1].evaluation
				try:
					self.beta=min(val,self.beta)
					if self.beta<=self.alpha:
						break;
				except:
					pass
			try:
				if self.beta<=self.alpha:
					self.evaluation=self.beta
				else:
					minimum=self.children[0].evaluation
					for child in self.children[1:]:
						if child.evaluation<minimum:
							minimum=child.evaluation
					self.evaluation=minimum
			except:
				minimum=self.children[0].evaluation
				for child in self.children[1:]:
					if child.evaluation<minimum:
						minimum=child.evaluation
				self.evaluation=minimum			
		elif self.minmax=="chance":
			tot=0
			cnt=0
			while self.getChild():
				val=self.children[-1].evaluation
				tot+=val*0.9
				self.getChild()
				val=self.children[-1].evaluation
				tot+=val*0.1
				cnt+=1
			self.evaluation=tot/cnt		
		elif self.minmax=="max":
			for i in range(4):
				self.getChild()
				if not self.children[i]==None:
					val=self.children[i].evaluation
					try:
						self.alpha=max(val,self.alpha)
						if self.alpha>=self.beta:
							break;
					except:
						pass
			try:
				if self.alpha>=self.beta:
					self.evaluation=self.alpha
				else:
					maximum=None
					for child in self.children:
						if not child==None:
							if not maximum==None:
								if child.evaluation>maximum:
									maximum=child.evaluation
							else:
								maximum=child.evaluation
					if not maximum==None:
						self.evaluation=maximum
			except:
				maximum=None
				for child in self.children:
					if not child==None:
						if not maximum==None:
							if child.evaluation>maximum:
								maximum=child.evaluation
						else:
							maximum=child.evaluation
				if not maximum==None:
					self.evaluation=maximum		

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

    def addNumber(self):
        if not CHEAT:
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
        else:
            num=0
            high=None
            typ=None
            for i in range(15):
                x,y=divmod(i,4)
                if self.board[y][x]=="":
                    temp2=copy.deepcopy(self.board)
                    temp2[y][x]="2"
                    evaluation=eval_board(temp2,self)
                    if high==None:
                        high=evaluation
                        num=i
                        typ=2
                    elif evaluation>high:
                        high=evaluation
                        num=i
                        typ=2
            x,y=divmod(num,4)
            self.board[y][x]=str(typ)



    def loop(self):
        self.ever_finish=True
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
                    if self.board[i][j]=="2048" and self.ever_finish:
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
    game=GameBoardAI()
    game.loop()