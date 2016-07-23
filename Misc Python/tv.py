#!  /usr/bin/python
#   Title: samsungremote.py
#   Author: Asif Iqbal
#   Date: 05APR2012
#   Info: To send remote control commands to the Samsung tv over LAN
#   TODO:

import socket
import base64
import time, datetime

#IP Address of TV
tvip = b"192.168.0.6"
#IP Address of TV
myip = b"192.168.0.8"
#Used for the access control/validation, but not after that AFAIK
mymac = b"90-2b-34-d6-09-4b"
#What the iPhone app reports
appstring = b"iphone..iapp.samsung"
#Might need changing to match your TV type
tvappstring = b"iphone.UE40D8000.iapp.samsung"
#What gets reported when it asks for permission
remotename = b"Python Samsung Remote"

# Function to send keys
def sendKey(skey, dataSock, appstring):
	messagepart3 = chr(0x00).encode() + chr(0x00).encode() + chr(0x00).encode() + chr(len(base64.b64encode(skey))).encode() + chr(0x00).encode() + base64.b64encode(skey)
	part3 = chr(0x00).encode() + chr(len(appstring)).encode() + chr(0x00).encode() + appstring + chr(len(messagepart3)).encode() + chr(0x00).encode() + messagepart3
	dataSock.send(part3);

# Open Socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((tvip, 55000))

# Key Reference
# Normal remote keys
 #KEY_0
 #KEY_1
 #KEY_2
 #KEY_3
 #KEY_4
 #KEY_5
 #KEY_6
 #KEY_7
 #KEY_8
 #KEY_9
 #KEY_UP
 #KEY_DOWN
 #KEY_LEFT
 #KEY_RIGHT
 #KEY_MENU
 #KEY_PRECH
 #KEY_GUIDE
 #KEY_INFO
 #KEY_RETURN
 #KEY_CH_LIST
 #KEY_EXIT
 #KEY_ENTER
 #KEY_SOURCE
 #KEY_AD #KEY_PLAY
 #KEY_PAUSE
 #KEY_MUTE
 #KEY_PICTURE_SIZE
 #KEY_VOLUP
 #KEY_VOLDOWN
 #KEY_TOOLS
 #KEY_POWEROFF
 #KEY_CHUP
 #KEY_CHDOWN
 #KEY_CONTENTS
 #KEY_W_LINK #Media P
 #KEY_RSS #Internet
 #KEY_MTS #Dual
 #KEY_CAPTION #Subt
 #KEY_REWIND
 #KEY_FF
 #KEY_REC
 #KEY_STOP
# Bonus buttons not on the normal remote:
 #KEY_TV

# First configure the connection
ipencoded = base64.b64encode(myip)
macencoded = base64.b64encode(mymac)
messagepart1 = chr(0x64).encode() + chr(0x00).encode() + chr(len(ipencoded)).encode() + chr(0x00).encode() + ipencoded + chr(len(macencoded)).encode() + chr(0x00).encode() + macencoded + chr(len(base64.b64encode(remotename))).encode() + chr(0x00).encode() + base64.b64encode(remotename)

part1 = chr(0x00).encode() + chr(len(appstring)).encode() + chr(0x00).encode() + appstring + chr(len(messagepart1)).encode() + chr(0x00).encode() + messagepart1
sock.send(part1)

messagepart2 = chr(0xc8).encode() + chr(0x00).encode()
part2 = chr(0x00).encode() + chr(len(appstring)).encode() + chr(0x00).encode() + appstring + chr(len(messagepart2)).encode() + chr(0x00).encode() + messagepart2
sock.send(part2)

# Now send the keys as you like, e.g.,
sendKey(b"KEY_7",sock,tvappstring)
time.sleep(1)
sendKey(b"KEY_0",sock,tvappstring)

# Close the socket when done
sock.close()