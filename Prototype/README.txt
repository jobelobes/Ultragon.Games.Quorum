Flash Prototype of Quorum

XBOX360 Gamepad Server.exe
This is a socket server that exposes XBOX 360 Controllers (USB/Wireless) to Flash.
It should be launched and run before starting the swf or compiling the fla.
Original version downloaded from: http://projects.psibaspace.net.au/code.php
Cleaned and Vista compatible version thanks to Mike Ey (he's my hero!)

ControllerDemo.fla
This is a demo for testing the Server.exe
It illustrates all buttons and sticks interactively.

ControllerDemo.exe
This is the same as the .fla - but doesn't need the Flash IDE to run.
Run the Server.exe, then run ControllerDemo.exe
If an XBox Controller is present on your system, press buttons and push sticks. :-D

Quorum.fla
This will be our rapid prototype, now that I've roughed out the inputs for Flash.
I'm hoping this will save us time in the long run:
- testing control schemes
- testing ship layouts

Quorum.exe
This prototype works without needing the Flash IDE.
Player controls:
Anytime:
- Triggers control the zoom level
Standing:
- Left Stick navigates the player on the bridge.
- A seats the player if he's close enough to a station.
Seated:
- Y makes the player stand up
- Bumpers send the player clockwise/counter-clockwise


Package structure/overview:
quorum
- ControllerDemo.as - the MAIN file for the ControllerDemo.fla
- QuorumDocument.as - the MAIN file for our future prototype
- Player.as - instantiated by QuorumDocument, handles Player movement/interaction
+- xbox
  - XBox360Manager.as - communication with the Socket server
  - XBox360Controller.as - an instance of a controller
  - XBox360ControllerGraphic.as - a graphical representation tied to a controller instance
  +- events
    - XBox360ConnectEvent.as - dispatched when a controller connects/disconnects
	- XBox360ButtonEvent.as - dispatched when a controller button is pressed
	- XBox360AnalogEvent.as - dispatched when a trigger or stick is moved
+- ship
  - Ship.as - Ship object holds Player instances and other ship-based things
  - Station.as - Station object holds all station-related things
	
That's it for now. I'll be adding things as the come up!