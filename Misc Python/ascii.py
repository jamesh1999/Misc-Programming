import pygame
import random
 
IMG=pygame.image.load("download.jpg")
XEDGE=2000

def convert_ascii(img,xedge):
	greyscale = [" "," `","`'","'.`","'.",".-'","-^","^,","\\/:\"","~!_<>",":<>~",";:","=+","()+|?","i|j","jil1","l17{}","tv[]","rcI","fuJ2","oxLO3%","nsyzaC*","VYC","Y4569","ehNTZ0&","bwkdAFP","DX8$","pqGU","gKQS","BE","mRW#","H#","HM","M@H","M@H","M@","M@"]
	size=img.get_size()
	mul=xedge/size[0]
	img = pygame.transform.scale(img, (xedge, int(size[1]*mul/1.37)))
	size=img.get_size()
	s=""
	for y in range(0,size[1]):
		for x in range(0,size[0]):
			col=img.get_at((x,y))
			i=36-int(((col[0]+col[1]+col[2])/3)//7)
			s+=greyscale[i][random.randrange(0,len(greyscale[i]))]
		s+="\n"
	return s

open("out.txt","w").write(convert_ascii(IMG,XEDGE))
print("Done")