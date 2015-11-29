#3DGames template
#Import PyGame and Py3D
import pygame
from Py3D import *

#Import other required modules


#Runs game normally
def main():
    #Sets up the game object
    game=Game(fov=75,ticks_per_second=40,max_fps=120,
                         update_callback=None,frame_callback=None,time_source=None,
                         min_brightness=0,min_colour=white, debug=False,
                          caption="Py3D Game", light_distance=3.5)
    pygame.mouse.set_visible(False)

    #Create classes for Objects and Sub-Objects that extend ObjectBase and
    #SubObjectBase respectively
    class Monkey(ObjectBase):
        def update(self):
            mx,my=pygame.mouse.get_rel()
            if mx:
                self.rotate("x",self.centre(),-mx)
            if my:
                self.rotate("y",self.centre(),-my)
            for event in game.events:
                if event.type==pygame.MOUSEBUTTONDOWN:
                    if event.button==4:
                        self.scale(self.centre(),2)
                    elif event.button==5:
                        self.scale(self.centre(),0.5)
            if pygame.key.get_pressed()[K_w]:
                self.y=-0.5
            if pygame.key.get_pressed()[K_s]:
                self.y=0.5
            if pygame.key.get_pressed()[K_a]:
                self.x=-0.5
            if pygame.key.get_pressed()[K_d]:
                self.x=0.5
            if pygame.key.get_pressed()[K_q]:
                self.z=-0.5
            if pygame.key.get_pressed()[K_z]:
                self.z=0.5

    #Create instances of the objects and add the models
    monkey=Monkey()
    monkey_mesh=SubObjectBase()
    monkey.add_subobject(monkey_mesh)
    monkey_mesh.add_mesh("monkey.obj")

    #Add your objects to the game
    objects={}
    objects["monkey"]=monkey
    game.objects=objects

    #Create lights(x,y,z,brightness(%),colour)
    game.add_light(x=0,y=0,z=0,brightness=100,colour=white)

    #Define what should happen each tick
    class TickHandler(object):

        #Defines what should happen before objects update
        def pre_update(self,game):
            pass

        #Defines what should happen after objects update
        def post_update(self,game):
            pass

    #Allows use of a custom GUI
    class GUI(object):

        #What should happen every update
        def update(self,game):
            pass

        #Draws the GUI
        def draw(self,game):
            pass

    #Do not change
    game.tick_handler=TickHandler()
    game.gui=GUI()

    #Start the game loop
    game.loop()

#Runs the game
main()

#Be IDLE friendly
pygame.quit()
