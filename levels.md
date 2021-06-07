Levels are defined in .lvl files that include the following
- The level's 3D model (an OBJ file in the special SF3 format)
- The level's 2D background (a PNG file)
- The map's script (in compiled bytecode form), along with any other scripts it references - the maps script will define both the MP3 file for the level's music, and any script trigger points (if player.Z == 100 then do event) in the main loop

The scripts use a C-style scripting language, interpreted and executed in a stack-based virtual machine at runtime. All entities have scripts associated with them, including the map/camera (entity -1)

All scripts will have an init() function, a loop() function, and event handlers. For example, the map/camera's init() func will spawn the Arwing. The Arwing's init func will set its HP, etc. The map's loop function can check the X, Y, and Z coordinates of the Arwing to see if it falls into a certain range to trigger map events, such as enemies spawning, or your teammates talking.

Alternatively, the map may recieve a message from the game engine informing it of a keypress. The map will then pass this along to the Arwing if it falls within the controls for that starfighter, which will then spawn a laser right in front of the ship (potentially two if its state variable for twin lasers are set) and play a sound effect. The laser will keep moving forward until it hits something, when it will destroy itself, and send the laser impact message to whatever it hits. That object will then respond to the message by decrementing the appropriate HP, and playing the appropriate animation and destroying itself if necessary. 

Exported Functions
------------------
log(string message) -> Prints string to the console

objThisID() -> Returns entity ID of current object (returns -1 if camera)

objColliding() -> Returns entity ID of colliding object (returns -1 if no object)

objDestroy(float entityID) -> Destroys object of given ID

objRotate(float entityID, float xRot, float yRot, float zRot) -> Rotates object on given axes 

objCurrentX(float entityID) -> Returns current pitch of object

objCurrentY(float entityID) -> Returns current yaw of object

objCurrentZ(float entityID) -> Returns current roll of object

objMove( ) -> 

tick() -> Does nothing

sayMsg(float speaker, float messageID) -> Using the image from the given speaker, display the text and play the sound associated with the given ID

spawnObj(string model, float scaleX, float scaleY, float scaleZ, float xPos, float yPos, float zPos, float scriptID) -> 

sendMessage(float entityID, float messageType) -> Sends a message to another entity

setVar(float entityID, float varIndex, floatValue) -> Alters value of local entity state variable

getVar(float entityID, float varIndex) -> Fetches value of local entity state variable

Event Handlers
--------------
onCollide(float entityID)

onMessage(float message, float sender)
