
1.  Usage:  Fbx2X.exe  <options> <input filename>

	   <input filename>    Input fbx filename. 
		     (Default: output seperated and compressed model and animation.)
	   /sx                 Seperate animation from the mesh before exporting
	   /d                  Delete meshes without skin info before exporting
	   /o <file>           Optional output model filename
	   /a <file>           Optional output animation filename
	   /m <file>           Optional output material filename
	   /y                  Force the Y axis to up direction.( Default: Z is up!)
	   /r                  Force to right-hand coordinate( Default: left-hand coord!)
	   /s <scale>          The equivalent number of centimeters.( Default: scale is 1m!)
	   /w                  Force swap the winding order
	   /nop                Not optimize the mesh before exporting
	   /nt                 Not copy textures to destination
	   /xc                 Convert to .x compressed binary file( Default)
	   /x                  Convert to .x binary file
	   /xt                 Convert to .x text file
	   /?                  To see this help information.

	For example,
	1) 
		D:\model_samples>Fbx2X box.fbx

		Note: the above command running on cmd of windows will output a box.x file, which can be viewed in DirectX Viewer.

	2)	D:\model_samples>Fbx2X box.fbx /o boxx.x

		Note: force the output filename to be boxx.x.


	3)     To convert man.fbx to .x file��run the following command

		  D:\model_samples>Fbx2X man.fbx

		If successful��you will get��

		   Converting inputfile:D:\model_samples\man.fbx ...
		   Note: written log file:D:\model_samples\man.log.

		man.log explains which files have been outputted. E.g.��

		Model( with animation):man.x
		Format:Compressed
		Up Direction:Z
		All used 1 textures:
		man01.jpg

		The outputed file, man.x, can be viewed by DirectX viewer.


	4��     To convert man.fbx to .x file��seperating animation from the model

		   D:\model_samples>Fbx2X man.fbx /sx

		If successful��you will get��

		   Converting inputfile:D:\model_samples\man.fbx ...
		   Note: written log file:D:\model_samples\man.log.

		Note: man.log explains which files have been outputted. E.g.��

		Model( without animation):man.x
		Animation:man_anim.x
		Format:Compressed
		Up Direction:Z
		All used 1 textures:
		man01.jpg

		The former is animation file and the latter is the model file.


2. Compilation steps,

	1) Install directX SDK(version 2007 or 2008) 
	2) Install fbx SDK(version 2009.1) 
	3) Set c++ directory: include and lib for above two sdk 
	4) Use VS2005 to compile the solution.


Longjiang li
Email:Longjiang.lee@gmail.com




