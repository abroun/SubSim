"""
Defines PlayerRobot, a subclass of robot

(c) 2005, PyrobRobotics.org. Licenced under the GNU GPL.
"""

__author__ = "Douglas Blank <dblank@brynmawr.edu>"
__version__ = "$Revision: 1.99 $"

from pyrobot.robot import *
from math import pi, cos, sin
import threading, time, Tkinter
from os import getuid
from pyrobot.robot.device import *
from pyrobot.geometry import PIOVER180, DEG90RADS, COSDEG90RADS, SINDEG90RADS
import playerc

class PlayerDevice(Device):
    def __init__(self, client, type, groups = {},
                 mode=None, index=0, visible=0):
        Device.__init__(self, type, visible=visible)
        self.groups = groups
        self._client = client
        self.index = index
        self.data = None
        self._noise = 0.0
        # Required:
        self.startDevice(mode)
        if "get_geom" in self._client.__dict__:
            self._client.get_geom() # reads it into handle.pose or poses
    def __getattr__(self, item):
        if item in dir(self._dev):
            return self._dev.__getattribute__(item)
        else:
            raise AttributeError
    def startDevice(self, mode):
        dtype = self.type
        if self.type == "position":
            dtype = "position3d"

        #exec("self._dev = playerc.playerc_%s(self._client, %d)" %
        #     (dtype, self.index))

        #mode = playerc.PLAYERC_OPEN_MODE
        #retval = self._dev.subscribe(mode)
        #if retval != 0:
        #    raise playerc.playerc_error_str()

        print self._client

        pos3d = playerc.playerc_position3d( self._client, 0 )
        if pos3d.subscribe( playerc.PLAYERC_OPEN_MODE ) != 0:
            print playerc.playerc_error_str()
            raise 0

        self._dev = pos3d
        print "Created Device!!!!!!!"

        Device.startDevice(self)
        # robot.blobfinder.blobs[0].x, y, top, left, range, right, color,
        #   area, bottom,
        # color is a pointer
        # robot.blobfinder.blob_count, height, width
    def getDeviceData(self, pos = 0):
        return self.getPose()  #self._dev.scan[0]
    
    def getPose(self):
        """ Move the device. x, y are in meters """
        x, y, th = self._dev.px, self._dev.py, self._dev.pa
        return x, y, th / PIOVER180
    def setPose(self, xM, yM, thDeg):
        """ Move the device. x, y are in meters """
        function = self._client.__class__.__dict__["set_cmd_pose"]
        if function != None:
            return function( self._client, xM * 1000.0, yM * 1000.0, 0.0, thDeg % 360)
        else:
            raise DeviceError, "Function 'setPose' is not available for device '%s'" % self.type

class PlayerSimulationDevice(PlayerDevice):
    def __init__(self, hostname):
        self._client = playerc.playerc_client(None, hostname, 7000)
        self._client.connect()
        PlayerDevice.__init__(self, self._client, "simulation")
        #self.thread = PlayerUpdater(self)
        #self.thread.start()
    def setPose(self, name, x, y, deg):
        th = deg * PIOVER180
        #self._dev.set_pose2d(name, x, y, th)
        print "setPose not implemented"
    def getPose(self, name):
        print "getPose not implemented"
        return 0.0, 0.0, 0.0
        #result, x, y, thr = self._dev.get_pose2d(name)
        #if result == 0: # no errors
        #    return x, y, (thr / PIOVER180)
        #else:
        #    raise "simulation.getPose() failed"

class PlayerFiducialDevice(PlayerDevice):
    def __init__(self, client):
        PlayerDevice.__init__(self, client, "fiducial", mode=playerc.PLAYERC_OPEN_MODE)

        self.count = len(self)
        self.id = self.getFiducialsById
        self.idlist = self.getFiducialIDs

    def getFiducialsById(self, id):
        for f in self._dev.fiducials:
            if f.id==id:
                return f
        return

    def getFiducialIDs(self):
        return [f.id for f in self._dev.fiducials]

    def getDeviceData(self):
        fs = self._dev.fiducials
        return fs

    def __len__(self):
        return len(self._dev.fiducials)

class PlayerSpeechDevice(SpeechDevice, PlayerDevice):
    def __init__(self, client):
        PlayerDevice.__init__(self, client, "speech", visible = 0)

class PlayerGraphics2dDevice(PlayerDevice):
    def __init__(self, client):
        PlayerDevice.__init__(self, client, "graphics2d", visible = 0)
    def addWidgets(self, window):
        window.addCommand("draw", "Draw Polygon!", "[(0,0), (1,1), (1,-1)]", lambda s: self.drawPolygon(eval(s)))
        window.addButton("clear", "Clear!", self.clear)
    def drawPolygon(self, lyst, something = 4, fill = 1, color = (255, 0, 0, 0)):
        # points
        # something, fill color?
        # fill?
        # line color red, green, blue, alpha
        return self.draw_polygon(lyst, something, fill, color)
    # 'clear', 'color', 'destroy', 'draw_points', 'draw_polygon',
    # 'draw_polyline', 'info', 'setcolor', 

class PlayerBumperDevice(PlayerDevice):
    def __init__(self, client):
        PlayerDevice.__init__(self, client, "bumper", visible = 0)
        self._dev.get_geom() # stores it in self._dev.poses[]
        self.groups = {'all': range(len(self)),
                       'front': range(0, len(self)/2 + 1),
                       'back': range(len(self)/2 + 1, len(self)),
                       }
        self.units    = "RAW"
        # What are the raw units?
        # Anything that you pass to rawToUnits should be in these units
        self.rawunits = "RAW"
        self.maxvalueraw = 1.0 # meters
        # These are fixed in meters: DO NOT CONVERT ----------------
        self.radius = 0.40 # meters
        # ----------------------------------------------------------
        # All of the rest of the measures are relative to units, given in rawunits:
        self._noise = 0.00 # 5 percent
        self.count = len(self)
        self.arc = 7.5 * PIOVER180 #??

    def __len__(self):
        return len(self._dev.bumpers)

    def getSensorValue(self, pos):
        z = 0.23 # height
        return SensorValue(self, self._dev.bumpers[pos], pos,
                           (self._dev.poses[pos].pose.px, # x in meters
                            self._dev.poses[pos].pose.py, # y
                            z,                    # z
                            self._dev.poses[pos].pose.pa, # rads
                            self.arc),               # rads
                           noise=self._noise)
	
class PlayerSonarDevice(PlayerDevice):
    def __init__(self, client):
        PlayerDevice.__init__(self, client, "sonar")
        self._dev.get_geom() # stores it in self._dev.poses[]
        while len(self) == 0: pass
        if len(self) == 16:
            self.groups = {'all': range(16),
                           'front': (3, 4),
                           'front-left' : (1,2,3),
                           'front-right' : (4, 5, 6),
                           'front-all' : (1,2, 3, 4, 5, 6),
                           'front-to-back-left': (3, 2, 1, 0, 15, 14, 13, 12), # in order
                           'front-to-back-right': (4, 5, 6, 7, 8, 9, 10, 11), # in order
                           'left' : (0, 15), 
                           'right' : (7, 8), 
                           'left-front' : (0,), 
                           'right-front' : (7, ),
                           'left-back' : (15, ),
                           'right-back' : (8, ),
                           'back-right' : (9, 10, 11),
                           'back-left' : (12, 13, 14), 
                           'back' : (11, 12),
                           'back-all' : ( 9, 10, 11, 12, 13, 14)}
        elif len(self) == 32: # people bot says 32, but only 24
            self.groups = {'all': range(24),
                           'front': (3,4, 19,20),
                           'front-left' : (1,2,3, 17,18,19),
                           'front-right' : (4,5,6, 20,21,22),
                           'front-all' : (1,2,3,4,5,6, 17,18,19,20,21,22),
                           'left' : (0,15, 16), 
                           'right' : (7,8, 23), 
                           'left-front' : (0, 16), 
                           'right-front' : (7, 23),
                           'left-back' : (15, ),
                           'right-back' : (8, ),
                           'back-right' : (9, 10, 11),
                           'back-left' : (12, 13, 14), 
                           'back' : (11, 12),
                           'back-all' : ( 9, 10, 11, 12, 13, 14)}
        else:
            self.groups= {'all': range(len(self))}
        self.units    = "ROBOTS"
        # What are the raw units?
        # Anything that you pass to rawToUnits should be in these units
        self.rawunits = "METERS"
        self.maxvalueraw = 8.0 # meters
        # These are fixed in meters: DO NOT CONVERT ----------------
        self.radius = 0.40 # meters
        # ----------------------------------------------------------
        # All of the rest of the measures are relative to units, given in rawunits:
        self._noise = 0.05 # 5 percent
        self.count = len(self)
        self.arc = 7.5 * PIOVER180
    def __len__(self):
        return self._dev.scan_count
    def getSensorValue(self, pos):
        if pos < 16: z = 0.23
        else:        z = 1.1
        return SensorValue(self, self._dev.scan[pos], pos,
                           (self._dev.poses[pos].px, # x in meters
                            self._dev.poses[pos].py, # y
                            z,                    # z
                            self._dev.poses[pos].pa, # rads
                            self.arc),               # rads
                           noise=self._noise)

class PlayerLaserDevice(PlayerDevice):
    def __init__(self, client):
        PlayerDevice.__init__(self, client, "laser")
        while len(self) == 0: pass # wait for data to be loaded
        count = len(self)
        part = int(count/8)
        start = 0
        posA = part
        posB = part * 2
        posC = part * 3
        posD = part * 4
        posE = part * 5
        posF = part * 6
        posG = part * 7
        end = count
        self.groups = {'all': range(count),
                       'right': range(0, posB),
                       'left': range(posF, end),
                       'front': range(posC, posE),
                       'front-right': range(posB, posD),
                       'front-left': range(posD, posF),
                       'front-all': range(posB, posF),
                       'right-front': range(posA, posB),
                       'right-back': range(start, posA),
                       'left-front': range(posF,posG),
                       'left-back': range(posG,end),
                       'back-right': [],
                       'back-left': [],
                       'back': [],
                       'back-all': []}
        self.units    = "ROBOTS"
        self.arc      = 1.0 * PIOVER180 # in radians
        self._noise    = 0.01
        # -------------------------------------------
        self.rawunits = "METERS"
        self.maxvalueraw = 8.0 # rawunits
        # -------------------------------------------
        # These are fixed in meters: DO NOT CONVERT ----------------
        self.radius = 0.40 # meters
        # -------------------------------------------
        self.count = count
    def __len__(self):
        return self._dev.scan_count
    def getSensorValue(self, pos):
        return SensorValue(self, self._dev.ranges[pos],
                           pos,
                           (self._dev.pose[0],
                            self._dev.pose[1],
                            0.03,
                            (pos - 90) * PIOVER180,
                            self.arc),
                           noise=self._noise)
    def addWidgets(self, window):
        for i in range(0, self.count, 10):
            window.addData(str(i), "[%d]:" % i, self._dev.ranges[i])
    def updateWindow(self):
        if self.visible:
            for i in range(0, self.count, 10):
                self.window.updateWidget(str(i), "%.2f" % self[i].value)

class PlayerCommDevice(PlayerDevice):
    def __init__(self, client, name):
        PlayerDevice.__init__(self, client, name)
        self.messages = []
    
    def sendMessage(self, message):
        if self._client.comms == {}:
            print "Need to startDevice('comm') in robot: message not sent"
            return
        self._client.send_message(message)

    def getMessages(self):
        if not 'comms' in dir(self._client) or self._client.comms == {}:
            raise DeviceError, "Need to startDevice('comm') in robot"
        #if self._client.comms[0] != '':
        #    self.update() # this is update in robot
        tmp = self.messages
        # reset queue:
        self.messages = []
        return tmp

    def updateDevice(self):
        for i in self._client.comms:
            msg = self._client.get_comms()
            if msg:
                self.messages.append( msg )

class PlayerPositionDevice(PlayerDevice):
    def __init__(self, client):
        PlayerDevice.__init__(self, client, "position")
    def addWidgets(self, window):
        window.addData("x", ".x:", self._dev.px)
        window.addData("y", ".y:", self._dev.py)
        window.addData("thr", ".th (angle in radians):", self._dev.pa)
        window.addData("th", ".thr (angle in degrees):", self._dev.pa / PIOVER180)
        window.addData("stall", ".stall:", self._dev.stall)
    def updateWindow(self):
        if self.visible:
            self.window.updateWidget("x", self._dev.px)
            self.window.updateWidget("y",self._dev.py)
            self.window.updateWidget("thr", self._dev.pa)
            self.window.updateWidget("th", self._dev.pa / PIOVER180)
            self.window.updateWidget("stall",self._dev.stall)

class PlayerPTZDevice(PlayerDevice):
    def __init__(self, client, name):
        PlayerDevice.__init__(self, client, name)
        self.subtype = "Canon VC-C50i"
        self.origPose = (0, 0, 0)
        self.pose = [self.origPose[0], self.origPose[1], self.origPose[2]]
        self.supports = ["pan", "tilt", "zoom"]

    def init(self):
        self.setPose( *self.origPose )

    def setPose(self, *args):
        if len(args) == 3:
            self.pose = [args[0], args[1], args[2]]
        elif len(args) == 1 and len(args[0]) == 3:
            self.pose = [args[0][0], args[0][1], args[0][2]]
        else:
            raise AttributeError, "setPose takes pan, tilt, and zoom"
        if self.pose[0] < self.getMaxNegPan():
	    self.pose[0] = self.getMaxNegPan()
	if self.pose[0] > self.getMaxPosPan():
	    self.pose[0] = self.getMaxPosPan()
	if self.pose[1] < self.getMaxNegTilt():
	    self.pose[1] = self.getMaxNegTilt()
	if self.pose[1] > self.getMaxPosTilt():
	    self.pose[1] = self.getMaxPosTilt()
	if self.pose[2] < self.getMinZoom():
	    self.pose[2] = self.getMinZoom()
	if self.pose[2] > self.getMaxZoom():
	    self.pose[2] = self.getMaxZoom()

        return self._dev.set(self.pose[0], self.pose[1], self.pose[2])

    def getPose(self):
	return self.pose

    def pan(self, panVal):
	newPose = (panVal, self.pose[1], self.pose[2])
	return self.setPose(*newPose)

    def tilt(self, tiltVal):
	newPose = (self.pose[0], tiltVal, self.pose[2])
	return self.setPose(*newPose) 

    def zoom(self, zoomVal):
	newPose = (self.pose[0], self.pose[1], zoomVal)
	return self.setPose(*newPose)

    def center(self):
	return self.setPose(*self.origPose)

    def canGetRealPanTilt(self):
        return 0

    def canGetRealZoom(self):
        return 0

    def getMaxPosPan(self):
        if self.subtype == "Canon VC-C50i":
            return 2
        else:
            return 100
    def getMaxNegPan(self):
        if self.subtype == "Canon VC-C50i":
            return -2
        else:
            return -100
    def getMaxPosTilt(self):
        if self.subtype == "Canon VC-C50i":
            return 1.5
        else:
            return 30
    def getMaxNegTilt(self):
        if self.subtype == "Canon VC-C50i":
            return -0.5
        else:
            return -30
    def getMaxZoom(self):
        if self.subtype == "Canon VC-C50i":
            return 34
        else:
            return 10
    def getMinZoom(self):
        if self.subtype == "Canon VC-C50i":
            return 0
        else:
            return 120
    def addWidgets(self, window):
        p, t, z = 0, 0, 0
        window.addCommand("pan", "Pan!", str(p), lambda p: self.pan(float(p)))
        window.addCommand("tilt", "Tilt!", str(t), lambda t: self.tilt(float(t)))
        window.addCommand("zoom", "Zoom!", str(z), lambda z: self.zoom(float(z)))

class PlayerMapDevice(PlayerDevice):
    def __init__(self, client):
        PlayerDevice.__init__(self, client, "map", visible = 0)
        self.get_map()
        # Makes these available: 'cells', 'destroy', 'get_map', 'get_vector',
        # 'height', 'info', 'num_segments', 'origin', 'resolution', 'segments',
        # 'subscribe', 'this', 'unsubscribe', 'vmaxx', 'vmaxy', 'vminx',
        # 'vminy', 'width'

class PlayerGripperDevice(PlayerDevice, GripperDevice):
    def __init__(self, client):
        PlayerDevice.__init__(self, client, "gripper", visible = 0)

    def open(self):
        return self._dev.set_cmd(1, 0) 

    def close(self):
        return self._dev.set_cmd(2, 0) 

    def up(self):
        return self._dev.set_cmd(4, 0) 

    def down(self):
        return self._dev.set_cmd(5, 0) 

    #def stop(self):
    #    return self._dev.set_cmd(6, 0) 

    def store(self):
        #return self._dev.set_cmd(7, 0)
        self.close()
        self.up()

    def deploy(self):
        #return self._dev.set_cmd(8, 0)
        self.down()
        self.close()

    #def halt(self):
    #    return self._dev.set_cmd(15, 0) 

    def getState(self):
        return self._dev.state

    def getBreakBeam(self, name):
        if name == "inner":
            return self._dev.inner_break_beam
        elif name == "outer":
            return self._dev.outer_break_beam
        else:
            raise "no such gripper beam name: '%s'" % name

    def getBreakBeamState(self):
        sum = 0
        if self._dev.beams & 8: # back
            sum += 2
        if self._dev.beams & 4: # front
            sum += 1
        return sum

    def isClosed(self): 
        return self._dev.paddles_closed

    def isOpened(self): 
        return self._dev.paddles_open

    def isMoving(self):
        return self._dev.paddles_moving

    def isLiftMoving(self):
        return self._dev.lift_moving

class PlayerUpdater(threading.Thread):
    """
    """
    def __init__(self, runable):
        """
        Constructor, setting initial variables
        """
        self.runable = runable
        self._stopevent = threading.Event()
        self._sleepperiod = 0.01
        # We have to read it this fast to keep up!
        threading.Thread.__init__(self, name="PlayerUpdater")
        
    def run(self):
        """
        overload of threading.thread.run()
        main control loop
        """
        while not self._stopevent.isSet():
            id = 0  #self.runable._client.read()
            self._stopevent.wait(self._sleepperiod)

    def join(self,timeout=None):
        """
        Stop the thread
        """
        self._stopevent.set()
        threading.Thread.join(self, timeout)

class PlayerRobot(Robot):
    def __init__(self, name = "Player", port = 6665, hostname = 'localhost',
                 startDevices = 1):
        Robot.__init__(self) # robot constructor
        self.scale = 0.5
        self.last_rotate = 0.0
        self.last_translate = 0.0
        self.simulated = 1 # FIX: how can you tell? See drivername below
        self.hostname = hostname
        self.port = port
        self.name = name
        self.connect() # set self._client to player robot
        self.thread = PlayerUpdater(self)
        self.thread.start()

        # a robot with no devices will hang here!
        #while self._client.get_devlist() == -1: pass
        #self.builtinDevices = []
        #for i in range(0, self._client.devinfo_count):
        #    devinfo =  self._client.devinfos[i]
        #    devname = playerc.playerc_lookup_name(devinfo.addr.interf)
        #    #devinfo.addr.robot: 6665
        #    #devinfo.addr.index: 0
        #    #devinfo.drivername: "stage"
        #    if devname == "position2d":
        #        self.builtinDevices.append("position")
        #    else:
        #        self.builtinDevices.append(devname)
        #if "blobfinder" in self.builtinDevices:
        #    self.builtinDevices.append( "camera" )
        
        self.builtinDevices.append( "position" )
        self.builtinDevices.append( "simulation" )
        if startDevices:
            for device in ["position", "laser", "ir", "sonar", "bumper",
                      "speech", "graphics2d", "gripper"]:
                #is it supported? if so start it up:
                if device in self.builtinDevices:
                    #try: # this is for gazebo; can't tell what it really has
                    deviceName = self.startDevice(device)
                    #except:
                    #    continue
                    # Make sure laser is before sonar, so if you have
                    # sonar, it will be the default 'range' device
                    if device == "laser":
                        self.range = self.laser[0]
                    elif device == "ir":
                        self.range = self.ir[0]
                    elif device == "sonar":
                        self.range = self.sonar[0]
                    elif device == "position":
                        self.supportedFeatures.append( "odometry" )
                        self.supportedFeatures.append( "continuous-movement" )
        if "range" in self.__dict__:
            self.supportedFeatures.append( "range-sensor" )

        

        # try to add a simulation device from port 7000
        try:
            self.startDevice("simulation")
        except:
            self.builtinDevices.remove("simulation")
            del self.simulation
        # default values for all robots:
        self.stall = 0
        self.x = 0.0
        self.y = 0.0
        self.th = 0.0
        self.thr = 0.0
        # Can we get these from player?
        self.radius = 0.40
        self.type = "Player"
        self.subtype = 0
        self.units = "METERS"
        self.localize(0.0, 0.0, 0.0)

        self.update()

    def destroy(self):
        self.thread.join()
        try:
            self.simulation.join()
        except:
            pass

    # Used to open an interface to an additional player device.  eg,
    # if you want to open position:10 in addition to position:0.
    #
    # name and idx correspond to the player interface type and device
    # number.
    #
    # Override is whether the new device should override devices
    # already open (eg, if some other position is being used by
    # default, should this override it or leave it alone).
    def startPlayerDevice(self, name, idx, override=False):
        d = self.robot.startDeviceBuiltin(name, idx)
        self.robot.startDevices(d, override=override)        
        
    def startDeviceBuiltin(self, item, index=0):
        if item == "laser":
            return {"laser": PlayerLaserDevice(self._client)}
        elif item == "sonar":
            return {"sonar": PlayerSonarDevice(self._client)}
        elif item == "camera":
            from pyrobot.camera.player import PlayerCamera
            from pyrobot.vision.cvision import VisionSystem
            camera = PlayerCamera(self.hostname, self.port, visionSystem=VisionSystem())
            return {"camera": camera}
        elif item == "fiducial":
            return {"fiducial": PlayerFiducialDevice(self._client)}
        elif item == "gripper":
            return {"gripper": PlayerGripperDevice(self._client)}
        elif item == "position":
            return {"position": PlayerPositionDevice(self._client)}
        elif item == "speech":
            return {"speech": PlayerSpeechDevice(self._client)}
        elif item == "graphics2d":
            return {"graphics2d": PlayerGraphics2dDevice(self._client)}
        elif item == "map":
            return {"map": PlayerMapDevice(self._client)}
        elif item == "simulation":
            obj = None
            try:
                obj = PlayerSimulationDevice(self.hostname)
            except:
                pass
            return {"simulation": obj}
	elif item == "ptz":
	    return {"ptz": PlayerPTZDevice(self._client, "ptz")}
	elif item == "bumper":
	    return {"bumper": PlayerBumperDevice(self._client)}
##         elif item == "camera":
##             if self.simulated:
##                 return self.startDevice("BlobCamera", visible=0)
##             else:
##                 return self.startDevice("V4LCamera")
##        elif item in self.builtinDevices:
        return {item: PlayerDevice(self._client, item, index=index)}
    
    def translate(self, translate_velocity):
        tv = translate_velocity * self.scale
        self.last_translate = tv
        self.position[0]._dev.set_velocity( tv, 0, 0,
                                            0, 0, self.last_rotate, 1)
        time.sleep(.1)
    def rotate(self, rotate_velocity):
        rv = rotate_velocity * self.scale
        self.last_rotate = rv
        self.position[0]._dev.set_velocity(self.last_translate, 0, 0,
                                           0, 0, rv, 1)
        time.sleep(.1)
    def move(self, translate_velocity, rotate_velocity):
        rv, tv = rotate_velocity * self.scale, translate_velocity * self.scale
        self.last_rotate = rv
        self.last_translate = tv
        self.position[0]._dev.set_velocity(tv, 0, 0, 
                                          0, 0, rv, 1)
        time.sleep(.1)
    def update(self):
        if self.hasA("position"):
            self.x = 0 #self.position[0]._dev.px
            self.y = 0 #self.position[0]._dev.py
            self.thr = 0 #self.position[0]._dev.pa # radians
            self.th = 0 #self.thr / PIOVER180
            self.stall = 0 #self.position[0]._dev.stall
        self.updateDevices()
            
    def localize(self, x = 0, y = 0, th = 0):
        """
        Set robot's internal pose to x (meters), y (meters),
        th (radians)
        """
        if self.hasA("position"):
            #self._client.set_odometry(x * 1000, y * 1000, th)
            self.x = x
            self.y = y
            self.th = th
            self.thr = self.th * PIOVER180

    def connect(self):
        print "Pyrobot Player: hostname=", self.hostname, "port=", self.port
        # FIXME: arbitrary time! How can we know server is up and running?
        time.sleep(1)
        print "Trying to connect..."
        self._client = playerc.playerc_client( None, 'localhost', 6665 )
        # Connect it
        if self._client.connect() != 0:
            raise playerc.playerc_error_str()

        #self._client = playerc.playerc_client(None, self.hostname, self.port)
        #retval = self._client.connect() 
        #while retval == -1:
        #    print "Trying to connect..."
        #    self._client = playerc.playerc_client(None, self.hostname, self.port)
        #    time.sleep(1)
        #    retval = self._client.connect()

    def removeDevice(self, item, number = 0):
        self.__dict__[item][number]._dev.unsubscribe()
        Robot.removeDevice(self, item)
        
if __name__ == '__main__':
    myrobot = PlayerRobot()
    myrobot.update()
    myrobot.translate(.2)
    myrobot.translate(.0)
    myrobot.disconnect()
