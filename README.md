# IMT2531 – Take-Home Exam 2018

## Task 1 - Terrain & Enviroment

### Terrain
* Textures
* Trees
* Water
* Arbitrary regions, they can also overlap

### Enviroment
* Shadows
* Days are longer in the summer
* Rising water level in the summer, the water turns to ice in the winter
* The skybox is tinted the same color as the sun. Orange sunrise and sunset

#### Day/night & season cycle and region settings
![Image of the settings in the scene editor](https://i.imgur.com/pKbBopC.jpg)

Per region settings:
* Range summer/winter: The min and max height for the region in the summer and winter, between 0 and 1
* Diffuse texture: The texture
* Color summer/winter: Texture tint in the summer/winter
* Texture tiling: The amount of times the texture is tiled across the entire terrain
* Tree name: The name of the tree object to use for trees in this region.
* Tree dencity: The chance to spawn a tree per world unit.
## Task 2 - Glider
![Image of the glider in the scene editor](https://i.imgur.com/2GMIQGI.png)

* Pitch, roll and yaw controlls.
* Moving parts: joystick, throttle, propeller, alierons and tail fin.
* Cockpit, tail, wing and chase camera
* Pseudo gravity and drag

## Controls

Day/Night cycle
* 0600: 1
* 1200: 2
* 1800: 3
* 2400: 4
* Toggle cycle: 5

Seasons
* Month 3:  6
* Month 6:  7
* Month 9:  8
* Month 12: 9
* Toggle cycle: 0

Toggle contour lines: O

Gilder:
* Pitch: W / S
* Roll: A / D
* Yaw: Q / E
* Throttle: , / .
* Reset position: R
* Cycle waypoints: F

Freelook camera:
* Forward: I
* Back: K
* Left: J
* Right: L
* Up: Y
* Down H

Cycle cameras: -

Freelook and chase camera zoom: N/M

## Framework

### The framework was buildt as part of assignment 1 & 2

* Component based system where each 'game object' is only responsible for managing the components attached to it.  
Some component like the Transform and MeshRenderer receives special treatment, but the user is free to create any sort of component they want.
* Transforms can have a parent and translates/scales/rotates correctly with the parent. This is shown in the Transforms test scene.
* The SceneManager provides an easy way to create multiple seperate scenes and change between them. Multiple scenes can be loaded at once.
* The Model-, Shader- and TextureManagers help load their respective kind of asset and prevents duplication if multiple components use the same asset.
* There are a few UI elements (Button, Label, Layout and padding) that makes setting up a simple menu or HUD a pretty trivial task.  
The UI elements can be anchored to any part of the screen and the position is automaticly updated when the windows is resized.
* InputManager for eays access to both mouse and keyboard input.
* The MeshRenderer components are sorted by material and mesh to reduse the overhead of repeatedly switching materials.  
GPU Instancing is also supported
* Scene editor with custom component inspectors
* Phong lighting model with diffuse, specular, emission and normal maps.
* Multiple directional and point lights.
* Multiple directional shadow maps and a single cube map.
* Skybox.
* FreeType text.

## Setup

### Prerequisites
Complier with support for C++ 17 and std::filesystem / std::experimental::filesystem

### Windows - Visual Studio 2017 with Visual Studio Tools for CMake
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
* [RapidJSON](https://github.com/Tencent/rapidjson)
* [ImGui](https://github.com/ocornut/imgui)
* [tiny file dialogs](https://sourceforge.net/projects/tinyfiledialogs/)

## Authors

* **Eivind Vold Aunebakk** - [Voldakk](https://github.com/Voldakk)

## Acknowledgments

* [TheChernoProject - Abstracting OpenGL into Classes](https://www.youtube.com/watch?v=bTHqmzjm2UI)
* [TheChernoProject - Buffer Layout Abstraction in OpenGL](https://www.youtube.com/watch?v=oD1dvfbyf6A)
* [TheChernoProject - Shader Abstraction in OpenGL](https://www.youtube.com/watch?v=gDtHL6hy9R8)
* [TheChernoProject - ImGui in OpenGL](https://www.youtube.com/watch?v=nVaQuNXueFw)
* [LearnOpenGL - Basic Lighting](https://learnopengl.com/Lighting/Basic-Lighting)
* [LearnOpenGL - Multiple lights](https://learnopengl.com/Lighting/Multiple-lights)
* [LearnOpenGL - Gamma Correction](https://learnopengl.com/Advanced-Lighting/Gamma-Correction)
* [LearnOpenGL - Shadow Mapping](https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping)
* [LearnOpenGL - Point Shadows](https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows)
* [LearnOpenGL - Normal Mapping](https://learnopengl.com/Advanced-Lighting/Normal-Mapping)
* [LearnOpenGL - Instancing](https://learnopengl.com/Advanced-OpenGL/Instancing)
* [LearnOpenGL - Text Rendering](https://learnopengl.com/In-Practice/Text-Rendering)
* [opengl-tutorial - Picking with custom Ray-OBB function](http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-custom-ray-obb-function/)
* [Stack Overflow - String to type mapping](https://stackoverflow.com/a/582456)
* [Stack Overflow - Full path to the executing app](https://stackoverflow.com/a/198099)
* [Stack Overflow - How to calculate normals in a terrain height grid](https://stackoverflow.com/a/13983431)
