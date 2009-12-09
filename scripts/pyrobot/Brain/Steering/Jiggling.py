#-------------------------------------------------------------------------------
import random
from Steering import SteeringBehaviour

#-------------------------------------------------------------------------------
NUM_MOVES = 5           # The number of moves that make up a jiggle  
NUM_MOVE_TICKS = 10    # The time in update ticks that we jiggle for

#-------------------------------------------------------------------------------
class Jiggling( SteeringBehaviour ):
    """A steering behaviour that attempts to recover from stalls by jiggling
       around..."""

    #---------------------------------------------------------------------------
    def __init__( self, robot ):
        SteeringBehaviour.__init__( self, robot )
        self.numMovesRemaining = 0
        self.numMoveTicksRemaining = 0
        self.moveSteeringResult = ( 0.0, 0.0 )

    #---------------------------------------------------------------------------
    def startJiggling( self ):
        self.numMovesRemaining = NUM_MOVES
        self.goBackwards = True
        self.pickNewMove()

    #---------------------------------------------------------------------------
    def isJigglingComplete( self ):
        return self.numMovesRemaining <= 0 and self.numMoveTicksRemaining <= 0

    #---------------------------------------------------------------------------
    def update( self ):
    
        moveAvailable = False

        # Work out if we have something to do
        if self.numMoveTicksRemaining > 0:
            moveAvailable = True
        else:
            if self.numMovesRemaining > 0:
                self.pickNewMove()
                self.numMovesRemaining = self.numMovesRemaining - 1
                moveAvailable = True
            
        steeringResult = ( 0.0, 0.0 )
        if moveAvailable:
            self.numMoveTicksRemaining = self.numMoveTicksRemaining - 1
            steeringResult = self.moveSteeringResult

        return steeringResult
    
    #---------------------------------------------------------------------------
    def pickNewMove( self ):
        """Pick a new speed and angular velocity at random"""

        MIN_FORWARD_SPEED = 0.2
        MAX_FORWARD_SPEED = 0.5

        MIN_TURNING_SPEED = 0.1
        MAX_TURNING_SPEED = 0.3

        forwardSpeed = random.uniform( MIN_FORWARD_SPEED, MAX_FORWARD_SPEED )
        if self.goBackwards:
            forwardSpeed = -forwardSpeed

        self.goBackwards = not self.goBackwards # Means that we jiggle back and forth

        turningSpeed = random.uniform( MIN_TURNING_SPEED, MAX_TURNING_SPEED )
        if random.random() < 0.5:
            turningSpeed = -turningSpeed

        self.numMoveTicksRemaining = NUM_MOVE_TICKS
        self.moveSteeringResult = ( forwardSpeed, turningSpeed )

        