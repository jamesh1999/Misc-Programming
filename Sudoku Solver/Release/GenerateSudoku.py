import pygame, sys
from pygame.locals import *

pygame.init()

screen = pygame.display.set_mode((450,450))
font = pygame.font.SysFont("monospace", 40)

white = (255,255,255)
black = (0,0,0)
green = (0,255,0)

sudoku = [[0,0,0,0,0,0,0,0,0],
			[0,0,0,0,0,0,0,0,0],
			[0,0,0,0,0,0,0,0,0],
			[0,0,0,0,0,0,0,0,0],
			[0,0,0,0,0,0,0,0,0],
			[0,0,0,0,0,0,0,0,0],
			[0,0,0,0,0,0,0,0,0],
			[0,0,0,0,0,0,0,0,0],
			[0,0,0,0,0,0,0,0,0]]

selected = [0,0]



def drawBoard(board):
	screen.fill(black)
	pygame.draw.rect(screen, green, (selected[0] * 50, selected[1] * 50, 50, 50))

	for i in range(8):
		i += 1

		if i % 3:
			pygame.draw.line(screen, white, (50 * i, 0), (50 * i, 450))
		else:
			pygame.draw.line(screen, white, (50 * i, 0), (50 * i, 450), 5)

	for i in range(8):
		i += 1

		if i % 3:
			pygame.draw.line(screen, white, (0, 50 * i), (450, 50 * i))
		else:
			pygame.draw.line(screen, white, (0, 50 * i), (450, 50 * i), 5)

	for i in range(9):
		for j in range(9):
			val = sudoku[j][i]

			if val:
				rendered = font.render(str(val), 1, white)
				screen.blit(rendered, ((i * 50) + 10, (j * 50) + 5))

	pygame.display.flip()



done = False

with open(sys.argv[1], "r") as infile:
	for y,line in enumerate(infile.readlines()):
		for x,c in enumerate(line):
			if c == ' ':
				c = '0'
			try:
				c = int(c)
				sudoku[y][x] = c
			except:
				pass
	infile.close()

while not done:
	drawBoard(sudoku)

	for event in pygame.event.get():
		if event.type == pygame.MOUSEBUTTONDOWN:
			pos = pygame.mouse.get_pos()
			selected = [pos[0] // 50, pos[1] // 50]

		if event.type == pygame.KEYDOWN:
			if event.key == K_0:
				sudoku[selected[1]][selected[0]] = 0
			elif event.key == K_1:
				sudoku[selected[1]][selected[0]] = 1
			elif event.key == K_2:
				sudoku[selected[1]][selected[0]] = 2
			elif event.key == K_3:
				sudoku[selected[1]][selected[0]] = 3
			elif event.key == K_4:
				sudoku[selected[1]][selected[0]] = 4
			elif event.key == K_5:
				sudoku[selected[1]][selected[0]] = 5
			elif event.key == K_6:
				sudoku[selected[1]][selected[0]] = 6
			elif event.key == K_7:
				sudoku[selected[1]][selected[0]] = 7
			elif event.key == K_8:
				sudoku[selected[1]][selected[0]] = 8
			elif event.key == K_9:
				sudoku[selected[1]][selected[0]] = 9
			elif event.key == K_RETURN:
				done = True
			elif event.key == K_BACKSPACE:
				sudoku = [[0,0,0,0,0,0,0,0,0],
							[0,0,0,0,0,0,0,0,0],
							[0,0,0,0,0,0,0,0,0],
							[0,0,0,0,0,0,0,0,0],
							[0,0,0,0,0,0,0,0,0],
							[0,0,0,0,0,0,0,0,0],
							[0,0,0,0,0,0,0,0,0],
							[0,0,0,0,0,0,0,0,0],
							[0,0,0,0,0,0,0,0,0]]

with open(sys.argv[1], "w") as outfile:
	for i in range(9):
		string = ""
		for j in range(9):
			if sudoku[i][j] == 0:
				string += " "
			else:
				string += str(sudoku[i][j])

		string += "\n"

		outfile.write(string)

	outfile.close()

pygame.quit()