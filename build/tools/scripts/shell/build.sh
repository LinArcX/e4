#!/bin/sh

build_debug() {
  echo ">>> Creating build directory"
  mkdir -p build/output/linux/debug

  echo ">>> Building app (Debug mode)"
  $CXX \
    -g \
    -lGL -ldl \
    -I src/ -I dependencies/include/imgui -I dependencies/include/imgui/backends \
    dependencies/include/imgui/backends/imgui_impl_sdl.cpp dependencies/include/imgui/backends/imgui_impl_opengl3.cpp dependencies/include/imgui/imgui*.cpp \
    `sdl2-config --cflags --libs`  \
    src/*.cpp \
    -o build/output/linux/debug/e4
}

build_release() {
  echo ">>> Creating build directory"
  mkdir -p build/output/linux/release

  echo ">>> Building app (Release mode)"
  $CXX \
    -lGL -ldl \
    -I src/ -I dependencies/include/imgui -I dependencies/include/imgui/backends \
    dependencies/include/imgui/backends/imgui_impl_sdl.cpp dependencies/include/imgui/backends/imgui_impl_opengl3.cpp dependencies/include/imgui/imgui*.cpp \
    `sdl2-config --cflags --libs`  \
    src/*.cpp \
    -o build/output/linux/release/e4
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
