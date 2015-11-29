import time,math,random
import pygame
from pygame.locals import *

particles=[]
GRAVITY=0.01
BOUNCE=0.75
WIDTH,HEIGHT=500,500
screen=pygame.display.set_mode((WIDTH,HEIGHT))#,pygame.FULLSCREEN)
WIDTH,HEIGHT=screen.get_size()[0],screen.get_size()[1]
DRAG_COEFFICIENT=0.47
AIR_MASS=0.000006

NUM_PARTICLES=16

class Particle(object):
	def __init__(self,mass,radius,pos):
		self.pos=pos
		self.velocity=[0,0]
		self.mass=mass
		self.radius=radius
		self.drag = DRAG_COEFFICIENT*AIR_MASS*self.radius**2*math.pi*0.5

	def update(self):	
		self.velocity[1]+=GRAVITY

		self.velocity[0]-=max(min(self.drag*self.velocity[0]**2*(-1 if self.velocity[0]<0 else 1),WIDTH),-WIDTH)
		self.velocity[1]-=max(min(self.drag*self.velocity[1]**2*(-1 if self.velocity[1]<0 else 1),HEIGHT),-HEIGHT)

		if self.pos[0]+self.radius>WIDTH:
			self.pos[0]=2*(WIDTH-self.radius)-self.pos[0]
			if self.velocity[0]>0:
				self.velocity[0]*=-BOUNCE
		if self.pos[0]-self.radius<0:
			self.pos[0]=2*self.radius-self.pos[0]
			if self.velocity[0]<0:
				self.velocity[0]*=-BOUNCE
		if self.pos[1]+self.radius>HEIGHT:
			self.pos[1]=2*(HEIGHT-self.radius)-self.pos[1]
			if self.velocity[1]>0:
				self.velocity[1]*=-BOUNCE
		if self.pos[1]-self.radius<0:
			self.pos[1]=2*self.radius-self.pos[1]
			if self.velocity[1]<0:
				self.velocity[1]*=-BOUNCE

		if not self.velocity[0]:
			self.velocity[0]+=0.000001
		if not self.velocity[1]:
			self.velocity[1]+=0.000001

	def move(self):
		self.pos[0]+=self.velocity[0]
		self.pos[1]+=self.velocity[1]

for i in range(NUM_PARTICLES):
	radius=random.randint(10,20)
	while True:
		pos=[random.randint(0,WIDTH-radius),random.randint(0,HEIGHT-radius)]
		for i in particles:
			if math.hypot(pos[0]-i.pos[0],pos[1]-i.pos[1])<radius+i.radius:
				break
		else:
			break
	density=random.randint(7,15)
	particles.append(Particle(density*radius**2,radius,pos))

def collide(particlea,particleb):

	dx=particlea.pos[0]-particleb.pos[0]
	dy=particlea.pos[1]-particleb.pos[1]
	if math.hypot(dx,dy)<particleb.radius+particlea.radius:
		
		angle=math.atan2(dy,dx)+0.5*math.pi

		#Calculate speed/angle for both particles
		aspeed=math.hypot(particlea.velocity[0],particlea.velocity[1])
		atan=math.atan(particlea.velocity[1]/particlea.velocity[0])
		if particlea.velocity[0]<0:
			if particlea.velocity[1]>=0:
				aangle=1.5*math.pi-atan
			else:
				aangle=1.5*math.pi+atan
		else:
			if particlea.velocity[1]>=0:
				aangle=0.5*math.pi+atan
			else:
				aangle=0.5*math.pi-atan

		bspeed=math.hypot(particleb.velocity[0],particleb.velocity[1])
		btan=math.atan(particleb.velocity[1]/particleb.velocity[0])
		if particleb.velocity[0]<0:
			if particleb.velocity[1]>=0:
				bangle=1.5*math.pi-btan
			else:
				bangle=1.5*math.pi+btan
		else:
			if particleb.velocity[1]>=0:
				bangle=0.5*math.pi+btan
			else:
				bangle=0.5*math.pi-btan

		total_mass = particlea.mass + particlea.mass

		#Workout x&y velocities
		particlea.velocity[0]=((aspeed*math.cos(aangle-angle)*(particlea.mass-particleb.mass)+2*particleb.mass*bspeed*math.cos(bangle-angle))/total_mass)*math.cos(angle)+aspeed*math.sin(aangle-angle)*math.cos(angle+math.pi/2)
		particlea.velocity[1]=((aspeed*math.cos(aangle-angle)*(particlea.mass-particleb.mass)+2*particleb.mass*bspeed*math.cos(bangle-angle))/total_mass)*math.sin(angle)+aspeed*math.sin(aangle-angle)*math.sin(angle+math.pi/2)
		particlea.velocity[0]*=BOUNCE
		particlea.velocity[1]*=BOUNCE

		particleb.velocity[0]=((bspeed*math.cos(bangle-angle)*(particleb.mass-particlea.mass)+2*particlea.mass*aspeed*math.cos(aangle-angle))/total_mass)*math.cos(angle)+bspeed*math.sin(bangle-angle)*math.cos(angle+math.pi/2)
		particleb.velocity[1]=((bspeed*math.cos(bangle-angle)*(particleb.mass-particlea.mass)+2*particlea.mass*aspeed*math.cos(aangle-angle))/total_mass)*math.sin(angle)+bspeed*math.sin(bangle-angle)*math.sin(angle+math.pi/2)
		particleb.velocity[0]*=BOUNCE
		particleb.velocity[1]*=BOUNCE

		#Unstick particles
		overlap = 0.5*(particlea.radius + particleb.radius - math.hypot(dx,dy)+1)
		particlea.pos[0] += math.sin(angle)*overlap
		particlea.pos[1] -= math.cos(angle)*overlap
		particleb.pos[0] -= math.sin(angle)*overlap
		particleb.pos[1] += math.cos(angle)*overlap

cnt=0
while True:
	cnt%=4000
	if not cnt:
		particles.pop(0)
		radius=random.randint(10,20)
		while True:
			pos=[random.randint(0,WIDTH-radius),random.randint(0,HEIGHT-radius)]
			for i in particles:
				if math.hypot(pos[0]-i.pos[0],pos[1]-i.pos[1])<radius+i.radius:
					break
			else:
				break
		density=random.randint(7,15)
		particles.append(Particle(density*radius**2,radius,pos))
	for i in particles:
		i.update()
	for i,particle in enumerate(particles):
		for particle1 in particles[i+1:]:
			collide(particle,particle1)
	for i in particles:
		i.move()
	screen.fill((0,0,0))
	for i in particles:
		pygame.draw.circle(screen, (255,255,255), (int(max(min(i.pos[0],WIDTH+i.radius),0-i.radius)),int(max(min(i.pos[1],HEIGHT+i.radius),0-i.radius))), i.radius, 0)
	time.sleep(0.002)
	pygame.display.flip()
	cnt+=1