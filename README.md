# JaJaEngine
A C++ game engine made from scratch with SDL2.

### Description:
The engine features a dynamic XML parsing system that allows for all properties, entities, maps, and map objects to be completely changed without recompiling the code. It uses a quadtree for collisions and was designed from the ground-up to be simple, expandable, and efficient.

![image](https://user-images.githubusercontent.com/52947350/174325602-c990e17c-2873-47ce-b816-d45ab52d87d0.png)

### Dependencies:
1. sdl2       2.0.22-2
2. sdl2_image 2.0.5-2
3. sdl2_ttf   2.0.18-1

### Build Instructions:
(First time)
1. Create a build directory in root and navigate into it from a terminal
2. Run "cmake .."

(Subsequent times)
1. Navigate to root from a terminal
2. Run "cmake --build ./build"   to build
3. Run "./build/source/JaJaEngine" to run
