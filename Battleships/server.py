from GameSocket import *

s=GameSocket(is_server=True)
print("Address: "+s.getName()[0]+":"+str(s.getName()[1]))

while True:
	try:
		s.acceptConnections(2)
		print("Making match: "+str(s.peers[0][1])+" "+str(s.peers[1][1]))
		#Assign player numbers
		s.send(1,peer=s.peers[0])
		s.send(2,peer=s.peers[1])

		#Create a direct connection
		data=s.receive(peer=s.peers[0])
		s.send(data,peer=s.peers[1])
		result=int(s.receive(peer=s.peers[1]))
		if result==1:
			print("Match made.")
		else:
			print("Clients failed to connect.")


		print("Terminating connections.")
		s.disconnect(peer=s.peers[0])
		s.disconnect(peer=s.peers[1])
		s.removePeers()
	except Exception as e:
		print("\n"+str(e)+"\n")
		print("Notifying active peers of the error.")
		for peer in s.peers:
			try:
				s.send(0,peer=peer)
			except:
				print(str(peer[1])+" lost connection.")
		print("Removing peer connections...")
		s.removePeers()
