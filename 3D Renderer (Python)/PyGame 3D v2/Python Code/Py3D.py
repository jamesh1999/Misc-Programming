#Py3D.py
#Contains all code to render and run 3D games

#Imports required modules
import time,sys,math,random
from operator import itemgetter

#Import pygame
import pygame
from pygame.locals import *



#Variables

black=(0,0,0)
white=(255,255,255)
blue=(0,0,255)
red=(255,0,0)
green=(0,255,0)
yellow=(255,255,0)
cyan=(0,255,255)
magenta=(255,0,255)
light_blue=(100,170,255)
pink=(255,150,255)
purple=(180,65,255)
grey=(150,150,150)
dark_grey=(80,80,80)
light_grey=(180,180,180)
dark_blue=(0,10,100)
dark_green=(10,110,10)
lime_green=(200,250,40)
orange=(250,130,0)



#Classes

#Main game class
class Game(object):
    def __init__(self,fov=75,ticks_per_second=60,max_fps=120,update_callback=None,
                 frame_callback=None,time_source=None,max_frame_skip=5,min_brightness=100,
                 min_colour=white, debug=False, caption="Py3D Game", gui=None,
                 tick_handler=None, render_distance=500, light_distance=0):

        #Starts pygame
        pygame.init()

        #Screen Related
        self.screen=pygame.display.set_mode((0,0),pygame.FULLSCREEN)
        self.width,self.height=self.screen.get_size()

        #Settings
        self.ticks_per_second = ticks_per_second
        self.max_fps = max_fps
        self.max_frame_skip = max_frame_skip
        self.fov=fov
        pygame.display.set_caption(caption)
        self.debug = debug
        self.auto_move = True

        #Rendering Settings
        self.displayNodes = 0
        self.displayEdges = 0
        self.displayFaces = 1
        self.displayLights = 0
        self.displayNorms = 0
        self.displayRays = 0
        self.background = black
        self.node_colour = green
        self.edge_colour = blue
        self.nodeRadius = 5
        self.min_brightness=min_brightness
        self.min_colour=min_colour
        self.render_distance=render_distance
        self.light_distance=light_distance

        #Clock Related
        self._wait = time.sleep
        if time_source is not None:
            self._get_ticks = time_source
        elif sys.platform in ('win32','cygwin'):
            self._get_ticks = time.clock
        else:
            self._get_ticks = time.time

        self.time = self._get_ticks()
        self._elapsed = 0.0
        self._frames_skipped = 0
        self._last_update = self.time
        self._last_frame = self.time
        self._update_ready = True
        self._frame_ready = True
        self._tps = 0
        self.tps = 0
        self.fps = 0
        self.ups = 0
        self._frame_count = 0
        self._update_count = 0
        self._frame_start = 0
        self._frame_time = 0
        self._frames_to_skip = 0
        self._update_start = 0
        self._update_time = 0
        self._next_update = 0

        #Other
        self._done = False
        self._skip_console = False
        self.objects = {}
        self.tick_handler = tick_handler
        self.gui = gui
        self.instance = Instance(0,0,0,0,0,0)
        self.lights = []
        self.events= []

    #Properties
    @property
    def fov(self):
        return self._fov
    @fov.setter
    def fov(self, val):
        if val > 0:
            self._fov = val
        else:
            self._fov = 75
        self._scr_dist=math.tan(math.radians(180-val/2+90))*self.width/2

    @property
    def ticks_per_second(self):
        return self._ticks_per_second
    @ticks_per_second.setter
    def ticks_per_second(self, val):
        if val > 0:
            self._ticks_per_second = val
        else:
            self._ticks_per_second = 25
        self._tick_step = 1.0 / self._ticks_per_second

    @property
    def max_fps(self):
        return self._max_fps
    @max_fps.setter
    def max_fps(self, val):
        if val > 0:
            self._max_fps = val
            self._frame_step = 1.0 / val
        else:
            self._max_fps = 0
            self._frame_step = 0

    @property
    def max_frame_skip(self):
        return self._max_frame_skip
    @max_frame_skip.setter
    def max_frame_skip(self, n):
        if n > 0:
            self._max_frame_skip = n
        else:
            self._max_frame_skip = 0

    #Main loop
    #Updates objects, controls clock, renders game and registers iconify and close of window
    def loop(self):
        while not self._done:
            TIME = self._get_ticks()
            self._ticks = TIME - self.time
            self._elapsed += self._ticks
            self.time = TIME

            if self._elapsed >= 1.0:
                self._elapsed %= 1.0
                self.tps = self._tps
                self._tps = 0
                self.fps = self._frame_count
                self.ups = self._update_count
                self._frame_count = self._update_count = 0

            self._tps += 1
            self._update_ready = self._frame_ready = False

            if self._frames_to_skip:
                self._update_ready = True
                self._frames_to_skip-=1
                if not self._frames_to_skip:
                    self._frame_ready = True
            elif TIME >= self._next_update:
                self._update_ready = True

            if self.max_fps == 0 or self._last_frame + self._frame_step < self._get_ticks():
                if not TIME+self._frame_time>self._last_update+self._tick_step and self._frames_skipped:
                    self._frame_ready=True
                elif not self._update_ready and self._frames_skipped:
                    self._frames_to_skip=min(self._max_frame_skip,int(self._frame_time/(self._tick_step-self._update_time)))

            #Run every update
            if self._update_ready:
                self._update_start=self._get_ticks()
                self._next_update+=self._tick_step
                self._update_count += 1
                self._frames_skipped += 1
                self._last_update += self._tick_step
                self._last_update = TIME
                self.tick_handler.pre_update(self)
                for i in self.objects:
                    j=self.objects.get(i)
                    j.update()
                    if self.auto_move:
                        j.end_update()
                for i in self.lights:
                    i.update()
                self.gui.update(self)
                self.tick_handler.post_update(self)
                self.events=pygame.event.get()
                for event in self.events:
                    if self.debug:
                        print(event)
                    if event.type == pygame.QUIT:
                        self._done = True
                        break;
                    if event.type == pygame.KEYDOWN:
                        if event.key == K_ESCAPE and event.mod&KMOD_SHIFT:
                            self._done = True
                        elif event.key == K_ESCAPE:
                            pygame.display.iconify()
                        elif event.key == K_LALT:
                            pygame.display.iconify()
                            self.console()
                self._update_time=self._get_ticks()-self._update_start

            #Run every frame
            if self._frame_ready:
                self._frame_start=self._get_ticks()
                self._frame_count += 1
                self._frames_skipped = 0
                self._last_frame = TIME
                #-----Start of render function-----#
                screen=self.screen
                screen.fill(self.background)
                d=self._scr_dist
                j=self.instance

                dist_list=[]
                for key in self.objects:
                    wireframe=self.objects.get(key)
                    for i in wireframe.sub_objects:
                        for node in i.nodes:
                            dist=getDistance((node.x,node.y,node.z),(j.x,j.y,j.z))
                            dist_list.append([node,dist,"n"])
                            node.camdist=dist
                            try:
                                xx=node.x-j.x
                                yy=node.y-j.y
                                zz=node.z-j.z
                                if zz<0:
                                    node.renderx=node.rendery=None
                                    continue;
                                node.renderx=xx/zz*d+self.width/2
                                node.rendery=yy/zz*d+self.height/2
                            except ZeroDivisionError:
                                node.renderx=node.rendery=None
                        if self.displayEdges:
                            for edge in i.edges:
                                dist=(edge.start.camdist+edge.stop.camdist)/2
                                dist_list.append([edge,dist,"e"])
                        if self.displayFaces:
                            for face in i.faces:
                                dist=(face.corners[0].camdist+face.corners[1].camdist+face.corners[2].camdist)/3
                                dist_list.append([face,dist,"f"])
                for part in sorted(dist_list,key=itemgetter(1),reverse=True):
                    partobj=part[0]
                    if part[2]=="n" and self.displayNodes:
                        try:
                            pygame.draw.circle(screen, self.node_colour, (int(partobj.renderx),int(partobj.rendery)), self.nodeRadius, 0)
                        except TypeError:
                            pass

                    if part[2]=="e" and self.displayEdges:
                        try:
                            pygame.draw.aaline(screen, self.edge_colour, (int(partobj.start.renderx),int(partobj.start.rendery)), (int(partobj.stop.renderx),int(partobj.stop.rendery)), 1)
                        except TypeError:
                            pass

                    if part[2]=="f" and self.displayFaces:
                        meanX = sum([node.x for node in partobj.corners]) / 3
                        meanY = sum([node.y for node in partobj.corners]) / 3
                        meanZ = sum([node.z for node in partobj.corners]) / 3
                        centre=(meanX,meanY,meanZ)
                        vvector=normalise((centre[0]-j.x,centre[1]-j.y,centre[2]-j.z))
                        facedist=partobj.norm[0]*vvector[0]+partobj.norm[1]*vvector[1]+partobj.norm[2]*vvector[2]
                        if facedist<0:
                            cornerlist=[]
                            for node in partobj.corners:
                                cornerlist.append((node.renderx,node.rendery))
                            light=[0,0,0]
                            for lightobj in self.lights:
                                lvector=normalise((lightobj.x-centre[0],lightobj.y-centre[1],lightobj.z-centre[2]))
                                dist=lvector[0]*partobj.norm[0]+lvector[1]*partobj.norm[1]+lvector[2]*partobj.norm[2]
                                rvector=normalise((2*dist*partobj.norm[0]-lvector[0],2*dist*partobj.norm[1]-lvector[1],2*dist*partobj.norm[2]-lvector[2]))

                                #Diffusion shading
                                diffuseval=[0,0,0]
                                diffuseval[0]=partobj.diff_intensity*partobj.diff_tint[0]*max(dist,0)/255
                                diffuseval[1]=partobj.diff_intensity*partobj.diff_tint[1]*max(dist,0)/255
                                diffuseval[2]=partobj.diff_intensity*partobj.diff_tint[2]*max(dist,0)/255
                                
                                #Specular shading
                                dot=rvector[0]*vvector[0]+rvector[1]*vvector[1]+rvector[2]*vvector[2]
                                specularval=[0,0,0]
                                specularval[0]=partobj.spec_intensity*partobj.spec_tint[0]*max(dot**partobj.spec_shine,0)*diffuseval[0]/255
                                specularval[1]=partobj.spec_intensity*partobj.spec_tint[1]*max(dot**partobj.spec_shine,0)*diffuseval[1]/255
                                specularval[2]=partobj.spec_intensity*partobj.spec_tint[2]*max(dot**partobj.spec_shine,0)*diffuseval[2]/255
                                
                                if self.displayRays:
                                    try:
                                        a_xx=centre[0]-j.x
                                        a_yy=centre[1]-j.y
                                        a_zz=centre[2]-j.z
                                        a_x=a_xx/a_zz*d+self.width/2
                                        a_y=a_yy/a_zz*d+self.height/2

                                        b_xx=centre[0]+lvector[0]-j.x
                                        b_yy=centre[1]+lvector[1]-j.y
                                        b_zz=centre[2]+lvector[2]-j.z
                                        b_x=b_xx/b_zz*d+self.width/2
                                        b_y=b_yy/b_zz*d+self.height/2

                                        pygame.draw.aaline(screen, self.edge_colour, (int(a_x),int(a_y)), (int(b_x),int(b_y)), 1)
                                        
                                    except ZeroDivisionError:
                                        pass

                                    try:
                                        a_xx=centre[0]-j.x
                                        a_yy=centre[1]-j.y
                                        a_zz=centre[2]-j.z
                                        a_x=a_xx/a_zz*d+self.width/2
                                        a_y=a_yy/a_zz*d+self.height/2

                                        b_xx=centre[0]+rvector[0]-j.x
                                        b_yy=centre[1]+rvector[1]-j.y
                                        b_zz=centre[2]+rvector[2]-j.z
                                        b_x=b_xx/b_zz*d+self.width/2
                                        b_y=b_yy/b_zz*d+self.height/2

                                        pygame.draw.aaline(screen, self.edge_colour, (int(a_x),int(a_y)), (int(b_x),int(b_y)), 1)
                                        
                                    except ZeroDivisionError:
                                        pass

                                #Calculating light acting on face
                                temp_col=[0,0,0]
                                temp_col[0]=lightobj.colour[0]*(diffuseval[0]+specularval[0])
                                temp_col[1]=lightobj.colour[1]*(diffuseval[1]+specularval[1])
                                temp_col[2]=lightobj.colour[2]*(diffuseval[2]+specularval[2])
                                
                                #Inverse squared
                                if self.light_distance:
                                    ldist=getDistance(centre,(lightobj.x,lightobj.y,lightobj.z))
                                    temp_col[0]/=(ldist/self.light_distance)**2
                                    temp_col[1]/=(ldist/self.light_distance)**2
                                    temp_col[2]/=(ldist/self.light_distance)**2

                                #Add on light
                                light[0]=min(255,temp_col[0]+light[0])
                                light[1]=min(255,temp_col[1]+light[1])
                                light[2]=min(255,temp_col[2]+light[2])

                            colour0=min(partobj.col[0]*(light[0]/255*(255-(self.min_brightness*self.min_colour[0]/100))+self.min_brightness*self.min_colour[0]/100)/255,255)
                            colour1=min(partobj.col[1]*(light[1]/255*(255-(self.min_brightness*self.min_colour[1]/100))+self.min_brightness*self.min_colour[1]/100)/255,255)
                            colour2=min(partobj.col[2]*(light[2]/255*(255-(self.min_brightness*self.min_colour[2]/100))+self.min_brightness*self.min_colour[2]/100)/255,255)

                            try:
                                pygame.draw.polygon(screen, (colour0,colour1,colour2), cornerlist, 0)
                            except TypeError:
                                pass

                            if self.displayNorms:
                                try:
                                    a_xx=centre[0]-j.x
                                    a_yy=centre[1]-j.y
                                    a_zz=centre[2]-j.z
                                    if a_zz<0:
                                            continue;
                                    a_x=a_xx/a_zz*d+self.width/2
                                    a_y=a_yy/a_zz*d+self.height/2

                                    b_xx=partobj.norm[0]+centre[0]-j.x
                                    b_yy=partobj.norm[1]+centre[1]-j.y
                                    b_zz=partobj.norm[2]+centre[2]-j.z
                                    if b_zz<0:
                                            continue;
                                    b_x=b_xx/b_zz*d+self.width/2
                                    b_y=b_yy/b_zz*d+self.height/2
                                except ZeroDivisionError:
                                    continue;

                                pygame.draw.aaline(screen, self.edge_colour, (int(a_x),int(a_y)), (int(b_x),int(b_y)), 1)
                    
                if self.displayLights:
                    for light in self.lights:
                        try:
                            xx=light.x-j.x
                            yy=light.y-j.y
                            zz=light.z-j.z
                            if zz<0:
                                    continue;
                            x=xx/zz*d+self.width/2
                            y=yy/zz*d+self.height/2
                        except ZeroDivisionError:
                            continue;

                        pygame.draw.circle(screen, self.node_colour, (int(x),int(y)), self.nodeRadius, 0)
                        
                self.gui.draw(self)
                pygame.display.flip()
                self._frame_time=self._get_ticks()-self._frame_start

    #Adds a light object
    def add_light(self,x=0,y=0,z=0,brightness=100,colour=white,light=None):
        if light==None:
            self.lights.append(Light(x=x,y=y,z=z,brightness=brightness,colour=colour))
        else:
            self.lights.append(light)

    #Outputs Objects
    def __str__(self):
        for key in self.objects:
            print("\n"+key)
            print(self.objects.get(key))
        return ""

    #Run whenever the colsole is called up
    def console(self):
        pygame.event.pump()
        restart=False
        disco=False
        while True:
            command = input(">>>")
            #Carrys out command
            if command == "output":
                print(self)
            elif command == "wireframe":
                if self.displayFaces:
                    self.displayNodes = 1
                    self.displayEdges = 1
                    self.displayFaces = 0
                else:
                    self.displayNodes = 0
                    self.displayEdges = 0
                    self.displayFaces = 1
            elif command == "quit" or command == "exit":
                self._done = True
            elif command == "debug":
                if self.debug:
                    self.debug = False
                else:
                    print(self)
                    self.debug = True
            elif command == "performance":
                print("FPS:", str(self.fps))
                print("TPS:", str(self.tps))
                print("UPS:", str(self.ups))
            elif command == "disco":
                disco=True
            elif command[0:3] == "set":
                variable = command [4:]
                value = input(">>>")
                if variable == "maxfps":
                    self.max_fps = int(value)
                elif variable == "tickspersecond":
                    self.ticks_per_second = int(value)
                elif variable == "maxframeskip":
                    self.max_frame_skip = int(value)
                elif variable == "use_wait":
                    self.use_wait = bool(value)
                elif variable == "fov":
                    self.fov = int(value)
                elif variable == "caption":
                    pygame.display.set_caption(value)
                elif variable == "debug":
                    self.debug = bool(value)
                elif variable == "automove":
                    self.auto_move = bool(value)
                elif variable == "nodes":
                    self.displayNodes =  int(value)
                elif variable == "edges":
                    self.displayEdges =  int(value)
                elif variable == "faces":
                    self.displayFaces =  int(value)
                elif variable == "lights":
                    self.displayLights =  int(value)
                elif variable == "norms":
                    self.displayNorms =  int(value)
                elif variable == "rays":
                    self.displayRays =  int(value)
                else:
                    print(variable.title(),"isn't a valid variable")
                    continue;
            else:
                print(command.title(),"is not a valid command!")
                continue;
            break;

        #Restarts game
        while not restart and not self._done:
            for event in pygame.event.get(pygame.VIDEOEXPOSE):
                if event.type == pygame.VIDEOEXPOSE:
                    restart = True
        if disco:
            for i in range(35):
                self._wait(0.3)
                self.screen.fill((random.randint(0,255),random.randint(0,255),random.randint(0,255)))
                pygame.display.flip()

    #Adds objects to the game
    def add_objects(self,objects,keys):
        for num in range(len(objects)):
            self.objects[keys[num]]=objects[num]

    def save_screen(self,name):
        pygame.image.save(self.screen, name)


#Basic Instance class
class Instance(object):
    def __init__(self,x,y,z,rotate_z,rotate_y,rotate_x):
        self.x=x
        self.y=y
        self.z=z
        self.rotate_z=rotate_z
        self.rotate_y=rotate_y
        self.rotate_x=rotate_x

#Basic object class
class ObjectBase(object):
    def __init__(self,x=0,y=0,z=0,rotate_z=0,rotate_y=0,rotate_x=0,sub_objects=[]):
        self.x=x
        self.y=y
        self.z=z
        self.rotate_z=rotate_z
        self.rotate_y=rotate_y
        self.rotate_x=rotate_x
        self.sub_objects=sub_objects

    def __str__(self):
        for i,obj in enumerate(self.sub_objects):
            print(str(i)+":")
            print(obj)
        return ""

    def update(self):
        pass

    def end_update(self):
        for i in self.sub_objects:
            i.x+=self.x
            i.y+=self.y
            i.z+=self.z
            i.obj_rotate_z=self.rotate_z
            i.obj_rotate_y=self.rotate_y
            i.obj_rotate_x=self.rotate_x
            i.obj_rotate=self.centre()
            i.update()
        for i in self.sub_objects:
            i.update_rotate()
        self.rotate_z=self.rotate_y=self.rotate_x=self.x=self.y=self.z=0

    def centre(self):
        num_sub=len(self.sub_objects)
        meanX=0
        meanY=0
        meanZ=0
        for i in self.sub_objects:
            x,y,z=i.centre()
            meanX+=x/num_sub
            meanY+=y/num_sub
            meanZ+=z/num_sub
        return (meanX,meanY,meanZ)

    def rotate(self,direction,c,degrees):
        if direction=='z':
            for i in self.sub_objects:
                i.rotate('z',c,degrees)
        elif direction=='y':
            for i in self.sub_objects:
                i.rotate('y',c,degrees)
        elif direction=='x':
            for i in self.sub_objects:
                i.rotate('x',c,degrees)

    def add_subobject(self,subobject):
        subobject.parent=self
        self.sub_objects.append(subobject)

    def scale(self, centre, scale):
        for i in self.sub_objects:
            i.scale(centre,scale)

#Rendering Components(Node, Edge and Face)

#Node Class
class Node(object):
    def __init__(self, coordinates):
        self.x = coordinates[0]
        self.y = coordinates[1]
        self.z = coordinates[2]
        self.camdist=None
        self.renderx=None
        self.rendery=None

#Edge Class
class Edge(object):
    def __init__(self, start, stop):
        self.start = start
        self.stop  = stop

#Face Class
class Face(object):
    def __init__(self,corners,colour,spec_shine=1,spec_intensity=1,spec_tint=white,diff_intensity=1,diff_tint=white):
        self.corners=corners
        self.col=colour
        self.spec_shine=spec_shine
        self.spec_intensity=spec_intensity
        self.spec_tint=spec_tint
        self.diff_intensity=diff_intensity
        self.diff_tint=diff_tint
        meanX = sum([node.x for node in self.corners]) / 3
        meanY = sum([node.y for node in self.corners]) / 3
        meanZ = sum([node.z for node in self.corners]) / 3
        center=(meanX,meanY,meanZ)
        vector0=[corners[0].x-center[0],corners[0].y-center[1],corners[0].z-center[2]]
        vector1=[corners[1].x-center[0],corners[1].y-center[1],corners[1].z-center[2]]
        norm=[0,0,0]
        norm[0]=((vector0[1]*vector1[2])-(vector0[2]*vector1[1]))*3
        norm[1]=((vector0[2]*vector1[0])-(vector0[0]*vector1[2]))*3
        norm[2]=((vector0[0]*vector1[1])-(vector0[1]*vector1[0]))*3
        self.norm=normalisel(norm)

#Basic sub-object class
class SubObjectBase(object):
    def __init__(self,parent=None,x=0,y=0,z=0,rotate_z=0,rotate_y=0,rotate_x=0):
        self.x=x
        self.y=y
        self.z=z
        self.rotate_z=rotate_z
        self.rotate_y=rotate_y
        self.rotate_x=rotate_x
        self.nodes=[]
        self.edges=[]
        self.faces=[]
        self.obj_rotate_z=0
        self.obj_rotate_y=0
        self.obj_rotate_x=0
        self.obj_rotate=[0,0,0]

    def update(self):
        if self.x:
            self.translate("x",self.x)
        if self.y:
            self.translate("y",self.y)
        if self.z:
            self.translate("z",self.z)
        self.x=0
        self.y=0
        self.z=0

    def update_rotate(self):
        if self.rotate_z:
            self.rotate('z',self.centre(),rotate_z)
        if self.rotate_y:
            self.rotate('y',self.centre(),rotate_y)
        if self.rotate_x:
            self.rotate('x',self.centre(),rotate_x)
        if self.obj_rotate_z:
            self.rotate('z',self.obj_rotate,self.obj_rotate_z)
        if self.obj_rotate_y:
            self.rotate('y',self.obj_rotate,self.obj_rotate_y)
        if self.obj_rotate_x:
            self.rotate('x',self.obj_rotate,self.obj_rotate_x)
        self.obj_rotate_z=0
        self.obj_rotate_y=0
        self.obj_rotate_x=0
        self.rotate_z=0
        self.rotate_y=0
        self.rotate_x=0

    def add_nodes(self, nodeList):
        try:
            for node in nodeList:
                self.nodes.append(Node(node))
        except:
            self.nodes.append(Node(nodeList))

    def add_edges(self, edgeList):
        try:
            for (start, stop) in edgeList:
                self.edges.append(Edge(self.nodes[start], self.nodes[stop]))
        except:
            self.edges.append(Edge(self.nodes[edgeList[0]], self.nodes[edgeList[1]]))

    def add_faces(self, faceList):
        try:
            for (corners,colour) in faceList:
                corner_list=[]
                for i in corners:
                    corner_list.append(self.nodes[i])
                for i in range(len(corner_list)-1):
                    self.edges.append(Edge(corner_list[i], corner_list[i+1]))
                try:
                    self.edges.append(Edge(corner_list[len(corner_list)-1], corner_list[0]))
                except:
                    pass
                for i in range(len(corner_list)-3):
                    self.edges.append(Edge(corner_list[0],corner_list[i+2]))
                for i in range(len(corner_list)-2):
                    self.faces.append(Face((corner_list[0],corner_list[i+1],corner_list[i+2]),colour))
        except:
            corners=faceList[0]
            colour=faceList[1]
            corner_list=[]
            for i in corners:
                corner_list.append(self.nodes[i])
            for i in range(len(corner_list)-1):
                self.edges.append(Edge(corner_list[i], corner_list[i+1]))
            self.edges.append(Edge(corner_list[len(corner_list)-1], corner_list[0]))
            for i in range(len(corner_list)-3):
                self.edges.append(Edge(corner_list[0],corner_list[i+2]))
            for i in range(len(corner_list)-2):
                self.faces.append(Face((corner_list[0],corner_list[i+1],corner_list[i+2]),colour))

    def edit_face(self,face,colour=None,spec_shine=None,spec_intensity=None,spec_tint=None,
                  diff_intensity=None,diff_tint=None,all_faces=False):
        if not all_faces:
            if not colour==None:
                self.faces[face].colour=colour
            if not spec_shine==None:
                self.faces[face].spec_shine=spec_shine
            if not spec_intensity==None:
                self.faces[face].spec_intensity=spec_intensity
            if not spec_tint==None:
                self.faces[face].spec_tint=spec_tint
            if not diff_intensity==None:
                self.faces[face].diff_intensity=diff_intensity
            if not diff_tint==None:
                self.faces[face].diff_tint=diff_tint
        else:
            for face in self.faces:
                if not colour==None:
                    face.colour=colour
                if not spec_shine==None:
                    face.spec_shine=spec_shine
                if not spec_intensity==None:
                    face.spec_intensity=spec_intensity
                if not spec_tint==None:
                    face.spec_tint=spec_tint
                if not diff_intensity==None:
                    face.diff_intensity=diff_intensity
                if not diff_tint==None:
                    face.diff_tint=diff_tint

    def add_mesh(self,filename):
        verts = []  
        norms = []
        faces = []
        for line in open(filename, "r"):  
            vals = line.split()
            if len(vals)==0:
                continue;
            if vals[0][0]=="#":
                continue;
            if vals[0] == "v":  
                v = vals[1:4]
                count=0
                for i in v:
                    v[count]=float(i)
                    count+=1
                verts.append(v)  
            if vals[0] == "vn":  
                n = vals[1:4]
                count=0
                for i in n:
                    n[count]=float(i)
                    count+=1
                norms.append(n)  
            if vals[0] == "f":
                v=[]
                n=[]
                for f in vals[1:]:  
                    w = f.split("/")
                    v.append(int(w[0])-1)
                    try:
                        n.append(int(w[2])-1)
                    except:
                        pass
                faces.append((v,n))
        self.add_nodes(verts)
        faceList=[]
        for i in faces:
            faceList.append((i[0],white))
        self.add_faces(faceList)
            

    def add_model(self,model):
        self.add_nodes(model[0])
        self.add_faces(model[1])

    def __str__(self):
        self.print_nodes()
        self.print_edges()
        self.print_faces()
        return ""

    def print_nodes(self):
        print(" --- Nodes --- ")
        for i, node in enumerate(self.nodes):
            print(" %d: (%.2f, %.2f, %.2f)" % (i, node.x, node.y, node.z))

    def print_edges(self):
        print("\n --- Edges --- ")
        for i, edge in enumerate(self.edges):
            print(" %d: (%.2f, %.2f, %.2f)" % (i, edge.start.x, edge.start.y, edge.start.z))

    def print_faces(self):
        print("\n --- Faces --- ")
        for i, face in enumerate(self.faces):
            print(" %d: (%.2f, %.2f, %.2f, %.2f)" % (i, face.num_corners, face.corners[0].x, face.corners[0].y, face.corners[0].z))

    def translate(self, axis, d):
        if axis in ['x', 'y', 'z']:
            for node in self.nodes:
                setattr(node, axis, getattr(node, axis) + d)

    def scale(self, centre, scale):
        centre_x=centre[0]
        centre_y=centre[1]
        centre_z=centre[2]
        for node in self.nodes:
            node.x = centre_x + scale * (node.x - centre_x)
            node.y = centre_y + scale * (node.y - centre_y)
            node.z = centre_z + scale * (node.z - centre_z)
        for face in self.faces:
            for tri in face.triangles:
                tri.norm[0] = centre_x + scale * tri.norm[0]
                tri.norm[1] = centre_y + scale * tri.norm[1]
                tri.norm[2] = centre_z + scale * tri.norm[2]

    def centre(self):
        num_nodes = len(self.nodes)
        meanX = sum([node.x for node in self.nodes]) / num_nodes
        meanY = sum([node.y for node in self.nodes]) / num_nodes
        meanZ = sum([node.z for node in self.nodes]) / num_nodes
        return (meanX,meanY,meanZ)

    def rotate(self, direction, c, degrees):
        if direction=='z':
            for node in self.nodes:
                x      = node.x - c[0]
                y      = node.y - c[1]
                d      = math.hypot(y, x)
                theta  = math.atan2(y, x) + math.radians(degrees)
                node.x = c[0] + d * math.cos(theta)
                node.y = c[1] + d * math.sin(theta)
            for face in self.faces:
                x      = face.norm[0]
                y      = face.norm[1]
                d      = math.hypot(y, x)
                theta  = math.atan2(y, x) + math.radians(degrees)
                face.norm[0] = d * math.cos(theta)
                face.norm[1] = d * math.sin(theta)
        if direction=='y':
            for node in self.nodes:
                y      = node.y - c[1]
                z      = node.z - c[2]
                d      = math.hypot(y, z)
                theta  = math.atan2(y, z) + math.radians(degrees)
                node.z = c[2] + d * math.cos(theta)
                node.y = c[1] + d * math.sin(theta)
            for face in self.faces:
                y      = face.norm[1]
                z      = face.norm[2]
                d      = math.hypot(y, z)
                theta  = math.atan2(y, z) + math.radians(degrees)
                face.norm[2] = d * math.cos(theta)
                face.norm[1] = d * math.sin(theta)
        if direction=='x':
            for node in self.nodes:
                x      = node.x - c[0]
                z      = node.z - c[2]
                d      = math.hypot(x, z)
                theta  = math.atan2(x, z) + math.radians(degrees)
                node.z = c[2] + d * math.cos(theta)
                node.x = c[0] + d * math.sin(theta)
            for face in self.faces:
                x      = face.norm[0]
                z      = face.norm[2]
                d      = math.hypot(x, z)
                theta  = math.atan2(x, z) + math.radians(degrees)
                face.norm[2] = d * math.cos(theta)
                face.norm[0] = d * math.sin(theta)
                
#Light class
class Light(object):
    def __init__(self,x=0,y=0,z=0,brightness=100,colour=white):
        self.x=x
        self.y=y
        self.z=z
        self.colour=(colour[0]*brightness/100,colour[1]*brightness/100,colour[2]*brightness/100)

    def update(self):
        pass



def getDistance(a,b):
    return math.hypot((max(a[0],b[0])-min(a[0],b[0])),math.hypot((max(a[1],b[1])-min(a[1],b[1])),(max(a[2],b[2])-min(a[2],b[2]))))

def clamp(x,a,b):
    if a<=b:
        return min(max(x,a),b)
    else:
        return min(max(x,b),a)

def convert_mesh(filename):
    print("Starting Initial Conversion...")
    print("Loading File...")
    text=""
    for line in open(filename, "r"):
        text+=line
    print("Done")
    count=0
    ln_start=0
    write=open(filename,"w")
    line_count=0
    for char in text:
        if char=="v" or char=="f":
            write.write(text[ln_start:count]+"\n")
            ln_start=count
            line_count+=1
            if line_count==1000:
                line_count=0
                write.close()
                print("Converting...")
                write=open(filename,"a")
        count+=1
    write.write(text[ln_start:count])
    write.close()
    print("Starting Optimisation...")
    print("Reading File...")
    verts = []  
    norms = []
    faces = []
    dupes = []
    for line in open(filename, "r"):  
        vals = line.split()
        if len(vals)==0:
            continue;
        if vals[0][0]=="#":
            continue;
        if vals[0] == "v":  
            v = vals[1:4]
            count=0
            for i in v:
                v[count]=float(i)
                count+=1
            if v in verts:
                count=0
                for i in verts:
                    if i==v:
                        dupes.append((len(verts),count))
                        break;
                    count+=1
            verts.append(v)  
        if vals[0] == "vn":  
            n = vals[1:4]
            count=0
            for i in n:
                n[count]=float(i)
                count+=1
            norms.append(n)  
        if vals[0] == "f":
            v=[]
            n=[]
            for f in vals[1:]:  
                w = f.split("/")
                notindupes=True
                for i in dupes:
                    if int(w[0])-1==i[0]:
                        v.append(i[1])
                        notindupes=False
                if notindupes:
                    v.append(int(w[0])-1)
                try:
                    n.append(int(w[2])-1)
                except:
                    pass
            faces.append((v,n))
    write=open(filename,"w")
    print("Adding identifier...")
    identifier=""
    identifier+="#"+filename+" optimised"
    write.write(identifier)
    print("Optimising Vertices...")
    count=0
    for i in verts:
        text=""
        text+="v "+str(i[0])+" "+str(i[1])+" "+str(i[2])
        write.write(text)
    print("Done")
    print("Optimising Normals...")
    print(str(len(norms)),"Unused Normals Removed")
    print("Done")
    print("Optimising Faces...")
    for i in faces:
        if len(i[0])>=3:
            text="f "
            count=0
            for j in i[0]:
                text+=str(j+1)+"//0 "
                count+=1
            text=text[:len(text)-1]
            write.write(text)
    print(str(len(dupes)),"Dupliacte Vertices Removed")
    print("Done")
    write.close()
    print("Finishing Conversion...")
    print("Reloading File...")
    text=""
    for line in open(filename, "r"):
        text+=line
    print("Done")
    count=0
    ln_start=0
    write=open(filename,"w")
    line_count=0
    for char in text:
        if char=="v" or char=="f":
            write.write(text[ln_start:count]+"\n")
            ln_start=count
            line_count+=1
            if line_count==1000:
                line_count=0
                write.close()
                print("Converting...")
                write=open(filename,"a")
        count+=1
    write.write(text[ln_start:count])
    write.close()
    input("Finished!\nPress enter to continue... ")

def magnitude(vector):
    return math.hypot(max(vector[0],-vector[0]),math.hypot(max(vector[1],-vector[1]),max(vector[2],-vector[2])))

def normalise(vector):
    try:
        mag=magnitude(vector)
        return (vector[0]/mag,vector[1]/mag,vector[2]/mag)
    except:
        return (0,0,0)

def normalisel(vector):
    try:
        mag=magnitude(vector)
        return [vector[0]/mag,vector[1]/mag,vector[2]/mag]
    except:
        return [0,0,0]

#Code for demo program
def main():
    print("Py3D Module")
    print("See Template.py for usage instructions and game1.py for an example program")
    while True:
        choice=input("Do you want to open the demo program Y/N? ")
        if choice!="N" and choice!="n" and choice!="y" and choice !="Y":
            print("That is not a valid choice")
            continue;
        elif choice=="Y" or choice=="y":
            input("The controls are q,w,e,a,s,d,u,j,m,i,k,l\nPress enter to continue")
            game=Game(fov=75,ticks_per_second=40,max_fps=120,use_wait=True,
                         update_callback=None,frame_callback=None,time_source=None,
                         min_brightness=5,min_colour=white, debug=False,
                          caption="Py3D Game")
            game.min_brightness=5
            game.min_colour=white
            class cube(ObjectBase):
                def update(self):
                    if pygame.key.get_pressed()[K_q]:
                        self.rotate_z=-4
                    if pygame.key.get_pressed()[K_w]:
                        self.rotate_y=4
                    if pygame.key.get_pressed()[K_e]:
                        self.rotate_z=4
                    if pygame.key.get_pressed()[K_a]:
                        self.rotate_x=4
                    if pygame.key.get_pressed()[K_s]:
                        self.rotate_y=-4
                    if pygame.key.get_pressed()[K_d]:
                        self.rotate_x=-4
                    if pygame.key.get_pressed()[K_i]:
                        self.y=-1
                    if pygame.key.get_pressed()[K_k]:
                        self.y=1
                    if pygame.key.get_pressed()[K_j]:
                        self.x=-1
                    if pygame.key.get_pressed()[K_l]:
                        self.x=1
                    if pygame.key.get_pressed()[K_u]:
                        self.z=-1
                    if pygame.key.get_pressed()[K_m]:
                        self.z=1
            cube=cube()
            cube_frame=cube.add_subobject(SubObjectBase())
            cube_frame.add_nodes(((15,15,15),(15,50,15),(50,15,15),(50,50,15),(50,50,50),(50,15,50),(15,50,50),(15,15,50)))
            cube_frame.add_faces((((0,1,3,2),white),((4,5,7,6),cyan),((0,2,5,7),red),((1,3,4,6),green),((0,1,6,7),blue),((2,3,4,5),yellow)))
            objects={}
            objects["cube"]=cube
            game.objects=objects
            game.add_light(x=0,y=0,z=0,brightness=100,colour=white)
            class TickHandler(object):
                def pre_update(self,game):
                    pass
                def post_update(self,game):
                    pass
            class GUI(object):
                def update(self,game):
                    pass
                def draw(self,game):
                    pass
            game.tick_handler=TickHandler()
            game.gui=GUI()
            game.loop()
            pygame.quit()
            break;
        elif choice=="N" or choice=="n":
            break;

if __name__=="__main__":
    main()
