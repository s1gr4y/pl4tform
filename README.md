# pl4tform (beta/WIP)

A simple 3D platforming game made in C and OpenGL. It is expected that the gameplay will play similar to a basic platformer with nice movement.

### Current Features
- Basic movement in the ground/air
- Uses SAT algorithm and OBBs for collision detection
- Movement physics response such as resistance and changed velocity on slopes


### To Be Implemented
A TODO list, listed in order of importance and/or utility, top to bottom.
1. Resolve player velocity in the advent of moving objects (including rotation)
2. Coarse Grid for smart pre-check collision detection rather than check all objects
3. Code Refactor with comments for clarity/efficiency.

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

# File Directory
```bash
|-Debug/
	|-//out files / exe
|-Dependencies/
	|-cglm/
	|-glad/
	|-GLFW/
	|-stb_image.h
|-Resources/
	|-Game textures
|-src/
	|-engine/
		|-//engine files like obb collision detection, physics, and world
	|-entity/
		|-//game objects and player
	|-gfx/
		|-//shader, window, glad files (visuals)
	|-utils/
		|-//extra files as helpers.
	|-pl4tform.c
|-README.md
```

# Dependencies
- [OpenGL](https://www.opengl.org/): Graphics specification
- [GLFW](https://github.com/glfw/glfw): Window context
- [glad](https://github.com/Dav1dde/glad): OpenGL function pointer loader
- [cglm](https://github.com/recp/cglm): Helper math library for vectors/quaternions
- [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h): Single header file for image texture generation

