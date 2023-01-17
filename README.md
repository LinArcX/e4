# e4
Implementation of e4_spec

# Prerequisites
- git
- gcc(clang)
- SDL2-devel

# Build/Run/Clean
## Linue
```
chmod +x ./build/tools/scripts/shell/build.sh
source ./build/tools/scripts/shell/build.sh
```

Now you can use bash functions as operations.

- Build the project: `build_debug` or `build_release`

- Run it: `run_debug` or `run_release`

- Clean the build directory: `clean`

## Windows
### With Visual Studio's IDE

Use the provided project file (.vcxproj). Add to solution (imgui_examples.sln) if necessary.

### With Visual Studio's CLI

```
set SDL2_DIR=path_to_your_sdl2_folder
cl /Zi /MD /I.. /I..\.. /I%SDL2_DIR%\include main.cpp ..\..\backends\imgui_impl_sdl.cpp ..\..\backends\imgui_impl_opengl3.cpp ..\..\imgui*.cpp /FeDebug/example_sdl_opengl3.exe /FoDebug/ /link /libpath:%SDL2_DIR%\lib\x86 SDL2.lib SDL2main.lib opengl32.lib /subsystem:console
#          ^^ include paths                  ^^ source files                                                                                   ^^ output exe                    ^^ output dir   ^^ libraries

# or for 64-bit:
cl /Zi /MD /I.. /I..\.. /I%SDL2_DIR%\include main.cpp ..\..\backends\imgui_impl_sdl.cpp ..\..\backends\imgui_impl_opengl3.cpp ..\..\imgui*.cpp /FeDebug/example_sdl_opengl3.exe /FoDebug/ /link /libpath:%SDL2_DIR%\lib\x64 SDL2.lib SDL2main.lib opengl32.lib /subsystem:console
```
