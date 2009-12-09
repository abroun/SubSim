#-------------------------------------------------------------------------------
from Steering import SteeringBehaviour

#-------------------------------------------------------------------------------
class AvoidWall( SteeringBehaviour ):
    """A steering behaviour that attempts to avoid walls by steering away from
       them"""

    #---------------------------------------------------------------------------
    def __init__( self, robot ):
        SteeringBehaviour.__init__( self, robot )

    #---------------------------------------------------------------------------
    def update( self ):
       
        FORWARD_SPEED = 0.1
        TURNING_SPEED = 0.3
        
        minFrontSensor = min( [s.distance() for s in self.robot.range["front"]] )
        minLeftSensor = min( [s.distance() for s in (self.robot.range["front-left"] + self.robot.range["left-front"]) ] )
        minRightSensor = min( [s.distance() for s in (self.robot.range["front-right"] + self.robot.range["right-front"]) ] )
        
        # The sensors are less worried about obstacles to the side to let us drive 
        # through gaps
        frontObstacle = minFrontSensor < 0.5
        leftObstacle = minLeftSensor < 0.25
        rightObstacle = minRightSensor < 0.25
        obstacleIsReallyClose = ( minFrontSensor < 0.1 or minLeftSensor < 0.1 or minRightSensor < 0.1 )
        
        steeringResult = None   # By default we do nothing
        
        if leftObstacle:
            if not rightObstacle:
                steeringResult = ( FORWARD_SPEED, -TURNING_SPEED )  # Turn right
            else:
                steeringResult = ( FORWARD_SPEED, TURNING_SPEED )   # Turn left
        else:
            if frontObstacle or rightObstacle:
                steeringResult = ( FORWARD_SPEED, TURNING_SPEED )   # Turn left
                
        if obstacleIsReallyClose and steeringResult != None:
            steeringResult = ( -0.5, 0.0 )
                
        return steeringResult
