#-------------------------------------------------------------------------------
import math
import random

from Maths.Vector2D import Vector2D
from Steering import SteeringBehaviour

DISTANCE_TO_WANDER_CIRCLE = math.sqrt( 2.0 )
WANNDER_CIRCLE_RADIUS = 1.5

#-------------------------------------------------------------------------------
class Wander( SteeringBehaviour ):
    """Takes the robot on a random walk"""

    #---------------------------------------------------------------------------
    def __init__( self, robot, wanderRate = 0.7 ):
        SteeringBehaviour.__init__( self, robot )
        self.setWanderRate( wanderRate )
        self.wanderTargetAngle = 0.0

    #---------------------------------------------------------------------------
    def setWanderRate( self, wanderRate ):
        """Sets the maximum distance off the wander circle that the target
           pos tries to move each update"""
        self.wanderRate = wanderRate

        if self.wanderRate < 0.0:
            self.wanderRate = 0.0
        if self.wanderRate > 1.0:
            self.wanderRate = 1.0

    #---------------------------------------------------------------------------
    def update( self ):
    
        # This routine imagines that the robot is facing along the y-axis
        # with the x-axis going right. Angles are measured anti-clockwise
        # from the y-axis
        wanderCircleCentrePos = Vector2D( 0, DISTANCE_TO_WANDER_CIRCLE )
        oldWanderTargetPos = wanderCircleCentrePos \
            + Vector2D( math.sin( self.wanderTargetAngle ), math.cos( self.wanderTargetAngle ) )*WANNDER_CIRCLE_RADIUS
            
        # Move the wander target in a random direction
        offsetAngle = random.random()*2.0*math.pi
        offsetDistance = random.random()*self.wanderRate
        wanderTargetPos = oldWanderTargetPos \
            + Vector2D( math.sin( offsetAngle ), math.cos( offsetAngle ) )*offsetDistance
        
        # Store the wander target as an angle
        vectorFromCentre = wanderTargetPos - wanderCircleCentrePos
        vectorLength = vectorFromCentre.Length()
        if vectorLength != 0.0:
            self.wanderTargetAngle = math.atan2( vectorFromCentre.x, vectorFromCentre.y )
        else:
            # Hit the centre so just leave it where it was
            pass
            
        # Translate the wander target angle into a steering result
        
        MAX_FORWARD_SPEED = 1.2
        MAX_TURNING_SPEED = 0.8
        
        normalisedForwardComponent = (1.0 + math.cos( self.wanderTargetAngle )) / 2.0
        forwardSpeed = normalisedForwardComponent*MAX_FORWARD_SPEED
        
        turningSpeed = math.sin( self.wanderTargetAngle )*MAX_TURNING_SPEED
           
        #print str( math.degrees( self.wanderTargetAngle ) ) + " " + str( ( forwardSpeed, turningSpeed ) )
           
        return ( forwardSpeed, turningSpeed )
        
