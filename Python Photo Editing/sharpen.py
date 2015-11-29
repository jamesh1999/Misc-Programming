import pygame,blur,math

AMOUNT = 1
RADIUS = 3
MASK = 2

def getMask(img):
	blurred = blur.blur(img,RADIUS)
	mask = img.copy()

	size = img.get_size()

	for x in range(size[0]):
		for y in range(size[1]):
			old = mask.get_at((x,y))
			blurred_col = blurred.get_at((x,y))
			mask.set_at((x,y),(min(math.sqrt((old[0]-blurred_col[0])**2)*MASK,255),min(math.sqrt((old[1]-blurred_col[1])**2)*MASK,255),min(math.sqrt((old[2]-blurred_col[2])**2)*MASK,255)))

	return mask



def sharpen(img):
	new = img.copy()
	mask = getMask(img)
	size = img.get_size()

	for x in range(size[0]):
		for y in range(size[1]):
			mask_col = mask.get_at((x,y))
			contrast_r = (259*((AMOUNT*mask_col[0])+255))/(255*(259-(AMOUNT*mask_col[0])))
			contrast_g = (259*((AMOUNT*mask_col[1])+255))/(255*(259-(AMOUNT*mask_col[1])))
			contrast_b = (259*((AMOUNT*mask_col[2])+255))/(255*(259-(AMOUNT*mask_col[2])))
			old = new.get_at((x,y))
			new.set_at((x,y), (max(min(contrast_r*(old[2]-128)+128,255),0),max(min(contrast_g*(old[1]-128)+128,255),0),max(min(contrast_b*(old[0]-128)+128,255),0)))

	return new
	
if __name__ == "__main__":
	IMG=pygame.image.load("image_exp_space052.jpg")
	screen=pygame.display.set_mode(IMG.get_size())
	screen.fill((0,0,0))
	screen.blit(sharpen(IMG), IMG.get_rect())
	pygame.display.flip()
	print("done")
	input("")