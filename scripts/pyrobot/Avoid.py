# robot goes forward and then slows to a stop when it detects something  
   
from pyrobot.brain import Brain  
   
class Avoid(Brain):  
           
   # Give the front two sensors, decide the next move  
   def determineMove(self, front, left, right):  
      if front < 0.5:   
         #print "obstacle ahead, hard turn"  
         return(0, .3)  
      elif left < 0.8:
         #print "object detected on left, slow turn"
         return(0.1, -.3)  
      elif right < 0.8: 
         #print "object detected on right, slow turn" 
         return(0.1, .3)  
      else:  
         #print "clear"  
         return(0.5, 0.0) 
      
   def step(self):  
      front = min([s.distance() for s in self.robot.range["front"]])
      left = min([s.distance() for s in self.robot.range["left-front"]])
      right = min([s.distance() for s in self.robot.range["right-front"]])
      translation, rotate = self.determineMove(front, left, right)  
      self.robot.move(translation, rotate)

def INIT(engine):  
   assert (engine.robot.requires("range-sensor") and
           engine.robot.requires("continuous-movement"))
   return Avoid('Avoid', engine)  

