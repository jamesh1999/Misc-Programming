import pygame,random
 
IMG=pygame.image.load("Audi R8.jpg")

screen=pygame.display.set_mode((640,640))

def convert(img):
	size=img.get_size()
	for y in range(0,size[1]):
		for x in range(0,size[0]):
			try:
				col=img.get_at((x,y))
				col=(col[0]+col[1]+col[2])/3
				for cnt,c in enumerate(COLOURS):
					if c[0]>=col>=COLOURS[cnt+1][0]:
						d0=c[0]-col
						d1=col-COLOURS[cnt+1][0]
						dist=c[0]-COLOURS[cnt+1][0]
						if not dist:
							dist+=1
						new=(((c[1][0]*(dist-d0)/dist)+(COLOURS[cnt+1][1][0]*(dist-d1)/dist)),((c[1][1]*(dist-d0)/dist)+(COLOURS[cnt+1][1][1]*(dist-d1)/dist)),((c[1][2]*(dist-d0)/dist)+(COLOURS[cnt+1][1][2]*(dist-d1)/dist)))
						break;
				screen.set_at((x,y),new)
			except:
				pass

while True:
	num_col=random.randint(2,200)
	COLOURS=[(255,(random.randrange(255),random.randrange(255),random.randrange(255)))]
	prev=255
	for i in range(num_col-2):
		prev=random.randrange(prev+1)
		COLOURS.append((prev,(random.randrange(255),random.randrange(255),random.randrange(255))))
	COLOURS.append((0,(random.randrange(255),random.randrange(255),random.randrange(255))))
	convert(IMG)
	pygame.display.flip()
input("Press enter to exit. ")