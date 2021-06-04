Levels are defined in .lvl files that include the following
- The level's 3D model (an OBJ file in the special SF3 format)
- The level's 2D background (a PNG file)
- The level's music (an MP3 file) - specified in script
- Any script trigger points (including one-dimensional triggers) and scripts -> specified in script loop

The scripts use a simple, custom language, interpreted and executed in a minimal virtual machine at runtime, defined with the following commands.

**CAMROTATE**: Takes pitch/yaw rotation as arguments (example: _CAMROTATE 90 15_ would rotate 90 degrees horizontally and ten degrees vertically)

**CAMMOVE**: 

**SPAWNOBJ**: Spawns enemy ships

**SAYMSG**: Takes two arguements - the speaker ID (used for the image) and message ID (used for the message text and sound clip if appicable) (example: _SAYMSG 001 365_ would have speaker 1 say message 365)

Additionally, each entity can have its own script, which is "ticked" each frame, with the following commands.

**TICK**: Does nothing. Used for empty scripts.

**OBJROTATE**

**OBJMOVE**

**OBJCOLLIDE**: Returns the ID of any object the current object is colliding with. Returns -1 if no object.

**OBJDESTROY**: Takes one arguement - the object ID to destroy. -1 is no object, -2 is "this" object.

Objects can also check for collision with other objects 
