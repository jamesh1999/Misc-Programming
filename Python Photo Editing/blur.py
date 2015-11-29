import pygame

def blur(img, radius):
	new = img.copy()
	size = new.get_size()
	for x in range(size[0]):
		for y in range(size[1]):
			total = [0,0,0]
			cnt = 0

			for kx in range(-radius,radius+1):
				for ky in range(-radius,radius+1):
					try:
						ncol = img.get_at((x + kx, y + ky))
						total[0] += ncol[0]
						total[1] += ncol[1]
						total[2] += ncol[2]
						cnt+=1
					except IndexError:
						pass

			new.set_at((x,y) , (total[2]/cnt,total[1]/cnt,total[0]/cnt))

	return new

if __name__ == "__main__":
	IMG=pygame.image.load("image_exp_space052.jpg")
	screen=pygame.display.set_mode(IMG.get_size())
	screen.fill((0,0,0))
	screen.blit(blur(IMG,2), IMG.get_rect())
	pygame.display.flip()
	input("")