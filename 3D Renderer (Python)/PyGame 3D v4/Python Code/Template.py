#3DGames template
#Import PyGame and Py3D
import pygame
from Py3D import *

#Import other required modules


#Runs game normally
def main():
    #Sets up the game object
    game=Game(fov=75,ticks_per_second=60,max_fps=0,time_source=None,
                         min_brightness=10,min_colour=white, debug=False,
                          caption="Py3D Game", light_distance=0)

    #Create classes for Objects and Sub-Objects that extend ObjectBase and
    #SubObjectBase respectively


    #Create instances of the objects and add the models


    #Add your objects to the game
    objects={}
    game.objects=objects

    #Create lights(x,y,z,brightness(%),colour)
    

    #Define what should happen each tick
    class Tick_Handler(TickHandler):

        #Defines what should happen before objects update
        def pre_update(self,game):
            pass

        #Defines what should happen after objects update
        def post_update(self,game):
            pass

    #Allows use of a custom GUI
    class GUI_1(GUI):

        #What should happen every update
        def update(self,game):
            pass

        #Draws the GUI
        def draw(self,game):
            pass

    #Do not change
    game.tick_handler=Tick_Handler()
    game.gui=GUI_1()

    #Start the game loop
    game.loop()

#Runs the game
main()

#Be IDLE friendly
pygame.quit()

