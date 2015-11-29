DEPTH=6
CLEARANCE=10

cur_depth=0

class Node(object):

	def __init__(self,minmax,state,game,alpha=None,beta=None,depth=0):
		self.minmax=minmax
		self.children=[]
		self.state=state
		self.game=game
		self.depth=depth
		self.evaluation=0
		self.alpha=alpha
		self.beta=beta
		if (DEPTH and depth==DEPTH) or depth==cur_depth:
			self.eval_self()
		else:
			self.eval_branch()


	def getNextChild(self):
		return False

	def eval_self(self):
		pass

	def eval_branch(self):
		if self.minmax=="min":
			while self.getNextChild():
				val=self.children[-1].evaluation
				try:
					self.beta=min(val,self.beta)
					if self.beta<=self.alpha:
						self.evaluation=self.beta
						break;
				except:
					pass
			minimum=self.children[0].evaluation
			for child in self.children[1:]:
				minimum=min(minimum,child.evaluation)
			self.evaluation=minimum		
		elif self.minmax=="chance":
			tot=0
			while self.getNextChild():
				tot+=self.children[-1].evaluation
			self.evaluation=tot/len(self.children)		
		elif self.minmax=="max":
			while self.getNextChild():
				val=self.children[-1].evaluation
				try:
					self.alpha=max(val,self.alpha)
					if self.alpha>=self.beta:
						self.evaluation=self.alpha
						break;
				except:
					pass
			maximum=self.children[0].evaluation
			for child in self.children[1:]:
				maximum=max(maximum,child.evaluation)
			self.evaluation=maximum

class AI(object):

	def __init__(self,game,depth=None):
		self.depth=depth
		self.game=game
		self.alpha=None
		self.beta=None

	@property
	def depth(self):
		return DEPTH

	@depth.setter
	def depth(self, val):
		self.depth=val
		if not val==None:
			global DEPTH
			DEPTH=depth
			self.adaptive_depth=False
		else:
			self.adaptive_depth=True

	def getMove(self):
		#If adaptive depth is on decide on the depth
		if self.adaptive_depth:
			global cur_depth
			cur_depth=self.chooseDepth()

		#Evaluate and decide on move
		moves=self.getPossibleMoves()
		vals=[]
		for i in moves:
			node=Node("min",moves[i],self.game,alpha=self.alpha-CLEARANCE,beta=self.beta+CLEARANCE)
			vals.append(node.evaluation)
		maximum=vals[0]
		num=0
		for i in range(len(vals)-1):
			if vals[i+1]>maximum:
				num=i+1
				maximum=vals[i+1]
		return moves[num]

	def getPossibleMoves(self):
		return []

	def chooseDepth(self):
		return 0