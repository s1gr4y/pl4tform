# pl4tform (beta/WIP)

A simple 3D platforming game made in C and OpenGL. It is expected that the gameplay will play similar to a basic platformer with nice movement.

### Preview

![thumbnail](README_imgs/prev_with_text.png)

### Current Features
- Movement in the ground/air and on moving objects
- Uses SAT algorithm and OBBs for collision detection
- Movement physics response such as resistance and changed velocity on slopes
- Fonts and text displays

### To Be Implemented
A TODO list, listed in order of importance and/or utility, top to bottom.
1. Resolve player velocity in the advent of moving objects (including rotation) -mostly resolved
2. Coarse Grid for smart pre-check collision detection rather than check all objects
3. Code Refactor with comments for clarity/efficiency (somewhat complete).

### Potential Features
A reach/extra additions list, listed in no particular order.
- Have Text/UI
- Ability to rebind keys and have main/pause menu
- Clear lose/win states
- Include lighting
- Have more/different models/textures
- Adjust README to have build/helper information

### Controls
- W: Foward
- A: Left
- S: Back
- D: Right
- SPACE: Jump
- ESC: Quit

# How to build
### This currently only builds on Windows 10/11. Inclusion of Linux && MacOS builds are in the works.
If you are on a Linux/MacOS, you need to build GLFW and have the libraries in the same folder of the binary/executable (bin folder).\
Additionally, you need to build the [FreeType](https://freetype.org/) libraries and place them in the Dependencies folder.

## Windows 10/11 
It can be built with cmake. You can install CMake here: [Link](https://cmake.org/install/)\
The minimum CMake version is 3.0 but lower versions could work if you change the requirements in CMakeLists.txt.\
Windows specific notes:\
If you want to use mingw compiler rather than .NET/Microsoft (or default) compiler, do the following:\
```cmake -G "MinGW Makefiles" .```\
```make```\
General steps:\
In your preferred terminal, in the platform/ directory
1. Enter "cmake ."
2. Enter "cmake --build ."

And there you go! The output is built in the pl4tform/bin directory, including the source images, glfw dll, and the program.

# File Directory
```bash
|-bin/
	|-//out files, dll files, exe
|-Dependencies/
	|-cglm/
	|-freetype/
	|-glad/
	|-GLFW/
	|-freetype.a	//lib to link
	|-ft2build.h
	|-stb_image.h
|-README_imgs/
|-Resources/
	|-Game textures
	|-Fonts
|-src/
	|-engine/
		|-//engine files like obb collision detection, physics, and world
	|-entity/
		|-//game objects and player
	|-gfx/
		|-//shaders, window, glad files (visuals)
	|-include/
		|-//all .h files
	|-utils/
		|-//fonts, extra files as helpers.
	|-pl4tform.c
|-README.md
```

# Other Notes

For the freetype library, we only need the lib ".a" file when linking with the exe, no dll need.\
We use freetype version 2.12. We use OpenGL version 3.3 but higher versions could work.\
We use GLFW3 version 3.3.8 but lower/other versions could work.

For getting the dependencies, you need to get GLFW and FreeType libs by building them from their binaries.\
On linux:\
After building,\
GFLW: need libglfw3dll.so.3.3 and libglfw3.a placed in the pl4tform/Dependencies/GLFW/lib-vc2022 folder.\
FreeType: need libfreetype.so placed in the pl4tform/Dependencies folder.

Currently there's a velocity inheritance bug, I did not appropriately configure the correct rules or cases accordingly.\
It is resolveable but busy with other aspects, you gain extreme speeds if hugging an moving object in the air.

# Dependencies
- [OpenGL](https://www.opengl.org/): Graphics specification
- [GLFW](https://github.com/glfw/glfw): Window context
- [glad](https://github.com/Dav1dde/glad): OpenGL function pointer loader
- [cglm](https://github.com/recp/cglm): Helper math library for vectors/quaternions
- [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h): Single header file for image texture generation
- [FreeType](https://freetype.org/): Library to generate glyphs/data from ttf files for processing like textures

