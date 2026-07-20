# CornJam
A small game engine mostly made to make quick games, with the tradeoff being less control. 

## Supports:
- Textures
- Grid-based animation (I'll add .json support later)
- Compute shader-based particles! :\)
- `.wav` only Audio (I didn't add `dr_ogg` and `dr_flac` yet)
- Grid-based bitmap text rendering (I'll add freetype later)
- Custom game states
- Key input management
- AABB collision
- Virtual entity class (ugly) 

This was just made as a template for me to make a game in a game jam, hence the name CornJam.

## How To Build
I just built this with manual G++, sadly.

### Without Statically-Linked Libraries

`g++ src/main.cpp src/misc/glad.c src/misc/shaders.cpp src/game.cpp src/scenes.cpp src/misc/collision.cpp src/misc/utility.cpp src/globals.cpp -o bin/main.exe -I include -L lib -lglfw3dll -lOpenAL32`

`g++ src/main.cpp src/misc/glad.c src/misc/shaders.cpp src/game.cpp src/scenes.cpp src/misc/collision.cpp src/misc/utility.cpp src/globals.cpp -o bin/main.exe -I include -L lib -static -static-libgcc -static-libstdc++ -lglfw3 -lopengl32 -lgdi32 -lOpenAL32`
(you still need to keep the `openal32.dll` file though)