# IMT2531 Assignment2 - 3D Pacman

A 3D Pacman game made in C++ and OpenGL

## Marking Consideration

### Weighting
While I'm fairly happy with most aspects of the project I choose to weight slightly towards Architecture/Framework

### Architecture/Framework
* I choose to create a generic OpenGL game engine, and subsequently an implementation of a Pacman game using the engine.  
* It uses a component based system where each 'game object' is only responsible for managing the components attached to it.  
Some component like the Transform and MeshRenderer receives special treatment, but the user is free to create any sort of component they want.

### Gameplay/Usability
* Both Pacman and the ghosts are locked to the grid, and can never move more than 1 tile each frame.   
  This migrates any chance of collision errors or otherwise weird moment due to lag spikes.
* The ghosts are all different from each other and follows their behaviour from the original game as described [here](http://gameinternals.com/post/2072558330/understanding-pac-man-ghost-behavior).
* There are 3 different camera modes: Top-Down, Chase and Free.  
 In Top-Down and Free mode the input (WASD) is in relation to the world; D wil always make Pacman go towards the right side of the map.   
 In Chase mode the input is in relation to Pacman.
* The game can load a map of any size using the [provided format](http://prod3.imt.hig.no/Voldakk/IMT2531-Assignment2/blob/master/assets/levels/format.md).

### Aesthetics/Quality of Graphics
The engine uses the Phong lighting model. It supports: 
* Diffuse, specular, emission and normal maps.
* Multiple directional and point lights.
* Multiple directional shadow maps and a single cube map.
* A skybox.
* Instanced rendering.
* FreeType text.

## Setup

### Windows - Visual Studio 2017
1. File -> Open -> Folder - Select the root folder
2. Select "main" as the startup item

### Windows - CMake (cmake-gui)
1. Set the source code file to the root directory containing "CMakeLists.txt"
2. Set the build path to "\build\"
3. Click the configure button and select which generator you would like to use
4. Click the generate button
5. If your generator is an IDE such as Visual Studio, then open up the newly created EVA.sln file and set "main" as you StartUp Project.

### Ubuntu - CLion 2017
1. File -> Open - Select the root folder
2. Select the "main" configuration

### Ubuntu - Cmake terminal
Open a terminal window in the root folder of the project

##### 1. Create a build directory
```
mkdir build
cd build
```
##### 2. Generate UNIX Makefile (point CMake to CMakeLists.txt)
```
cmake ..
```
##### 3. Execute make command
```
make
```
##### 4. Run executable
```
cd bin
./main
```
## Built With

* [GLEW](http://glew.sourceforge.net/)
* [GLFW](http://www.glfw.org/)
* [GLM](https://glm.g-truc.net/0.9.8/index.html)
* [FreeType](https://www.freetype.org/)
* [ASSIMP](http://assimp.org/)
* [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)

## Authors

* **Eivind Vold Aunebakk** - 471185 - [Voldakk](https://github.com/Voldakk)

## Acknowledgments

* [TheChernoProject - Abstracting OpenGL into Classes](https://www.youtube.com/watch?v=bTHqmzjm2UI)
* [TheChernoProject - Buffer Layout Abstraction in OpenGL](https://www.youtube.com/watch?v=oD1dvfbyf6A)
* [TheChernoProject - Shader Abstraction in OpenGL](https://www.youtube.com/watch?v=gDtHL6hy9R8)
* [LearnOpenGL - Basic Lighting](https://learnopengl.com/Lighting/Basic-Lighting)
* [LearnOpenGL - Multiple lights](https://learnopengl.com/Lighting/Multiple-lights)
* [LearnOpenGL - Gamma Correction](https://learnopengl.com/Advanced-Lighting/Gamma-Correction)
* [LearnOpenGL - Shadow Mapping](https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping)
* [LearnOpenGL - Point Shadows](https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows)
* [LearnOpenGL - Normal Mapping](https://learnopengl.com/Advanced-Lighting/Normal-Mapping)
* [LearnOpenGL - Instancing](https://learnopengl.com/Advanced-OpenGL/Instancing)
* [LearnOpenGL - Text Rendering](https://learnopengl.com/In-Practice/Text-Rendering)