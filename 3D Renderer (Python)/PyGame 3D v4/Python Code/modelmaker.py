#3DGames template
#Import PyGame and Py3D
import pygame
from Py3D import *

#Import other required modules


#Runs game normally
def main():
    while True:
        try:
            ldist=clamp(float(input("What do you want the light distance to be? ")),99999,0)
        except:
            print("That isnt an actual number.")
            continue;
        break;

    #Sets up the game object
    game=Game(fov=75,ticks_per_second=60,max_fps=0,time_source=None,
                         min_brightness=10,min_colour=white, debug=False,
                          caption="Py3D Game", light_distance=ldist)
    game.displayNodes = 1
    game.displayEdges = 1
    game.displayFaces = 1
    game.max_frame_skip=1
    pygame.display.iconify()

    #Create classes for Objects and Sub-Objects that extend ObjectBase and
    #SubObjectBase respectively
    class Model(ObjectBase):
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

    while True:
        choice=input("Do you want to edit an existing model? Y/N ")
        if choice=="Y" or choice=="N" or choice=="y" or choice=="n":
            break;

    if choice=="Y" or choice=="y":
        file=input("What is the name of the file? ")

    #Create instances of the objects and add the models
    model=Model()
    model_mesh=SubObjectBase()
    model.add_subobject(model_mesh)
    if choice=="Y" or choice=="y":
        try:
            model_mesh.add_mesh(file)
        except:
            print("That wasnt a valid file. Continuing... ")
    model.z+=6

    #Add your objects to the game
    objects={}
    objects["model"]=model
    game.objects=objects

    #Create lights(x,y,z,brightness(%),colour)
    game.add_light(x=0,y=0,z=0,brightness=100,colour=white)

    #Define what should happen each tick
    class Tick_Handler(TickHandler):
        def __init__(self):
            self.node=False
            self.nx=0
            self.ny=0
            self.nz=0
        #Defines what should happen before objects update
        def pre_update(self,game):
            pass

        #Defines what should happen with/after objects update
        def post_update(self,game):
            if not self.node:
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
            else:
                if pygame.key.get_pressed()[K_BACKSPACE]:
                    self.nx=0
                    self.ny=0
                    self.nz=0
                    self.node=False
                    game.objects["model"].sub_objects[0].nodes=game.objects["model"].sub_objects[0].nodes[:len(game.objects["model"].sub_objects[0].nodes)-1]
                else:
                    if pygame.key.get_pressed()[K_KP6]:
                        self.nx+=0.5
                    if pygame.key.get_pressed()[K_KP4]:
                        self.nx-=0.5
                    if pygame.key.get_pressed()[K_KP2]:
                        self.ny+=0.5
                    if pygame.key.get_pressed()[K_KP8]:
                        self.ny-=0.5
                    if pygame.key.get_pressed()[K_KP_PLUS]:
                        self.nz+=0.5
                    if pygame.key.get_pressed()[K_KP_MINUS]:
                        self.nz-=0.5
                    game.objects["model"].sub_objects[0].nodes[len(game.objects["model"].sub_objects[0].nodes)-1].x=self.nx
                    game.objects["model"].sub_objects[0].nodes[len(game.objects["model"].sub_objects[0].nodes)-1].y=self.ny
                    game.objects["model"].sub_objects[0].nodes[len(game.objects["model"].sub_objects[0].nodes)-1].z=self.nz
                    if pygame.key.get_pressed()[K_RETURN]:
                        self.node=False
                        self.nx=0
                        self.ny=0
                        self.nz=0
            if not self.node:
                if pygame.key.get_pressed()[K_1]:
                    self.node=True
                    game.objects["model"].sub_objects[0].add_nodes((0,0,0))
                if pygame.key.get_pressed()[K_2]:
                    a=None
                    b=None
                    fin=0
                    while not fin:
                        for event in pygame.event.get():
                            if event.type == pygame.KEYDOWN:
                                if event.key==K_BACKSPACE:
                                    fin=1
                                    break;
                            if event.type== pygame.MOUSEBUTTONDOWN:
                                pygame.event.clear()
                                if event.button==1:
                                    j=game.instance
                                    screen=game.screen
                                    pos=event.pos
                                    dist_list=[]
                                    d=game._scr_dist
                                    for key in game.objects:
                                        obj=game.objects.get(key)
                                        for i in obj.sub_objects:
                                            for node in i.nodes:
                                                dist=getDistance((node.x,node.y,node.z),(j.x,j.y,j.z))
                                                dist_list.append([node,dist,"n"])
                                                node.camdist=dist
                                                xx=node.x-j.x
                                                yy=node.y-j.y
                                                zz=node.z-j.z
                                                rd=math.hypot(yy,xx)
                                                theta=math.atan2(yy, xx) + math.radians(-j.roll)
                                                xx=rd * math.cos(theta)
                                                yy=rd * math.sin(theta)
                                                rd=math.hypot(yy,zz)
                                                theta=math.atan2(yy, zz) + math.radians(-j.pitch)
                                                zz=rd * math.cos(theta)
                                                yy=rd * math.sin(theta)
                                                rd=math.hypot(xx,zz)
                                                theta=math.atan2(xx, zz) + math.radians(-j.yaw)
                                                zz=rd * math.cos(theta)
                                                xx=rd * math.sin(theta)
                                                if zz<0:
                                                    node.renderx=node.rendery=None
                                                    continue;
                                                node.renderx=int(xx/zz*d+game.width/2)
                                                node.rendery=int(yy/zz*d+game.height/2)
                                    node=None
                                    for part in sorted(dist_list,key=itemgetter(1),reverse=False):
                                        partobj=part[0]
                                        if (pos[0]-partobj.renderx)**2+(pos[1]-partobj.rendery)**2<=game.nodeRadius**2:
                                            node=partobj
                                            pygame.draw.circle(screen, yellow, (node.renderx,node.rendery), game.nodeRadius, 0)
                                            pygame.display.flip()
                                            break;
                                    if not node==None:
                                        if a==None:
                                            a=node
                                        else:
                                            b=node
                                        game._wait(0.1)
                                    if not a==None and not b==None:
                                        count=0
                                        an=0
                                        bn=0
                                        for node in game.objects["model"].sub_objects[0].nodes:
                                            if node==a:
                                                an=count
                                            elif node==b:
                                                bn=count
                                            count+=1
                                        game.objects["model"].sub_objects[0].add_edges((an,bn))
                                        fin=1
                                        break;
                if pygame.key.get_pressed()[K_3]:
                    a=None
                    b=None
                    c=None
                    fin=0
                    while not fin:
                        for event in pygame.event.get():
                            if event.type == pygame.KEYDOWN:
                                if event.key==K_BACKSPACE:
                                    fin=1
                                    break;
                            if event.type== pygame.MOUSEBUTTONDOWN:
                                pygame.event.clear()
                                if event.button==1:
                                    j=game.instance
                                    screen=game.screen
                                    pos=event.pos
                                    dist_list=[]
                                    d=game._scr_dist
                                    for key in game.objects:
                                        obj=game.objects.get(key)
                                        for i in obj.sub_objects:
                                            for node in i.nodes:
                                                dist=getDistance((node.x,node.y,node.z),(j.x,j.y,j.z))
                                                dist_list.append([node,dist,"n"])
                                                node.camdist=dist
                                                xx=node.x-j.x
                                                yy=node.y-j.y
                                                zz=node.z-j.z
                                                rd=math.hypot(yy,xx)
                                                theta=math.atan2(yy, xx) + math.radians(-j.roll)
                                                xx=rd * math.cos(theta)
                                                yy=rd * math.sin(theta)
                                                rd=math.hypot(yy,zz)
                                                theta=math.atan2(yy, zz) + math.radians(-j.pitch)
                                                zz=rd * math.cos(theta)
                                                yy=rd * math.sin(theta)
                                                rd=math.hypot(xx,zz)
                                                theta=math.atan2(xx, zz) + math.radians(-j.yaw)
                                                zz=rd * math.cos(theta)
                                                xx=rd * math.sin(theta)
                                                if zz<0:
                                                    node.renderx=node.rendery=None
                                                    continue;
                                                node.renderx=int(xx/zz*d+game.width/2)
                                                node.rendery=int(yy/zz*d+game.height/2)
                                    node=None
                                    for part in sorted(dist_list,key=itemgetter(1),reverse=False):
                                        partobj=part[0]
                                        if (pos[0]-partobj.renderx)**2+(pos[1]-partobj.rendery)**2<=game.nodeRadius**2:
                                            node=partobj
                                            pygame.draw.circle(screen, yellow, (node.renderx,node.rendery), game.nodeRadius, 0)
                                            pygame.display.flip()
                                            break;
                                    if not node==None:
                                        if a==None:
                                            a=node
                                        elif b==None:
                                            b=node
                                        else:
                                            c=node
                                        game._wait(0.1)
                                    if not a==None and not b==None and not c==None:
                                        count=0
                                        an=0
                                        bn=0
                                        cn=0
                                        for node in game.objects["model"].sub_objects[0].nodes:
                                            if node==a:
                                                an=count
                                            elif node==b:
                                                bn=count
                                            elif node==c:
                                                cn=count
                                            count+=1
                                        game.objects["model"].sub_objects[0].add_faces(((an,bn,cn),white))
                                        fin=1
                                        break;
                if pygame.key.get_pressed()[K_4]:
                    fin=0
                    while not fin:
                        for event in pygame.event.get():
                            if event.type == pygame.KEYDOWN:
                                if event.key==K_BACKSPACE:
                                    fin=1
                                    break;
                            if event.type== pygame.MOUSEBUTTONDOWN:
                                pygame.event.clear()
                                if event.button==1:
                                    j=game.instance
                                    screen=game.screen
                                    pos=event.pos
                                    dist_list=[]
                                    d=game._scr_dist
                                    for key in game.objects:
                                        obj=game.objects.get(key)
                                        for i in obj.sub_objects:
                                            for node in i.nodes:
                                                dist=getDistance((node.x,node.y,node.z),(j.x,j.y,j.z))
                                                node.camdist=dist
                                                xx=node.x-j.x
                                                yy=node.y-j.y
                                                zz=node.z-j.z
                                                rd=math.hypot(yy,xx)
                                                theta=math.atan2(yy, xx) + math.radians(-j.roll)
                                                xx=rd * math.cos(theta)
                                                yy=rd * math.sin(theta)
                                                rd=math.hypot(yy,zz)
                                                theta=math.atan2(yy, zz) + math.radians(-j.pitch)
                                                zz=rd * math.cos(theta)
                                                yy=rd * math.sin(theta)
                                                rd=math.hypot(xx,zz)
                                                theta=math.atan2(xx, zz) + math.radians(-j.yaw)
                                                zz=rd * math.cos(theta)
                                                xx=rd * math.sin(theta)
                                                if zz<0:
                                                    node.renderx=node.rendery=None
                                                    continue;
                                                node.renderx=int(xx/zz*d+game.width/2)
                                                node.rendery=int(yy/zz*d+game.height/2)
                                            for f in i.faces:
                                                dist=(f.corners[0].camdist+f.corners[1].camdist+f.corners[2].camdist)/3
                                                distn=getDistance((sum([node.x for node in f.corners]) / 3+f.norm[0],sum([node.y for node in f.corners]) / 3+f.norm[1],sum([node.z for node in f.corners]) / 3+f.norm[2]),(j.x,j.y,j.z))
                                                if distn<dist:
                                                    dist_list.append([f,dist,"f"])
                                    face=None
                                    for part in sorted(dist_list,key=itemgetter(1),reverse=False):
                                        try:
                                            partobj=part[0]
                                            p1=partobj.corners[0]
                                            p2=partobj.corners[1]
                                            p3=partobj.corners[2]
                                            alpha=((p2.rendery - p3.rendery)*(pos[0] - p3.renderx) + (p3.renderx - p2.renderx)*(pos[1] - p3.rendery))/((p2.rendery - p3.rendery)*(p1.renderx - p3.renderx) + (p3.renderx - p2.renderx)*(p1.rendery - p3.rendery))
                                            beta=((p3.rendery - p1.rendery)*(pos[0] - p3.renderx) + (p1.renderx - p3.renderx)*(pos[1] - p3.rendery))/((p2.rendery - p3.rendery)*(p1.renderx - p3.renderx) + (p3.renderx - p2.renderx)*(p1.rendery - p3.rendery))
                                            gamma=1 - alpha - beta
                                            if alpha>0 and beta>0 and gamma>0:
                                                face=partobj
                                                break;
                                        except:
                                            continue;
                                    game._wait(0.1)
                                    if not face==None:
                                        count=0
                                        facen=0
                                        for f in game.objects["model"].sub_objects[0].faces:
                                            if f==face:
                                                facen=count
                                            count+=1
                                        pygame.display.iconify()
                                        while True:
                                            try:
                                                col0=clamp(int(input("What should the red value be? ")),255,0)
                                                col1=clamp(int(input("What should the green value be? ")),255,0)
                                                col2=clamp(int(input("What should the blue value be? ")),255,0)
                                            except:
                                                print("Those valuse are invalid.")
                                                continue;
                                            break;

                                        restart=False
                                        while not restart and not game._done:
                                            for event in pygame.event.get(pygame.VIDEOEXPOSE):
                                                if event.type == pygame.VIDEOEXPOSE:
                                                    restart = True
                                        game.objects["model"].sub_objects[0].faces[facen].col=(col0,col1,col2)
                                        fin=1
                                        break;

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
