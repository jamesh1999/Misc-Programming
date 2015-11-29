import pygame, random, time
from pygame.locals import *

pygame.init()
screen = pygame.display.set_mode((640,640))
width,height=screen.get_size()
white = (255,255,255)

DELTA_T = 0
VERTICES = ((320, 20), (20,620), (620, 620))

pygame.draw.line(screen, white, VERTICES[0], VERTICES[1])
pygame.draw.line(screen, white, VERTICES[1], VERTICES[2])
pygame.draw.line(screen, white, VERTICES[2], VERTICES[0])
pygame.display.flip()

done = False
previous = VERTICES[0]
cnt = 0

while not done:
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			done = True
			break;
		if event.type == pygame.KEYDOWN:
			if event.key == K_ESCAPE:
				done = True
				break;
	
	choice = random.randrange(3)
	previous = (int((VERTICES[choice][0] + previous[0]) / 2), int((VERTICES[choice][1] + previous[1]) / 2))
	screen.set_at(previous, white)

	pygame.display.flip()

	time.sleep(DELTA_T)
	cnt += 1