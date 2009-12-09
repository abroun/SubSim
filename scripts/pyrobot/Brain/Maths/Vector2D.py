#-------------------------------------------------------------------------------
import math

#-------------------------------------------------------------------------------
class Vector2D:
    """A 2D vector class nicked from the web"""

    #---------------------------------------------------------------------------
    def __init__( self, x = None, y = None ):
        if x == None:   # Assume that we're being called with no arguments
            self.x = 0.0
            self.y = 0.0
        elif y == None:   # Assume that we have been passed an array
            v = x
            self.x = float( v[ 0 ] )
            self.y = float( v[ 1 ] )
        else:
            self.x = float( x )
            self.y = float( y )
        
    #---------------------------------------------------------------------------
    def __add__( self, v ):
        return Vector2D( self[ 0 ] + v[ 0 ], self[ 1 ] + v[ 1 ] )
    
    #---------------------------------------------------------------------------
    def __sub__( self, v ):
        return Vector2D( self[ 0 ] - v[ 0 ], self[ 1 ] - v[ 1 ] )
    
    #---------------------------------------------------------------------------
    def __iadd__( self, v ):
        self.x = v[ 0 ] + self.x
        self.y = v[ 1 ] + self.y
        return self
        
    #---------------------------------------------------------------------------
    def __isub__( self, v ):
        self.x = self.x - v[ 0 ]
        self.y = self.y - v[ 1 ]
        return self
    
    #---------------------------------------------------------------------------
    def __div__( self, v ):
        return Vector2D( self[ 0 ] / v, self[ 1 ] / v )
    
    #---------------------------------------------------------------------------
    def __mul__( self, v ):
        return Vector2D( self[ 0 ] * v, self[ 1 ] * v )
    
    #---------------------------------------------------------------------------
    def __idiv__( self, v ):
        self.x = self.x / v
        self.y = self.y / v
        return self
        
    #---------------------------------------------------------------------------
    def __imul__( self, val ):
        self.x = self.x * v
        self.y = self.y * v
        return self
        
    #---------------------------------------------------------------------------            
    def __getitem__( self, key ):
        if( key == 0 ):
            return self.x
        elif( key == 1 ):
            return self.y
        else:
            raise Exception( "Invalid key to Vector2D" )
        
    #---------------------------------------------------------------------------
    def __setitem__( self, key, value ):
        if( key == 0 ):
            self.x = value
        elif( key == 1 ):
            self.y = value
        else:
            raise Exception( "Invalid key to Vector2D" )
        
    #---------------------------------------------------------------------------
    def __str__( self ):
        return "(" + str( self.x ) + "," + str( self.y ) + ")"
    
    #---------------------------------------------------------------------------
    def LengthSquared( self ):
        """Returns the squared length of the vector. Marginally faster than Length()"""
        return self.x**2 + self.y**2
    
    #---------------------------------------------------------------------------
    def Length( self ):
        """Returns the length of the vector"""
        return math.sqrt( self.LengthSquared() )
       
    #---------------------------------------------------------------------------
    def Dot( self, v ):
        """Computes the dot product of this Vector2D and v"""
        return self.x*v[ 0 ] + self.y*v[ 1 ]
        
#-------------------------------------------------------------------------------
def Normalise( v ):
    """Returns a new Vector2D that has the same direction as v, but has a length of one."""
    vector2D = Vector2D( v )
    vector2DLength = vector2D.Length()
    
    if vector2DLength > 0:
        vector2D /= vector2DLength
        
    return vector2D

