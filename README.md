# Learning Vulkan through vulkan-tutorial.com and Intel Vulkan tutorial 

I'm currently learning the API by reeding tutorials and experimenting things.
My goal is to have a general comprehension of the API, and to learn commons graphics technics.

## TODO LIST

* Library loading
	* load extension functions only if activated at Instance/Device creation time

* Renderer
    * Rendering technics
	    * SSR
		* SSAO
		* PBR
		* SSSSS
		* SMAA
* Cameras
    * Orbiter
	* Free fly camera
	
## How to build

I use premake 5 to generate build files used by tools like Microsoft Visual Studio or GNU Make.
--> [Premake Wiki](https://github.com/premake/premake-core/wiki)

Simply run premake5 in the root directory (premake5.exe for Windows, ans premake5 for Linux)
It will create a "build" directory where you will find a directory named after your tool (like vs2017 for Visual Studio 2017).

## Used Libraries

* Vulkan 1.0.65.1
* GLFW 3.2.1
* GLM 0.9.8.5
* STB_image
* Tiny_Obj_Loader