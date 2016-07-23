board = [
		[" "," "," "," "," "," "," "," "," "," "," "," "," "],
		[" "," "," "," ","c","c","c"," "," "," "," "," "," "],
		[" "," "," "," "," "," ","c"," "," "," "," "," "," "],
		[" "," "," "," "," "," "," "," "," "," "," "," "," "],
		[" "," "," "," "," "," "," "," "," "," "," "," "," "],
		[" "," "," "," "," "," "," "," "," "," "," "," "," "],
		[" "," "," "," "," "," "," "," "," "," "," "," "," "],
		[" "," "," "," "," "," "," "," "," "," "," "," "," "],
		[" "," "," "," "," "," "," "," "," "," "," "," "," "],
		[" "," "," "," "," "," "," "," "," "," "," "," "," "],
		[" "," "," "," "," "," "," "," "," "," "," "," "," "]]

dist = int(input("How far do you want to go? ")) - 1

MAX_DEPTH = 16

def copy(board):
	nboard = []
	for i in board:
		row = []
		for j in i:
			row.append(j)
		nboard.append(row)

	return nboard

def getPreviousBoards(board):
	valid = []

	for y,row in enumerate(board):
		for x,item in enumerate(row):
			if item == "c":
				try:
					if board[y+1][x] == " " and board[y+2][x] == " ":
						valid.append(copy(board))
						valid[-1][y+1][x] = "c"
						valid[-1][y+2][x] = "c"
						valid[-1][y][x] = " "
						continue;
				except:
					pass

	for y,row in enumerate(board):
		for x,item in enumerate(row):
			if item == "c":
				try:
					if board[y][x-1] == " " and board[y][x-2] == " ":
						valid.append(copy(board))
						valid[-1][y][x-1] = "c"
						valid[-1][y][x-2] = "c"
						valid[-1][y][x] = " "
				except:
					pass

				try:
					if board[y][x+1] == " " and board[y][x+2] == " ":
						valid.append(copy(board))
						valid[-1][y][x+1] = "c"
						valid[-1][y][x+2] = "c"
						valid[-1][y][x] = " "
				except:
					pass

	return valid

def isComplete(board):
	for y in range(dist):
		for x in board[y]:
			if not x == " ":
				break;
		else:
			continue;
		break;
	else:
		return True
	return False

min_counters = MAX_DEPTH
min_board = []

def computeBranch(board, depth):
	global min_counters
	global min_board
	if depth < min_counters:
		if isComplete(board):
			min_counters = depth
			min_board = board

		for nboard in getPreviousBoards(board):
			computeBranch(nboard, depth + 1)

computeBranch(board, 0)

print("Counters used: ", min_counters + 4)
print("Board: \n\n")
for row in min_board:
	print('|'.join(row))
	print("-------------------------")