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
### Dependencies

* Vulkan >= 1.0.3 : [Download LunarG's SDK](https://vulkan.lunarg.com) or install vulkan packages for your Linux ditribution.
* GLFW >= 3.2.1 : install GLFW dev and regular packages for your Linux distribution.

### Generate workspace

I use premake 5 to generate build files used by tools like Microsoft Visual Studio or GNU Make.
--> [Premake Wiki](https://github.com/premake/premake-core/wiki/Using-Premake)

Simply run `premake5 [action]` in the root directory (premake5.exe for Windows, and premake5 for Linux).
It will create a "build" directory where you will find a directory named after selected action (like vs2017 for Visual Studio 2017).

## Shipped Libraries

* GLFW 3.2.1 (only Windows binaries are shipped)
* GLM 0.9.8.5
* STB_image
* Tiny_Obj_Loader