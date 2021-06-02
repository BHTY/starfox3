Levels are defined in .lvl files that include the following
- The level's 3D model (an OBJ file in the special SF3 format)
- The level's 2D background (a PNG file)
- The level's music (an MP3 file)
- Any script trigger points (including one-dimensional triggers) and scripts

The scripts use a simple, custom language, interpreted and executed in a minimal virtual machine at runtime, defined with the following commands.

**CAMROTATE**: Takes pitch/yaw rotation as arguments (example: _CAMROTATE 90 15_ would rotate 90 degrees horizontally and ten degrees vertically)

**CAMMOVE**: 

**SPAWNOBJ**: Spawns enemy ships

**SAYMSG**: Takes two arguements - the speaker ID (used for the image) and message ID (used for the message text and sound clip if appicable) (example: _SAYMSG 001 365_ would have speaker 1 say message 365)
