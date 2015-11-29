import socket

DEFAULT_PORT=6661

class GameSocket(object):

	peers=[]

	def __init__(self,s=None,is_server=False,port=None):
		if s==None:
			s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.s=s
		if is_server:
			if port==None:
				port=DEFAULT_PORT
			self.s.bind((socket.gethostname(),port))
			self.port=port
		self.is_server=is_server

	def connect(self,addr,port=None):
		if not self.is_server:
			if not port==None:
				self.s.connect((addr,port))
			else:
				self.s.connect((addr,DEFAULT_PORT))
		else:
			print("This socket is a server socket it can't connect to others.")

	def disconnect(self,peer=None):
		if not self.is_server:
			self.s.close()
		else:
			if not peer==None:
				peer[0].close()
			else:
				self.peers[0][0].close()

	def acceptConnections(self,num):
		self.s.listen(5)
		while True:
			if not num:
				break;
			else:
				peer,p_addr = self.s.accept()
				self.peers.append((peer,p_addr))
				num-=1

	def send(self,data,peer=None):
		if not peer==None:
			peer[0].send(str(data).encode())
		elif self.is_server:
			self.peers[0][0].send(str(data).encode())
		else:
			self.s.send(str(data).encode())

	def receive(self,peer=None,buff=1024):
		if not peer==None:
			data=peer[0].recv(buff)
			return data.decode()
		elif self.is_server:
			data=self.peers[0][0].recv(buff)
			return data.decode()
		else:
			data=self.s.recv(buff)
			return data.decode()

	def removePeers(self):
		self.peers=[]

	def getName(self):
		return self.s.getsockname()