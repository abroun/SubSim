#! /usr/bin/python

import math
from playerc import *

# Create a client object
playerClient = playerc_client( None, 'localhost', 6665 )
# Connect it
if playerClient.connect() != 0:
  raise playerc_error_str()

# Create a proxy for position3d:0
pos3d = playerc_position3d( playerClient, 0 )
if pos3d.subscribe( PLAYERC_OPEN_MODE ) != 0:
  raise playerc_error_str()

print "Connected to Player!"

while True:
    pos3d.set_velocity( 0.1, 0.1, 1.0, # x, y, z
                        0.0, 0.0, 0.05, # roll, pitch, yaw
                        0 )   # State

# Clean up
pos3d.unsubscribe()
playerClient.disconnect()
