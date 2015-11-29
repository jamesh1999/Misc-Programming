#Py3D.py
#Contains all code to render and run 3D games

#Imports required modules
import time,sys,math
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
    def __init__(self,fov=75,ticks_per_second=40,max_fps=120,use_wait=True,
                 update_callback=None,frame_callback=None,time_source=None,max_frame_skip=5,
                 min_brightness=100,min_colour=white, debug=False, caption="Py3D Game", gui=None,
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
        self.use_wait = use_wait
        self.dilation = 1.0
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
        self.frame_count = 0
        self.update_count = 0

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
    def use_wait(self):
        return self._use_wait
    @use_wait.setter
    def use_wait(self, enabled):
        self._use_wait = enabled
        self._tps = float(self.max_fps)

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
            self._ticks = (TIME - self.time) / self.dilation
            self._elapsed += self._ticks
            self.time = TIME

            if self._elapsed >= 1.0:
                self._elapsed %= 1.0
                self.tps = self._tps
                self._tps = 0
                self.fps = self.frame_count
                self.ups = self.update_count
                self.frame_count = self.update_count = 0

            self._tps += 1
            self._update_ready = self._frame_ready = False

            if TIME >= self._last_update+self._tick_step*self.dilation:
                self._update_ready = True

            if self.max_fps == 0:
                self._frame_ready = True
            elif self._use_wait and self._max_fps > 0:
                wait_sec = self._last_frame + self._frame_step - self._get_ticks()
                if wait_sec > 0:
                    self._wait(wait_sec)
                self._frame_ready = True
            elif not self._use_wait and self._max_fps > 0:
                if self._last_frame + self._frame_step < self._get_ticks():
                    self._frame_ready = True

            if self._update_ready:
                self.update_count += 1
                self._frames_skipped += 1
                self._last_update += self._tick_step
                drift = self._tick_step / 5.0
                if not (TIME-drift < self._last_update < TIME+drift):
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

            if self._frame_ready:
                self.frame_count += 1
                self._frames_skipped = 0
                if self._frame_step:
                    self._last_frame += self._frame_step
                    drift = self._frame_step * self._max_frame_skip
                    if not (TIME-drift < self._last_frame < TIME+drift):
                        self._last_frame = TIME
                #-----Start of render function-----#
                screen=self.screen
                screen.fill(self.background)
                d=self._scr_dist
                j=self.instance

                dist_list=[]
                for key in self.objects:
                    wireframe=self.objects.get(key)
                    dist=getDistance(wireframe.centre(),(j.x,j.y,j.z))
                    dist_list.append([key,dist])
                for tempobj in sorted(dist_list,key=itemgetter(1),reverse=True):
                    wireframe=self.objects.get(key)
                    for i in wireframe.sub_objects:
                        if self.displayEdges:
                            for edge in i.edges:
                                try:
                                    a_xx=edge.start.x-j.x
                                    a_yy=edge.start.y-j.y
                                    a_zz=edge.start.z-j.z
                                    if a_zz<0:
                                            continue;
                                    a_x=a_xx/a_zz*d+self.width/2
                                    a_y=a_yy/a_zz*d+self.height/2

                                    b_xx=edge.stop.x-j.x
                                    b_yy=edge.stop.y-j.y
                                    b_zz=edge.stop.z-j.z
                                    if b_zz<0:
                                            continue;
                                    b_x=b_xx/b_zz*d+self.width/2
                                    b_y=b_yy/b_zz*d+self.height/2
                                except ZeroDivisionError:
                                    continue;

                                pygame.draw.aaline(screen, self.edge_colour, (int(a_x),int(a_y)), (int(b_x),int(b_y)), 1)

                        if self.displayNodes:
                            for node in i.nodes:
                                try:
                                    xx=node.x-j.x
                                    yy=node.y-j.y
                                    zz=node.z-j.z
                                    if zz<0:
                                            continue;
                                    x=xx/zz*d+self.width/2
                                    y=yy/zz*d+self.height/2
                                except ZeroDivisionError:
                                    continue;

                                pygame.draw.circle(screen, self.node_colour, (int(x),int(y)), self.nodeRadius, 0)

                        if self.displayFaces:
                            avg_z=[]
                            k=0
                            for face in i.faces:
                                dist=0
                                for node in face.corners:
                                    dist+=math.hypot(math.hypot(max(node.x,j.x)-min(node.x,j.x),max(node.z,j.z)-min(node.z,j.z)),max(node.y,j.y)-min(node.y,j.y))/face.num_corners
                                avg_z.append([k,dist])
                                k+=1
                            for temp in sorted(avg_z,key=itemgetter(1),reverse=True):
                                if temp[1]>self.render_distance:
                                    continue;
                                face=i.faces[temp[0]]
                                for tri in face.triangles:
                                    meanX = sum([node.x for node in tri.corners]) / 3
                                    meanY = sum([node.y for node in tri.corners]) / 3
                                    meanZ = sum([node.z for node in tri.corners]) / 3
                                    centre=[meanX,meanY,meanZ]
                                    cornerlist=[]
                                    for node in tri.corners:
                                        try:
                                            xx=node.x-j.x
                                            yy=node.y-j.y
                                            zz=node.z-j.z
                                            if zz<0:
                                                continue;
                                            x=xx/zz*d+self.width/2
                                            y=yy/zz*d+self.height/2
                                            cornerlist.append((x,y))
                                        except ZeroDivisionError:
                                            zz=0
                                            continue;
                                    if not len(cornerlist)==len(tri.corners):
                                        continue;
                                    light=[0,0,0]
                                    for lightobj in self.lights:
                                        norm=[tri.norm[0]+centre[0],tri.norm[1]+centre[1],tri.norm[2]+centre[2]]
                                        ldist=getDistance(centre,(lightobj.x,lightobj.y,lightobj.z))
                                        nldist=getDistance(norm,(lightobj.x,lightobj.y,lightobj.z))
                                        angle=math.degrees(math.acos(clamp((tri.ndist**2+ldist**2-nldist**2)/(2*tri.ndist*ldist),-1,1)))
                                        if angle>=90:
                                            continue;
                                        #Specular shading
                                        lvector=[lightobj.x-centre[0],lightobj.y-centre[1],lightobj.z-centre[2]]
                                        dist=lvector[0]*tri.norm[0]+lvector[1]*tri.norm[1]+lvector[2]*tri.norm[2]
                                        mlight=[lightobj.x-2*dist*tri.norm[0],lightobj.y-2*dist*tri.norm[1],lightobj.z-2*dist*tri.norm[2]]
                                        reflected_light=[centre[0]+(centre[0]-mlight[0]),centre[1]+(centre[1]-mlight[1]),centre[2]+(centre[2]-mlight[2])]
                                        cam_dist=getDistance((j.x,j.y,j.z),centre)
                                        caml_dist=getDistance((j.x,j.y,j.z),reflected_light)
                                        refldist=getDistance(centre,reflected_light)
                                        rayangle=180-math.degrees(math.acos(clamp((cam_dist**2+refldist**2-caml_dist**2)/(2*cam_dist*refldist),-1,1)))
                                        dispersion_angle=180*(1-face.reflect)
                                        try:
                                            fraction_light=rayangle/180*(180-dispersion_angle)/dispersion_angle+1
                                        except:
                                            fraction_light=rayangle/180*(180-dispersion_angle)+1
                                        if self.displayRays:
                                            print
                                            try:
                                                a_xx=centre[0]-j.x
                                                a_yy=centre[1]-j.y
                                                a_zz=centre[2]-j.z
                                                a_x=a_xx/a_zz*d+self.width/2
                                                a_y=a_yy/a_zz*d+self.height/2

                                                b_xx=lightobj.x-j.x
                                                b_yy=lightobj.y-j.y
                                                b_zz=lightobj.z-j.z
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

                                                b_xx=reflected_light[0]-j.x
                                                b_yy=reflected_light[1]-j.y
                                                b_zz=reflected_light[2]-j.z
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

                                                b_xx=mlight[0]-j.x
                                                b_yy=mlight[1]-j.y
                                                b_zz=mlight[2]-j.z
                                                b_x=b_xx/b_zz*d+self.width/2
                                                b_y=b_yy/b_zz*d+self.height/2

                                                pygame.draw.aaline(screen, self.edge_colour, (int(a_x),int(a_y)), (int(b_x),int(b_y)), 1)
                                                
                                            except ZeroDivisionError:
                                                pass
                                        
                                        #Light density shading
                                        fraction_light=(fraction_light*face.reflect)+(math.sin(math.radians(90-angle))*(1-face.reflect))
                                        #Inverse squared
                                        if self.light_distance:
                                            fraction_light/=(ldist/self.light_distance)**2
                                        #Getting colour
                                        temp_col=[lightobj.colour[0]*lightobj.brightness*fraction_light/100,lightobj.colour[1]*lightobj.brightness*fraction_light/100,lightobj.colour[2]*lightobj.brightness*fraction_light/100]
                                        light[0]=min(255,temp_col[0]+light[0])
                                        light[1]=min(255,temp_col[1]+light[1])
                                        light[2]=min(255,temp_col[2]+light[2])
                                    colour0=min(face.col[0]*(light[0]/255*(255-(self.min_brightness*self.min_colour[0]/100))+self.min_brightness*self.min_colour[0]/100)/255,255)
                                    colour1=min(face.col[1]*(light[1]/255*(255-(self.min_brightness*self.min_colour[1]/100))+self.min_brightness*self.min_colour[1]/100)/255,255)
                                    colour2=min(face.col[2]*(light[2]/255*(255-(self.min_brightness*self.min_colour[2]/100))+self.min_brightness*self.min_colour[2]/100)/255,255)
                                    pygame.draw.polygon(screen, (colour0,colour1,colour2), cornerlist, 0)

                                    if self.displayNorms:
                                        try:
                                            a_xx=centre[0]-j.x
                                            a_yy=centre[1]-j.y
                                            a_zz=centre[2]-j.z
                                            if a_zz<0:
                                                    continue;
                                            a_x=a_xx/a_zz*d+self.width/2
                                            a_y=a_yy/a_zz*d+self.height/2

                                            b_xx=tri.norm[0]+centre[0]-j.x
                                            b_yy=tri.norm[1]+centre[1]-j.y
                                            b_zz=tri.norm[2]+centre[2]-j.z
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

    #Adds a light object
    def add_light(self,x=0,y=0,z=0,brightness=100,colour=white):
        self.lights.append(Light(x=x,y=y,z=z,brightness=brightness,colour=colour))

    #Outputs Objects
    def __str__(self):
        for key in self.objects:
            print("\n"+key)
            print(self.objects.get(key))
        return ""

    #Run whenever the colsole is called up
    def console(self):
        pygame.event.pump()
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
            else:
                print(command.title(),"is not a valid command!")
                continue
            break

        #Restarts game
        restart = False
        while not restart and not self._done:
            for event in pygame.event.get(pygame.VIDEOEXPOSE):
                if event.type == pygame.VIDEOEXPOSE:
                    restart = True

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

#Edge Class
class Edge(object):
    def __init__(self, start, stop):
        self.start = start
        self.stop  = stop

#Face Class
class Face(object):
    def __init__(self,corners,colour,reflect=0):
        self.corners=corners
        self.col=colour
        self.num_corners=len(corners)
        self.triangles=[]
        self.reflect=reflect
        for i in range(len(corners)-2):
            self.triangles.append(Triangle((corners[0],corners[i+1],corners[i+2])))

class Triangle(object):
    def __init__(self,corners):
        self.corners=corners
        meanX = sum([node.x for node in self.corners]) / 3
        meanY = sum([node.y for node in self.corners]) / 3
        meanZ = sum([node.z for node in self.corners]) / 3
        center=(meanX,meanY,meanZ)
        vector0=[corners[0].x-center[0],corners[0].y-center[1],corners[0].z-center[2]]
        vector1=[corners[1].x-center[0],corners[1].y-center[1],corners[1].z-center[2]]
        self.norm=[0,0,0]
        self.norm[0]=((vector0[1]*vector1[2])-(vector0[2]*vector1[1]))*3
        self.norm[1]=((vector0[2]*vector1[0])-(vector0[0]*vector1[2]))*3
        self.norm[2]=((vector0[0]*vector1[1])-(vector0[1]*vector1[0]))*3
        mag=magnitude(self.norm)
        self.norm[0]/=mag
        self.norm[1]/=mag
        self.norm[2]/=mag
        self.ndist=getDistance(self.norm,[0,0,0])

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
                self.edges.append(Edge(corner_list[len(corner_list)-1], corner_list[0]))
                for i in range(len(corner_list)-3):
                    self.edges.append(Edge(corner_list[0],corner_list[i+2]))
                self.faces.append(Face(corner_list,colour))
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
            self.faces.append(Face(corner_list,colour))

    def add_mesh(self,filename):
        verts = []  
        norms = []  
        vertsOut = []  
        normsOut = []
        dupes={}
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
                            break;
                        count+=1
                    dupes[str(v)]=count
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
                    if int(w[0]) in dupes:
                        v.append(dupes[int(w[0])])
                    else:
                        v.append(verts[int(w[0])-1])
                    try:
                        n.append(norms[int(w[2])-1])
                    except:
                        pass
                vertsOut.append(v)  
                normsOut.append(n)  
        count=0
        faces=[]
        for i in vertsOut:
            self.add_nodes(i)
            faces.append((count,count+len(i)))
            count+=len(i)
        faceList=[]
        for i in faces:
            l=[]
            for j in range(i[1]-i[0]):
                l.append(j+i[0])
            faceList.append((l,white))
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
                for tri in face.triangles:
                    x      = tri.norm[0]
                    y      = tri.norm[1]
                    d      = math.hypot(y, x)
                    theta  = math.atan2(y, x) + math.radians(degrees)
                    tri.norm[0] = d * math.cos(theta)
                    tri.norm[1] = d * math.sin(theta)
        if direction=='y':
            for node in self.nodes:
                y      = node.y - c[1]
                z      = node.z - c[2]
                d      = math.hypot(y, z)
                theta  = math.atan2(y, z) + math.radians(degrees)
                node.z = c[2] + d * math.cos(theta)
                node.y = c[1] + d * math.sin(theta)
            for face in self.faces:
                for tri in face.triangles:
                    y      = tri.norm[1]
                    z      = tri.norm[2]
                    d      = math.hypot(y, z)
                    theta  = math.atan2(y, z) + math.radians(degrees)
                    tri.norm[2] = d * math.cos(theta)
                    tri.norm[1] = d * math.sin(theta)
        if direction=='x':
            for node in self.nodes:
                x      = node.x - c[0]
                z      = node.z - c[2]
                d      = math.hypot(x, z)
                theta  = math.atan2(x, z) + math.radians(degrees)
                node.z = c[2] + d * math.cos(theta)
                node.x = c[0] + d * math.sin(theta)
            for face in self.faces:
                for tri in face.triangles:
                    x      = tri.norm[0]
                    z      = tri.norm[2]
                    d      = math.hypot(x, z)
                    theta  = math.atan2(x, z) + math.radians(degrees)
                    tri.norm[2] = d * math.cos(theta)
                    tri.norm[0] = d * math.sin(theta)
                
#Light class
class Light(object):
    def __init__(self,x=0,y=0,z=0,brightness=100,colour=white):
        self.x=x
        self.y=y
        self.z=z
        self.brightness=brightness
        self.colour=colour

    def update(self):
        pass



def getDistance(a,b):
    return math.sqrt((max(a[0],b[0])-min(a[0],b[0]))**2+(max(a[1],b[1])-min(a[1],b[1]))**2+(max(a[2],b[2])-min(a[2],b[2]))**2)

def clamp(x,a,b):
    if a<=b:
        return min(max(x,a),b)
    else:
        return min(max(x,b),a)

def convert_mesh(filename):
    text=""
    for line in open(filename, "r"):
        text+=line
    print("Done")
    count=0
    ln_start=0
    write=open(filename,"w")
    line_count=0
    for char in text:
        if char=="v" or "f":
            write.write(text[ln_start:count])
            ln_start=count
            line_count+=1
            if line_count==10000:
                line_count=0
                write.close()
                print("Converting...")
                write=open(filename,"a")
        count+=1
    write.close()

def magnitude(vector):
    return math.sqrt(max(vector[0],-vector[0])+max(vector[1],-vector[1])+max(vector[2],-vector[2]))

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
