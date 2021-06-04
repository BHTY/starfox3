Levels are defined in .lvl files that include the following
- The level's 3D model (an OBJ file in the special SF3 format)
- The level's 2D background (a PNG file)
- The level's music (an MP3 file) - specified in script
- Any script trigger points (including one-dimensional triggers) and scripts -> specified in script loop

The scripts use a C-style scripting language, interpreted and executed in a stack-based virtual machine at runtime. All entities have scripts associated with them, including the map/camera (entity -1)

All scripts will have an init() function, a loop() function, and event handlers
For example, the map/camera's init() func will spawn the Arwing
The Arwing's init func will set its HP, etc.

Exported Functions
------------------
log(string message) -> Prints string to the console

objThisID() -> Returns entity ID of current object (returns -1 if camera)

objColliding() -> Returns entity ID of colliding object (returns -1 if no object)

objDestroy(float entityID) -> Destroys object of given ID

objRotate(float entityID, float xRot, float yRot, float zRot) -> Rotates object on given axes 

objRotRelative(float entityID, float xRot, float yRot, float zRot) -> Rotates object on given axes

objMove( ) -> 

tick() -> Does nothing

sayMsg(float speaker, float messageID) -> Using the image from the given speaker, display the text and play the sound associated with the given ID

spawnObj(string model, float scaleX, float scaleY, float scaleZ, float xPos, float yPos, float zPos, string script) -> 

sendMessage(float entityID, float messageType) -> Sends a message to another entity

setVar(float entityID, float varIndex, floatValue) -> Alters value of local entity state variable

getVar(float entityID, float varIndex) -> Fetches value of local entity state variable

Event Handlers
--------------
onCollide(float entityID)

onMessage(float message, float sender)
