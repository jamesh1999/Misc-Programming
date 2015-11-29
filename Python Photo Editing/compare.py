import pygame
import math
import colorsys


LONG = 200               #Size of long edge of image
THRESHOLD = 60           #Evaluation given is percentage between THRESHOLD and 100
H_THRESHOLD = 4.5        #Minimum difference between pixels to be counted
S_THRESHOLD = 7.5        #"
V_THRESHOLD = 6          #"


def compare(real, test):
	size = real.get_size()

	if size[0] > size[1]:
		real = pygame.transform.scale(real, (LONG, int(size[1] / size[0] * LONG)))
		test = pygame.transform.scale(test, (LONG, int(size[1] / size[0] * LONG)))

	else:
		real = pygame.transform.scale(real, (int(size[0] / size[1] * LONG), LONG))
		test = pygame.transform.scale(test, (int(size[0] / size[1] * LONG), LONG))

	size = real.get_size()
	score = 0

	for x in range(size[0]):
		for y in range(size[1]):
			preal = real.get_at((x, y))
			ptest = test.get_at((x, y))

			hsvreal = colorsys.rgb_to_hsv(preal[0], preal[1], preal[2])
			hsvtest = colorsys.rgb_to_hsv(ptest[0], ptest[1], ptest[2])

			if math.fabs(hsvtest[0] - hsvreal[0]) > H_THRESHOLD or math.fabs(hsvtest[1] - hsvreal[1]) > S_THRESHOLD or math.fabs(hsvtest[2] - hsvreal[2]) > V_THRESHOLD:
				score += math.hypot(math.hypot(math.fabs(preal[0] - ptest[0]), math.fabs(preal[1] - ptest[1])), math.fabs(preal[2] - ptest[2]))

	maximum = (size[0] * size[1] * 442)

	return (((maximum - score) / maximum * 100) - THRESHOLD) * 100 / (100 - THRESHOLD)



if __name__ == "__main__":
	IMG_REAL=pygame.image.load("_DSC0525.JPG")
	IMG_TEST=pygame.image.load("_DSC0246.JPG")
	print(compare(IMG_REAL, IMG_TEST))