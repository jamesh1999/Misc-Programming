import random,pygame,time
from pygame.locals import *

WIDTH = None
HEIGHT = None
IMG_TRAP=pygame.image.load("trap.png")
IMG_TREASURE=pygame.image.load("treasure.png")
IMG_PLAYER=pygame.image.load("player.png")
IMG_MONSTER=pygame.image.load("monster.png")
IMG_MONSTER1=pygame.image.load("monster1.png")
IMG_MONSTEREYES2=pygame.image.load("monstereyes2.png")
IMG_MONSTER1EYES2=pygame.image.load("monster1eyes2.png")
IMG_MONSTER1EYES3=pygame.image.load("monster1eyes3.png")
IMG_LIGHT=pygame.image.load("light.png")
IMG_EYES=pygame.image.load("eyes.png")
IMG_EYES2=pygame.image.load("eyes2.png")
IMG_EYES3=pygame.image.load("eyes3.png")
IMG_ACTIVATED=pygame.image.load("trapactivated.png")
IMG_PILLAR1=pygame.image.load("pillar1.png")
IMG_PILLAR2=pygame.image.load("pillar2.png")
IMG_PILLAR3=pygame.image.load("pillar3.png")
IMG_PILLAR4=pygame.image.load("pillar4.png")
IMG_PILLAR1L=pygame.image.load("pillar1l.png")
IMG_PILLAR2L=pygame.image.load("pillar2l.png")
IMG_PILLAR3L=pygame.image.load("pillar3l.png")
IMG_PILLAR4L=pygame.image.load("pillar4l.png")

def tint( surface, colour, original=None ):
    tintSurf = pygame.Surface( surface.get_size() ).convert_alpha()
    tintSurf.fill( colour )
    if original != None:
        surface.blit( original, (0,0) )
    surface.blit( tintSurf, (0,0), special_flags=BLEND_RGB_MULT )

def roomGen():
	room=[]
	for i in range(WIDTH):
		col=[]
		for j in range(HEIGHT):
			col.append("")
		room.append(col)

	viable=[]
	for i in range(WIDTH):
		if i==0 or i==WIDTH-2 or i==WIDTH-1:
			continue;

		for j in range(HEIGHT):
			if j==0 or j==HEIGHT-2 or j==HEIGHT-1:
				continue;

			for v in viable:
				if 3>i-v[0]>-3 and 3>j-v[1]>-3:
					break;
			else:
				viable.append((i,j))

	for i in range(len(viable)//6):
		pillar=random.choice(viable)
		room[pillar[0]][pillar[1]]="P1"
		room[pillar[0]+1][pillar[1]]="P2"
		room[pillar[0]+1][pillar[1]+1]="P3"
		room[pillar[0]][pillar[1]+1]="P4"

	return room


class Entity(object):
	def __init__(self,coords,image,offset=0):
		self.image=image
		self.coords=coords
		self.last="u"
		self.offset=offset

	def getImage(self,image=None):
		if image==None:
			image=self.image
		if self.last=="u":
			return pygame.transform.rotate(image,self.offset)
		elif self.last=="d":
			return pygame.transform.rotate(image,180+self.offset)
		elif self.last=="l":
			return pygame.transform.rotate(image,90+self.offset)
		elif self.last=="r":
			return pygame.transform.rotate(image,270+self.offset)

class Player(Entity):
	def __init__(self):
		Entity.__init__(self,[0,0],IMG_PLAYER,offset=90)
		self.last="r"

	def move(self):
		moved=False
		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				self._done = True
				break;

			if event.type == pygame.KEYDOWN:
				if event.key == K_ESCAPE and event.mod&KMOD_SHIFT:
					self._done = True
				elif event.key == K_ESCAPE:
					pygame.display.iconify()

				elif event.key == K_RIGHT:
					if not self.coords[0]>=WIDTH-1:
						self.coords[0]+=1
						self.last="r"
						moved=True
				elif event.key == K_LEFT:
					if not self.coords[0]==0:
						self.coords[0]-=1
						self.last="l"
						moved=True

				elif event.key == K_UP:
					if not self.coords[1]==0:
						self.coords[1]-=1
						self.last="u"
						moved=True

				elif event.key == K_DOWN:
					if not self.coords[1]>=HEIGHT-1:
						self.coords[1]+=1
						self.last="d"
						moved=True
		return moved

class Monster(Entity):
	def __init__(self):
		Entity.__init__(self,[0,0],IMG_MONSTER,offset=-90)
		self.last=random.choice(["l","r","d","u"])
		self.images=(IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER1,IMG_MONSTER1,IMG_MONSTER1,
			IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER1,IMG_MONSTER1,IMG_MONSTER1,
			IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER1,IMG_MONSTER1,IMG_MONSTER1,
			IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER1,IMG_MONSTER1,IMG_MONSTER1,
			IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER1,IMG_MONSTER1,IMG_MONSTER1,
			IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER1,IMG_MONSTER1,IMG_MONSTER1,
			IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTER,IMG_MONSTEREYES2,IMG_MONSTER1EYES3,IMG_MONSTER1EYES3,IMG_MONSTER1EYES2,)
		self.inum=0
		self.eimages=(IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,
			IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,
			IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,
			IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,
			IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES,IMG_EYES2,IMG_EYES3,IMG_EYES3,IMG_EYES3,IMG_EYES2)
		self.eimage=IMG_EYES

	def move(self,player):
		new_coords=self.coords[:]
		distx=player.coords[0]-self.coords[0]
		disty=player.coords[1]-self.coords[1]

		if distx**2>=disty**2:
			if distx<0:
				new_coords[0]-=1
				self.last="l"
			elif distx>0:
				new_coords[0]+=1
				self.last="r"
		else:
			if disty<0:
				new_coords[1]-=1
				self.last="u"
			elif disty>0:
				new_coords[1]+=1
				self.last="d"
		self.coords=new_coords

	def update(self):
		self.inum=(self.inum+1)%len(self.images)
		self.image=self.images[self.inum]
		self.eimage=self.eimages[self.inum]


	def getEyesImage(self):
		if self.last=="u":
			return pygame.transform.rotate(self.eimage,self.offset)
		elif self.last=="d":
			return pygame.transform.rotate(self.eimage,180+self.offset)
		elif self.last=="l":
			return pygame.transform.rotate(self.eimage,90+self.offset)
		elif self.last=="r":
			return pygame.transform.rotate(self.eimage,270+self.offset)



class Game(object):
	def __init__(self):
		self.screen=pygame.display.set_mode((0,0),pygame.FULLSCREEN)
		w,h=self.screen.get_size()
		global WIDTH
		global HEIGHT
		WIDTH=w//64
		HEIGHT=h//64
		self.player=Player()
		self.monster=Monster()
		self.monster_found=False
		self.room=roomGen()
		self.done=False
		self.monster_moves=1
		self.start()

	def loop(self):
		while not self.done:
			self.update()
			self.draw()
			time.sleep(0.1)

	def start(self):

		for i in ["T","T","W"]:
			while True:
				randx=random.randint(0,WIDTH-1)
				randy=random.randint(0,HEIGHT-1)
				if self.room[randx][randy]=="":
					self.room[randx][randy]=i
					break;

		while True:
			mrandx=random.randint(1,WIDTH-1)
			mrandy=random.randint(1,HEIGHT-1)
			if self.room[mrandx][mrandy]=="":
				self.monster.coords=[mrandx,mrandy]
				break;

		self.draw()

	def update(self):
		if self.player.move():
			if self.monster_found:
				for i in range(self.monster_moves):
					self.monster.move(self.player)

			if self.room[self.player.coords[0]][self.player.coords[1]]=="T":
				print("You activated a trap hidden on the floor and hear a loud groaning sound nearby")
				self.monster_found=True
				self.room[self.player.coords[0]][self.player.coords[1]]="A"

			if self.player.coords==self.monster.coords and not self.monster_found:
				self.monster_found=True
				print("You have woken up the monster")

			elif self.player.coords==self.monster.coords and self.monster_found:
				print("The monster killed you")
				self.done=True

			if self.player.coords==self.getCoords("W"):
				print("You found the treasure!")
				self.done=True
		self.monster.update()


	def getCoords(self,key):
		found=False
		countx=0
		for i in self.room:
			county=0
			for j in i:
				if key==j:
					found=True
					break;
				else:
					county+=1
			if not found:
				countx+=1
			else:
				break;
		return [countx,county]

	def draw(self):
		self.screen.fill((0,0,0))

		player=self.player.coords
		monster=self.monster.coords

		if self.player.last=="l":
			sq=[player[0]-1,player[1]]
			self.screen.blit(pygame.transform.rotate(IMG_LIGHT,180),((player[0]-1)*64,player[1]*64,0,0))
		elif self.player.last=="r":
			sq=[player[0]+1,player[1]]
			self.screen.blit(IMG_LIGHT,(player[0]*64,player[1]*64,0,0))
		elif self.player.last=="u":
			sq=[player[0],player[1]-1]
			self.screen.blit(pygame.transform.rotate(IMG_LIGHT,90),(player[0]*64,(player[1]-1)*64,0,0))
		else:
			sq=[player[0],player[1]+1]
			self.screen.blit(pygame.transform.rotate(IMG_LIGHT,270),(player[0]*64,player[1]*64,0,0))

		for x in range(WIDTH):
			for y in range(HEIGHT):
				if self.room[x][y]=="P1":
					self.screen.blit(IMG_PILLAR1,(x*64,y*64,0,0))
				elif self.room[x][y]=="P2":
					self.screen.blit(IMG_PILLAR2,(x*64,y*64,0,0))
				elif self.room[x][y]=="P3":
					self.screen.blit(IMG_PILLAR3,(x*64,y*64,0,0))
				elif self.room[x][y]=="P4":
					self.screen.blit(IMG_PILLAR4,(x*64,y*64,0,0))



		try:
			obj=self.room[player[0]][player[1]]
			if obj=="T":
				self.screen.blit(IMG_TRAP,(player[0]*64,player[1]*64,0,0))
			elif obj=="W":
				self.screen.blit(IMG_TREASURE,(player[0]*64,player[1]*64,0,0))
			elif obj=="A":
				self.screen.blit(IMG_ACTIVATED,(player[0]*64,player[1]*64,0,0))
		except IndexError:
			pass

		try:
			obj=self.room[sq[0]][sq[1]]
			if obj=="T":
				self.screen.blit(IMG_TRAP,(sq[0]*64,sq[1]*64,0,0))
			elif obj=="W":
				self.screen.blit(IMG_TREASURE,(sq[0]*64,sq[1]*64,0,0))
			elif obj=="A":
				self.screen.blit(IMG_ACTIVATED,(sq[0]*64,sq[1]*64,0,0))
			elif obj=="P1":
				self.screen.blit(IMG_PILLAR1L,(sq[0]*64,sq[1]*64,0,0))
			elif obj=="P2":
				self.screen.blit(IMG_PILLAR2L,(sq[0]*64,sq[1]*64,0,0))
			elif obj=="P3":
				self.screen.blit(IMG_PILLAR3L,(sq[0]*64,sq[1]*64,0,0))
			elif obj=="P4":
				self.screen.blit(IMG_PILLAR4L,(sq[0]*64,sq[1]*64,0,0))
		except IndexError:
			pass

		if monster[0]==sq[0] and monster[1]==sq[1]:
			self.screen.blit(self.monster.getImage(),(monster[0]*64,monster[1]*64,0,0))
		else:
			self.screen.blit(self.monster.getEyesImage(),(monster[0]*64,monster[1]*64,0,0))

		self.screen.blit(self.player.getImage(),(player[0]*64,player[1]*64,0,0))			

		tint(self.screen,[255-random.random()*30]*3)

		pygame.display.flip()

game=Game()
game.loop()