SubSim
======

SubSim is a simulator for the UWE MSc Robotics amd ATE group project which involves building an Autonomous Underwater Vehicle (AUV) to take part in the Student Autonomous Underwater Challenge - Europe (SAUC-E) 2010.

It is built using the Irrlicht engine for graphics and the Open Dynamics Engine (ODE) for physics simulation. The simulation is controlled using the Player network server so that control code written for the submarine can hopefully be used with minimal modification for the real worl submarine.

Exporting Models from Blender
=============================

- Blender uses a right handed co-ordinate system. Irrlicht uses a left handed coordinate system but we want the simulator to have a right handed co-ordinate system which agrees with the co-ordinate system used by the rest of the team... It gets a bit complicated but hopefully a user of the simulator shouldn't need to know about it.

When exporting models from Blender however you need to follow these steps

- Select all of the items you want to export
- Choose File -> Export -> DirectX (.x)
- Make sure that 'Swap zy', 'Flip z' and 'BI.normals' are selected.
- Make sure that nothing else is selected
- Hit 'Export Sel' and then choose the file to export to. X files should be stored in the media/export directory and source files (the .blend files) should be stored in media/src
- After that press 'Exit' to quit out of the exporter