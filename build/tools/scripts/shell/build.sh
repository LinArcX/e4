#!/bin/sh

build_debug()
{
  echo ">>> Creating 'build/output/linux/debug' directory"
  mkdir -p build/output/linux/debug

  echo ">>> Building app (Debug mode)"
  $CXX \
    -g \
    -Wall -std=c++11 \
    -lGL -ldl \
    -I src/ -I dependencies/include/imgui -I dependencies/include/imgui/backends \
    dependencies/include/imgui/backends/imgui_impl_sdl.cpp dependencies/include/imgui/backends/imgui_impl_opengl3.cpp dependencies/include/imgui/imgui*.cpp \
    `sdl2-config --cflags --libs`  \
    src/*.cpp \
    -MJ temp.json \
    -o build/output/linux/debug/e4

  echo ">>> Generating compile_commands.json"
  sed -e '1s/^/[\n/' -e '$s/,$/\n]/' temp.json > compile_commands.json
  rm temp.json
}

build_release()
{
  echo ">>> Creating 'build/output/linux/release' directory"
  mkdir -p build/output/linux/release

  echo ">>> Building app (Release mode)"
  $CXX \
    -Wall -std=c++11 \
    -lGL -ldl \
    -I src/ -I src/ui -I dependencies/include/imgui -I dependencies/include/imgui/backends \
    dependencies/include/imgui/backends/imgui_impl_sdl.cpp dependencies/include/imgui/backends/imgui_impl_opengl3.cpp dependencies/include/imgui/imgui*.cpp \
    `sdl2-config --cflags --libs`  \
    src/*.cpp src/ui/*.cpp \
    -MJ temp.json \
    -o build/output/linux/release/e4

  echo ">>> Generating compile_commands.json"
  sed -e '1s/^/[\n/' -e '$s/,$/\n]/' temp.json > compile_commands.json
  rm temp.json
}

clean() {
    echo ">>> Cleaning build directory"
    rm -r build/output/linux/
}

run_debug() {
    echo ">>> Running app (Debug mode)"
    cd build/output/linux/debug
    ./e4 &
    cd ../../../..
}

run_release() {
    echo ">>> Running app (Release mode)"
    cd build/output/linux/release
    ./e4 &
    cd ../../../..
}
