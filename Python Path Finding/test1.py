import copy
import pygame
from pygame.locals import *

import astar

MOVE_DIAGONAL = True
BOARD_SIZE = (32, 32)
SCREEN_SIZE = (640, 640)

BOARD = []
row = [' '] * BOARD_SIZE[1]
for i in range(BOARD_SIZE[0]):
	BOARD.append(copy.copy(row))


pygame.init()

screen = pygame.display.set_mode((SCREEN_SIZE[0], SCREEN_SIZE[1]))

def drawBoard():
	screen.fill((0,0,0))

	#Draw walls
	for i in range(BOARD_SIZE[0]):
		for j in range(BOARD_SIZE[1]):
			if BOARD[i][j] != ' ':
				if BOARD[i][j] == '#':
					color = (0, 255, 255)
				elif BOARD[i][j] == 'S':
					color = (0, 255, 0)
				elif BOARD[i][j] == 'E':
					color = (255, 0, 0)


				left = i * (SCREEN_SIZE[0] // BOARD_SIZE[0])
				top = j * (SCREEN_SIZE[1] // BOARD_SIZE[1])
				width = (SCREEN_SIZE[0] // BOARD_SIZE[0])
				height = (SCREEN_SIZE[1] // BOARD_SIZE[1])
				pygame.draw.rect(screen, color, pygame.Rect(left, top, width, height))

	#Draw grid
	for i in range(1, BOARD_SIZE[0]):
		x_coord = i * (SCREEN_SIZE[0] // BOARD_SIZE[0])
		pygame.draw.line(screen, (255, 255, 255), (x_coord, 0), (x_coord, SCREEN_SIZE[1]))

	for i in range(1, BOARD_SIZE[1]):
		y_coord = i * (SCREEN_SIZE[1] // BOARD_SIZE[1])
		pygame.draw.line(screen, (255, 255, 255), (0, y_coord), (SCREEN_SIZE[0], y_coord))

#Set up walls
finished = False
exit = False
while not finished and not exit:

	for event in pygame.event.get():
		if event.type == QUIT:
			finished = True
			exit = True

		if event.type == KEYDOWN:
			if event.key == K_SPACE:
				finished = True

	#Create wall
	if pygame.mouse.get_pressed()[0]:
		pos = pygame.mouse.get_pos()
		BOARD[pos[0] // (SCREEN_SIZE[0] // BOARD_SIZE[0])][pos[1] // (SCREEN_SIZE[1] // BOARD_SIZE[1])] = '#'

	#Remove wall
	if pygame.mouse.get_pressed()[2]:
		pos = pygame.mouse.get_pos()
		BOARD[pos[0] // (SCREEN_SIZE[0] // BOARD_SIZE[0])][pos[1] // (SCREEN_SIZE[1] // BOARD_SIZE[1])] = ' '

	drawBoard()
	pygame.display.flip()

#Choose start
finished = False
while not finished and not exit:

	for event in pygame.event.get():
		if event.type == QUIT:
			finished = True
			exit = True

		#Select start
		elif event.type == MOUSEBUTTONDOWN:
			pos = pygame.mouse.get_pos()
			BOARD[pos[0] // (SCREEN_SIZE[0] // BOARD_SIZE[0])][pos[1] // (SCREEN_SIZE[1] // BOARD_SIZE[1])] = 'S'
			finished = True

	drawBoard()
	pygame.display.flip()

#Choose end
finished = False
while not finished and not exit:

	for event in pygame.event.get():
		if event.type == QUIT:
			finished = True
			exit = True

		#Select end
		elif event.type == MOUSEBUTTONDOWN:
			pos = pygame.mouse.get_pos()
			BOARD[pos[0] // (SCREEN_SIZE[0] // BOARD_SIZE[0])][pos[1] // (SCREEN_SIZE[1] // BOARD_SIZE[1])] = 'E'
			finished = True

	drawBoard()
	pygame.display.flip()

#Construct nodes
nodes = []
for x in range(BOARD_SIZE[0]):
	for y in range(BOARD_SIZE[1]):
		nodes.append(astar.Node())
		nodes[-1].position[0] = int((x + 0.5) * (SCREEN_SIZE[0] // BOARD_SIZE[0]))
		nodes[-1].position[1] = int((y + 0.5) * (SCREEN_SIZE[1] // BOARD_SIZE[1]))

		#Connect nodes
		if x > 0:
			connected = nodes[(x - 1) * BOARD_SIZE[0] + y]
			connected.connections.append(nodes[-1])
			nodes[-1].connections.append(connected)

		if y > 0:
			connected = nodes[-2]
			connected.connections.append(nodes[-1])
			nodes[-1].connections.append(connected)

		#Diagonal connections
		if MOVE_DIAGONAL:
			if x > 0 and y > 0:
				connected = nodes[(x - 1) * BOARD_SIZE[0] + y - 1]
				connected.connections.append(nodes[-1])
				nodes[-1].connections.append(connected)

			if x > 0 and y < BOARD_SIZE[1] - 1:
				connected = nodes[(x - 1) * BOARD_SIZE[0] + y + 1]
				connected.connections.append(nodes[-1])
				nodes[-1].connections.append(connected)

#Find start and end nodes
start = None
goal = None
for x in range(BOARD_SIZE[0]):
	for y in range(BOARD_SIZE[1]):
		if BOARD[x][y] == 'S':
			start = nodes[x * BOARD_SIZE[0] + y]
		elif BOARD[x][y] == 'E':
			goal = nodes[x * BOARD_SIZE[0] + y]

#Remove wall nodes
for x in range(BOARD_SIZE[0]):
	for y in range(BOARD_SIZE[1]):
		if BOARD[x][y] == '#':
			nodes[x * BOARD_SIZE[0] + y] = None
while None in nodes:
	nodes.remove(None)

#Remove invalid connections
for node in nodes:
	for connection in copy.copy(node.connections):
		if not connection in nodes:
			node.connections.remove(connection)

start_id = nodes.index(start)
goal_id = nodes.index(goal)

astar.initSearch(nodes, start_id, goal_id)

def drawNodeOverlay():
	for node in nodes:
		for connection in node.connections:
			pygame.draw.line(screen, (255, 255, 255), node.position, connection.position)

	for node in nodes:
		if node.is_searched:
			pygame.draw.circle(screen, (255, 255, 0), node.position, 4)
		else:
			pygame.draw.circle(screen, (255, 255, 255), node.position, 2)

drawBoard()
drawNodeOverlay()
pygame.display.flip()

#Search
finished = False
while not finished and not exit:

	for event in pygame.event.get():
		if event.type == QUIT:
			finished = True
			exit = True

	if astar.searchStep(nodes, start_id, goal_id):
		finished = True
	drawBoard()
	drawNodeOverlay()
	pygame.display.flip()

#Display path
drawBoard()
drawNodeOverlay()

path = astar.getPath(nodes, start_id, goal_id)

for i in range(len(path) - 1):
	pygame.draw.line(screen, (255, 0, 0), path[i].position, path[i + 1].position, 3)
pygame.display.flip()

#Wait for exit
while not exit:

	for event in pygame.event.get():
		if event.type == QUIT:
			exit = True