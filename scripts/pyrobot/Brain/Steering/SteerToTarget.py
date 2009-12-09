#-------------------------------------------------------------------------------
import math
from Maths.Vector2D import Vector2D
from Steering import SteeringBehaviour

#-------------------------------------------------------------------------------
class SteerToTarget( SteeringBehaviour ):
    """A steering behaviour that tries to steer towards a target. It will also
       try to 'arrive' at the target by slowing down as it gets closer"""

    #---------------------------------------------------------------------------
    def __init__( self, robot, locator ):
        SteeringBehaviour.__init__( self, robot )
        self.locator = locator
        self.targetPos = Vector2D( 0, 0 )
    
    #---------------------------------------------------------------------------
    def setTargetPos( self, targetPos ):
        self.targetPos = targetPos

    #---------------------------------------------------------------------------
    def update( self ):
    
        ARRIVAL_DISTANCE = 1.0
        MIN_TURN_DISTANCE = 0.1 # Don't try turning to face the target if we're really close to it
        
        BASE_FORWARD_SPEED = 0.05
        EXTRA_FORWARD_SPEED = 1.0  # We get this if we're facing towards the target
        EXTRA_FORWARD_SPEED_HEADING_DIFF = math.pi / 4.0
        
        HEADING_ARRIVAL_ANGLE = math.pi / 8.0   # Used to reduce the turning speed as we face the target
        BASE_TURNING_SPEED = math.pi / 4.0
        
        forwardSpeed = BASE_FORWARD_SPEED
        turningSpeed = 0.0
        
        # Work out where we are in relation to the target
        curPos = self.locator.GetPosition()
        vectorToTarget = self.targetPos - curPos
        distanceToTarget = vectorToTarget.Length()
        
        # Work out our heading related to the target
        angleToTarget = math.atan2( -vectorToTarget.x, vectorToTarget.y )
        headingDiff = angleToTarget - self.locator.GetHeading()
        
        # Normalise to the range [-PI,PI)
        while headingDiff < -math.pi:
            headingDiff += 2.0*math.pi
        while headingDiff >= math.pi:
            headingDiff -= 2.0*math.pi
        absHeadingDiff = math.fabs( headingDiff )
        
        # Turn the robot to face the target
        if distanceToTarget > MIN_TURN_DISTANCE:
        
            if headingDiff >= 0.0:
                turningSpeed = BASE_TURNING_SPEED
            else:
                turningSpeed = -BASE_TURNING_SPEED
        
            if absHeadingDiff < HEADING_ARRIVAL_ANGLE:
                turningSpeed *= (absHeadingDiff / HEADING_ARRIVAL_ANGLE)
        
        # Let the robot go faster if it's heading towards the target
        if absHeadingDiff < EXTRA_FORWARD_SPEED_HEADING_DIFF:
            forwardSpeed += (1.0 - absHeadingDiff/EXTRA_FORWARD_SPEED_HEADING_DIFF)*EXTRA_FORWARD_SPEED
        
        # Slow the robot down as it approaches the target
        if distanceToTarget < ARRIVAL_DISTANCE:
            forwardSpeed *= (distanceToTarget / ARRIVAL_DISTANCE)
            
        return ( forwardSpeed, turningSpeed )
                
    
    