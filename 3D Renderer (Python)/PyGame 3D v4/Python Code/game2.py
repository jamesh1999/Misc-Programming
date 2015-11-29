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
                          caption="Py3D Game", light_distance=4.4)

    #Create classes for Objects and Sub-Objects that extend ObjectBase and
    #SubObjectBase respectively
    class Monkey(ObjectBase):
        def update(self):
            if pygame.key.get_pressed()[K_q]:
                self.rotate_z=-2
            if pygame.key.get_pressed()[K_w]:
                self.rotate_y=2
            if pygame.key.get_pressed()[K_e]:
                self.rotate_z=2
            if pygame.key.get_pressed()[K_a]:
                self.rotate_x=2
            if pygame.key.get_pressed()[K_s]:
                self.rotate_y=-2
            if pygame.key.get_pressed()[K_d]:
                self.rotate_x=-2
            if pygame.key.get_pressed()[K_i]:
                self.y=-0.5
            if pygame.key.get_pressed()[K_k]:
                self.y=0.5
            if pygame.key.get_pressed()[K_j]:
                self.x=-0.5
            if pygame.key.get_pressed()[K_l]:
                self.x=0.5
            if pygame.key.get_pressed()[K_u]:
                self.z=-0.5
            if pygame.key.get_pressed()[K_m]:
                self.z=0.5

    #Create instances of the objects and add the models
    monkey=Monkey()
    monkey_mesh=SubObjectBase()
    monkey.add_subobject(monkey_mesh)
    monkey_mesh.add_mesh("monkey.obj")
    monkey.z+=6

    #Add your objects to the game
    objects={}
    objects["monkey"]=monkey
    game.objects=objects

    #Create lights(x,y,z,brightness(%),colour)
    game.add_light(x=0,y=0,z=0,brightness=100,colour=white)

    #Define what should happen each tick
    class Tick_Handler(TickHandler):

        #Defines what should happen before objects update
        def pre_update(self,game):
            pass

        #Defines what should happen with/after objects update
        def post_update(self,game):
            if not pygame.key.get_pressed()[K_KP0]:
                if pygame.key.get_pressed()[K_KP6]:
                    game.instance.x+=0.5
                if pygame.key.get_pressed()[K_KP4]:
                    game.instance.x-=0.5
                if pygame.key.get_pressed()[K_KP2]:
                    game.instance.y+=0.5
                if pygame.key.get_pressed()[K_KP8]:
                    game.instance.y-=0.5
                if pygame.key.get_pressed()[K_KP_PLUS]:
                    game.instance.z+=0.5
                if pygame.key.get_pressed()[K_KP_MINUS]:
                    game.instance.z-=0.5
            else:
                if pygame.key.get_pressed()[K_KP6]:
                    game.instance.yaw+=0.5
                if pygame.key.get_pressed()[K_KP4]:
                    game.instance.yaw-=0.5
                if pygame.key.get_pressed()[K_KP2]:
                    game.instance.pitch+=0.5
                if pygame.key.get_pressed()[K_KP8]:
                    game.instance.pitch-=0.5
                if pygame.key.get_pressed()[K_KP7]:
                    game.instance.roll+=0.5
                if pygame.key.get_pressed()[K_KP9]:
                    game.instance.roll-=0.5

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
