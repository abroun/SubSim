# robot goes forward and then slows to a stop when it detects something  
   
from pyrobot.brain import Brain  
from Steering.Wander import Wander
   
#-------------------------------------------------------------------------------
class SubBrain( Brain ):  

    #---------------------------------------------------------------------------
    def __init__( self, name = 'brain', engine = 0, **kwargs ):
        Brain.__init__( self, name, engine, **kwargs )

        self.robot.lastMove = (0.0, 0.0)
        self.robot.brain = self
        self.wanderBehaviour = Wander( self.robot )

    #---------------------------------------------------------------------------
    def step(self):  
     
        # Nothing to do but wander...
        translation, rotate = self.wanderBehaviour.update()
        self.robot.move( translation, rotate )

#-------------------------------------------------------------------------------
def INIT( engine ):  
    assert engine.robot.requires("continuous-movement")
    return SubBrain( 'SubBrain', engine )  

