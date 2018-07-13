from GameSocket import *
import random,os,time,sys

PORT=5564
addr=input("Please enter the server address: ")
s=GameSocket()

connected=False
cnt=0
while not connected:
	if cnt==5:
		print("Something is wrong with this connection. Please try a different address.")
		addr=input("Address: ")
		cnt=0
	try:

		s.connect(addr.split(":")[0],port=int(addr.split(":")[1]))

		print("Waiting to be assigned a number")
		num=int(s.receive())
		print("Received number!")

		if not num:
			print("There was an error with the connection.")
			print("Reconnecting...")
			continue;
		elif num==1:
			print("Getting direct connection")
			peer=GameSocket(is_server=True,port=PORT)
			data=peer.getName()[0]+":"+str(peer.getName()[1])
			s.send(data)
			peer.acceptConnections(1)
			peer.send(1)
		else:
			print("Getting direct connection")
			peer=GameSocket()
			data=s.receive()
			peer.connect(data.split(":")[0],port=int(data.split(":")[1]))
			s.send(peer.receive())
		s.disconnect()

		print("Connection acquired")
		connected=True

	except:
		print("There was an error with the connection.")
		print("Reconnecting...")

	cnt+=1

guesses=[]
for i in range(10):
	row=[]
	for j in range(10):
		row.append(" ")
	guesses.append(row)

ships=[]
for i in range(10):
	row=[]
	for j in range(10):
		row.append(" ")
	ships.append(row)

won=False
done=False

def add_ship(size,tag):
	fin=False
	while not fin:
		x=random.randrange(10)
		y=random.randrange(10)
		moves=[]
		for move in [(0,1),(1,0),(0,-1),(-1,0)]:
			possible=True
			for i in range(size):
				try:
					if not ships[y+(i*move[1])][x+(i*move[0])]==" " or not y+(i*move[1])>=0 or not x+(i*move[0])>=0:
						possible=False
				except:
					possible=False
			if possible:
				moves.append(move)
		if not moves==[]:
			move=random.choice(moves)
			fin=True
			for i in range(size):
				ships[y+(move[1]*i)][x+(move[0]*i)]=tag

def print_board():
	if sys.platform=="win32" or sys.platform=="cygwin":
		os.system('cls')
	elif sys.platform=="darwin" or sys.platform.startswith("linux"):
		os.system('clear')
	else:
		print("\n\n\n\n\n")
	print("   A  B  C  D  E  F  G  H  I  J   ")
	for i in range(10):
		if not i==9:
			print(" "+str(i+1),end=" ")
		else:
			print(str(i+1),end=" ")
		for j in range(10):
			print(guesses[i][j]+" ",end=" ")
		print(" ")
	print("\n\n\n")
	print("   A  B  C  D  E  F  G  H  I  J   ")
	for i in range(10):
		if not i==9:
			print(" "+str(i+1),end=" ")
		else:
			print(str(i+1),end=" ")
		for j in range(10):
			if ships[i][j] in tags:
				char="X"
			else:
				char=ships[i][j]
			print(char+" ",end=" ")
		print(" ")

def guess():
	global done
	global won
	time.sleep(0.6)
	print_board()
	fin=False
	while not fin:
		try:
			choice=input("Where do you want to fire? ")
			choice=choice.lower()
			x=choice[0]
			y=int(choice[1:])
			if x=="a":
				x=0
			elif x=="b":
				x=1
			elif x=="c":
				x=2
			elif x=="d":
				x=3
			elif x=="e":
				x=4
			elif x=="f":
				x=5
			elif x=="g":
				x=6
			elif x=="h":
				x=7
			elif x=="i":
				x=8
			elif x=="j":
				x=9
			else:
				break;
			if y>10:
				break;
			else:
				y-=1
			peer.send(str(x)+" "+str(y))
			data=peer.receive()
			if int(data)==0:
				print("Miss!")
				guesses[y][x]="."
			elif int(data)==1:
				print("Hit!")
				guesses[y][x]="X"
				guess()
			elif int(data)==2:
				print("Sunk!")
				guesses[y][x]="X"
				guess()
			elif int(data)==3:
				print("Sunk!")
				guesses[y][x]="X"
				won=True
				done=True
			else:
				break;
			fin=True
		except:
			pass

def opponent_move():
	global done
	time.sleep(0.6)
	print_board()
	data=peer.receive()
	x=int(data.split()[0])
	y=int(data.split()[1])
	if ships[y][x]==" " or ships[y][x]=="O":
		peer.send(0)
	else:
		tag=ships[y][x]
		ships[y][x]="O"
		sunk=True
		for x in range(10):
			for y in range(10):
				if ships[y][x]==tag:
					sunk=False
		if sunk:
			print("Your ship has been sunk!")
			ship=False
			for x in range(10):
				for y in range(10):
					if ships[y][x] in tags:
						ship=True
			if not ship:
				done=True
				peer.send(3)
			else:
				peer.send(2)
				opponent_move()
		else:
			peer.send(1)
			print("Your ship was hit!")
			opponent_move()

tags=["a","b","c","d","e"]
add_ship(2,"a")
add_ship(3,"b")
add_ship(3,"c")
add_ship(4,"d")
add_ship(5,"e")

while not done:
	if num==1:
		guess()
		if done:
			break;
		opponent_move()
		if done:
			break;
	else:
		opponent_move()
		if done:
			break;
		guess()
		if done:
			break;

peer.disconnect()
print_board()
if won:
	print("Well done! You have won.")
else:
	print("You have lost. Better luck next time.")
