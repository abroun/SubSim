from player import PlayerRobot  # Uses hijacked PlayerRobot for now...

# This should work for real and simulated Player-based robots

def INIT():
    robot = PlayerRobot("Player6665", 6665) #, hostname="bubbles")
    robot.scale = 2.0
    return robot



