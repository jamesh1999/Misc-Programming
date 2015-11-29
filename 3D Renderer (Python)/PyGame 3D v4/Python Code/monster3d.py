import random
from Py3D import *

WIDTH = 8
HEIGHT = 6
IMG_TRAP=pygame.image.load("trap.png")
IMG_TREASURE=pygame.image.load("treasure.png")
IMG_PLAYER=pygame.image.load("player.png")
IMG_MONSTER=pygame.image.load("monster.png")
IMG_BLANK=pygame.image.load("blank.png")
room=[]
for i in range(WIDTH):
	col=[]
	for j in range(HEIGHT):
		col.append("")
	room.append(col)

monster_found=False

def getFreeSquare(mark):
	global room
	while True:
		x=random.randint(0,WIDTH)
		y=random.randint(0,HEIGHT)
		if room[x][y]=="":
			break;
	room[x][y]=mark
	return [x,y]

def getCoords(key):
	found=False
	countx=0
	for i in room:
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

class Player(ObjectBase):
	def __init__(self,*args):
		self.coords=[0,0]
		room[0][0]="P"
		ObjectBase.__init__(self,*args)
	def update(self):
		room[self.coords[0]][self.coords[1]]=""
		moved=False
		while not moved:
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
							moved=True
					elif event.key == K_LEFT:
						if not self.coords[0]==0:
							self.coords[0]-=1
							moved=True
					elif event.key == K_UP:
						if not self.coords[1]==0:
							self.coords[1]-=1
							moved=True
					elif event.key == K_DOWN:
						if not self.coords[1]>=HEIGHT-1:
							self.coords[1]+=1
							moved=True
		room[self.coords[0]][self.coords[1]]="P"

class Monster(ObjectBase):
	def __init__(self,*args):
		self.coords=getFreeSquare("M")
		self.monster_moves=1
		ObjectBase.__init__(self,*args)
	def update(self):
		global room
		global monster_found
		if monster_found:
			new_coords=self.coords[:]
			player=getCoords("P")
			distx=player[0]-self.coords[0]
			disty=player[1]-self.coords[1]
			if distx**2>=disty**2:
				if distx<0:
					new_coords[0]-=1
				elif distx>0:
					new_coords[0]+=1
			else:
				if disty<0:
					new_coords[1]-=1
				elif disty>0:
					new_coords[1]+=1
			for move in range(self.monster_moves-1):
				distx=player[0]-new_coords[0]
				disty=player[1]-new_coords[1]
				if distx**2>=disty**2:
					if distx<0:
						new_coords[0]-=1
					elif distx>0:
						new_coords[0]+=1
				else:
					if disty<0:
						new_coords[1]-=1
					elif disty>0:
						new_coords[1]+=1
			room[self.coords[0]][self.coords[1]]=""
			room[new_coords[0]][new_coords[1]]="M"
		if player==self.coords and not .monster_found:
			monster_found=True
			print("You have woken up the monster")
		elif player==self.coords and monster_found:
			print("The monster killed you")

class Win(ObjectBase):
	def __init__(self,*args):
		self.coords=getFreeSquare("W")
		ObjectBase.__init__(self,*args)
	def update(self):
		if getCoords("P")==self.coords:
			print("You found the treasure!")
			self.done=True

class Trap(ObjectBase):
	def __init__(self,*args):
		self.coords=getFreeSquare("T")
		ObjectBase.__init__(self,*args)
	def update(self):
		global monster_found:
		if getCoords("P")==self.coords:
			print("You activated a trap hidden on the floor and hear a loud groaning sound nearby")
			monster_found=True

class Game(object):
	def __init__(self):
		self.player=Player()
		self.monster_found=False
		room=[]
		for i in range(WIDTH):
			col=[]
			for j in range(HEIGHT):
				col.append("")
			room.append(col)
		self.room=room
		self.done=False
		self.screen=pygame.display.set_mode((WIDTH*64,HEIGHT*64))
		self.monster_moves=1
		self.start()

	def loop(self):
		while not self.done:
			self.update()
			self.draw()

	def draw(self):
		self.screen.fill((0,0,0))

		player=self.player.coords
		temproom=[]
		for i in self.room: temproom.append(i[:])
		if temproom[player[0]][player[1]]=="":
			temproom[player[0]][player[1]]="P"

		x=0
		for i in temproom:
			y=0
			for j in i:
				image=None
				if j=="":
					image=IMG_BLANK
				elif j=="P":
					image=IMG_PLAYER
				elif j=="M":
					image=IMG_MONSTER
				elif j=="T":
					image=IMG_TRAP
				elif j=="W":
					image=IMG_TREASURE
				self.screen.blit(image,(x,y,x+64,y+64))
				y+=64
			x+=64			

		pygame.display.flip()

def main():
	game=Game(fov=75,ticks_per_second=60,max_fps=0,
                 update_callback=None,frame_callback=None,time_source=None,
                 min_brightness=10,min_colour=white, debug=False,
                  caption="Py3D Game", light_distance=4.4)
	player=Player()
	player_mesh=SubObjectBase()
	player_mesh.add_nodes(((0,0,0),(0,0,0.25),))

main()

pygame.quit()