#Import pygame and threading
import threading
import pygame.display
from pygame.locals import *

#Set constants

#Leave
DIVISIONS=64
NUM_THREADS=DIVISIONS**2

#Maximum iterations of the sequence
MAX_ITERATIONS=128

#Zoom speed
ZOOM=0.4

#Increase in iterations per pass
ITERATION_STEP=16

#Starting iterations
START_ITERATIONS=16


#Other variables
done=False
stop_refresh=False
screen=pygame.display.set_mode((0,0),pygame.FULLSCREEN)
disp_rect=[-3.22,-1.5,1.6,1.5]
width,height=screen.get_size()[0]-screen.get_size()[0]%DIVISIONS,screen.get_size()[1]-screen.get_size()[1]%DIVISIONS
cur_iterations=START_ITERATIONS
new_scr=True
pixels={}
npixels={}



#Thread for first pass of screen
class NewWorker(threading.Thread):
    def __init__(self,num):
        self.num=num
        threading.Thread.__init__(self)
    def run(self):
        a=int((self.num-(self.num%DIVISIONS))/DIVISIONS*width/DIVISIONS)
        b=int(self.num%DIVISIONS*height/DIVISIONS)
        for pixelw in range(a,int(width/DIVISIONS)+a):
            for pixelh in range(b,int(height/DIVISIONS)+b):
                c=complex(disp_rect[0]+(disp_rect[2]-disp_rect[0])*(pixelw/width),disp_rect[1]+(disp_rect[3]-disp_rect[1])*(pixelh/height))
                z=0
                for i in range(cur_iterations):
                    z=z*z+c
                    if z.real*z.real+z.imag*z.imag>4:
                        col=255/cur_iterations*i
                        screen.set_at((pixelw,pixelh),(col,col,col))
                        break;
                else:
                    npixels[(pixelh,pixelw)]=(z,c)
                    screen.set_at((pixelw,pixelh),(0,0,0))
        pygame.display.flip()

#Thread for other passes
class Worker(threading.Thread):
    def __init__(self,num):
        self.num=num
        threading.Thread.__init__(self)
    def run(self):
        a=int((self.num-(self.num%DIVISIONS))/DIVISIONS*width/DIVISIONS)
        b=int(self.num%DIVISIONS*height/DIVISIONS)
        for pixelw in range(a,int(width/DIVISIONS)+a):
            for pixelh in range(b,int(height/DIVISIONS)+b):
                try:
                    z,c=pixels[(pixelh,pixelw)]
                    for i in range(ITERATION_STEP):
                        z=z*z+c
                        if z.real*z.real+z.imag*z.imag>4:
                            col=255/cur_iterations*(i+cur_iterations-ITERATION_STEP+1)
                            screen.set_at((pixelw,pixelh),(col,col,col))
                            break;
                    else:
                        npixels[(pixelh,pixelw)]=(z,c)
                        screen.set_at((pixelw,pixelh),(0,0,0))
                except:
                    ocol=screen.get_at((pixelw,pixelh))
                    val=ocol[0]*(cur_iterations-ITERATION_STEP)/cur_iterations
                    screen.set_at((pixelw,pixelh),(val,val,val))
        pygame.display.flip()



#Main loop
while not done:
    if not stop_refresh:
        #Draw

        #Creating and starting threads
        if new_scr:
            for i in range(NUM_THREADS):
                NewWorker(i).start()
        else:
            for i in range(NUM_THREADS):
                Worker(i).start()

        pixels=npixels
        npixels={}
        
        #Setting new iterations and displaying current iterations
        pygame.display.set_caption("Iterations: "+str(cur_iterations))
        if cur_iterations<MAX_ITERATIONS:
            cur_iterations+=ITERATION_STEP
        else:
            stop_refresh=True
        new_scr=False

    #Registering user input
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True
            break;
        if event.type == pygame.KEYDOWN:
            if event.key == K_ESCAPE and event.mod&KMOD_SHIFT:
                done = True
                break;
            elif event.key == K_ESCAPE:
                pygame.display.iconify()
            elif event.key == K_EQUALS:
                disp_rect=[disp_rect[0]+(disp_rect[2]-disp_rect[0])/2*ZOOM,disp_rect[1]+(disp_rect[3]-disp_rect[1])/2*ZOOM,disp_rect[2]-(disp_rect[2]-disp_rect[0])/2*ZOOM,disp_rect[3]-(disp_rect[3]-disp_rect[1])/2*ZOOM]
                new_scr=True
                cur_iterations=START_ITERATIONS
                stop_refresh=False
            elif event.key == K_MINUS:
                disp_rect=[disp_rect[0]-(disp_rect[2]-disp_rect[0])/2*ZOOM,disp_rect[1]-(disp_rect[3]-disp_rect[1])/2*ZOOM,disp_rect[2]+(disp_rect[2]-disp_rect[0])/2*ZOOM,disp_rect[3]+(disp_rect[3]-disp_rect[1])/2*ZOOM]
                new_scr=True
                cur_iterations=START_ITERATIONS
                stop_refresh=False
            elif event.key == K_UP:
                disp_rect=[disp_rect[0],disp_rect[1]-(disp_rect[3]-disp_rect[1])/2,disp_rect[2],disp_rect[3]-(disp_rect[3]-disp_rect[1])/2]
                new_scr=True
                cur_iterations=START_ITERATIONS
                stop_refresh=False
            elif event.key == K_DOWN:
                disp_rect=[disp_rect[0],disp_rect[1]+(disp_rect[3]-disp_rect[1])/2,disp_rect[2],disp_rect[3]+(disp_rect[3]-disp_rect[1])/2]
                new_scr=True
                cur_iterations=START_ITERATIONS
                stop_refresh=False
            elif event.key == K_LEFT:
                disp_rect=[disp_rect[0]-(disp_rect[2]-disp_rect[0])/2,disp_rect[1],disp_rect[2]-(disp_rect[2]-disp_rect[0])/2,disp_rect[3]]
                new_scr=True
                cur_iterations=START_ITERATIONS
                stop_refresh=False
            elif event.key == K_RIGHT:
                disp_rect=[disp_rect[0]+(disp_rect[2]-disp_rect[0])/2,disp_rect[1],disp_rect[2]+(disp_rect[2]-disp_rect[0])/2,disp_rect[3]]
                new_scr=True
                cur_iterations=START_ITERATIONS
                stop_refresh=False