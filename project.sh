#!/usr/bin/bash
#
# we're our terminal/shell as our IDE! it's a unixy way to develop software :)
# there's a project.sh in roof of the project. before doing anything, source it: . project.sh
#
# tools:
# static code analyzer:
#   - c: splint
#   - c++: cppcheck
# memory debugger: valgrind
# system call tracer: strace
# display info about .obj files: objdump
#
# opening/editing files: noevim
#   folding/unfolding: z Shift+m, z Shift+r
#   switch between source/header: F1
#
# lookup refrences: ctags
# find/replace in single file: neovim
# find/replace in whole project: ambr <source_text> <dest_text>
# find files: ctrl-t | ff <file-name> | fzf | fd
# find string/text in single file: neovim (/)
# find string/text in whole project: ft <text> | rg <text>
# find docs of c standard librariy: install man-pages-devel and man <method>
#
# debugging: gdb build/output/linux/debug/$app
#   set breakpoint: b 1
#   start debugging: start
#   from this phase, for faster moving between files and methods, you can switch to single-key-mode: C-x s. and here are the commands in this mode:
#     q - quit, exit SingleKey mode.
#     c - continue
#     d - down
#     f - finish
#     n - next
#     r - run
#     s - step
#     u - up
#     v - info locals
#     w - where

app="e4"
mode="debug"
build_dir="build/output/linux"

p() {
  # raw command for building:
  # c++  -I .. -I ../.. -I ../../backends main.cpp ../../backends/imgui_impl_sdl.cpp ../../backends/imgui_impl_opengl3.cpp ../../imgui*.cpp `sdl2-config --libs` -lGL -ldl

  compiler="clang++"
  mode_flags=""

  src="src/*.c"
  source_dirs="src/*.cpp src/ui/*.cpp dependencies/include/imgui/ImGuiFileDialog.cpp"

  imgui_dependencies="dependencies/include/imgui/backends/imgui_impl_sdl.cpp dependencies/include/imgui/backends/imgui_impl_opengl3.cpp dependencies/include/imgui/imgui*.cpp"
  include_dirs="-I src/ -I src/ui -I src/fonts -I dependencies/include/imgui -I dependencies/include/imgui/backends"

  general_compiler_flags="-Wall -Wextra -pedantic -std=c++17"

  loader_flags="-lGL -ldl"
  sdl_include_dir="`sdl2-config --cflags`"
  sdl_include_libs_dir="`sdl2-config --libs`"

  output="-o $build_dir/$mode/$app"

  commands=("build" "debug" "run" "clean" "generate tags" "cppcheck" "valgrind"
    "find strings in the binary" "list symbols from object files")
  selected=$(printf '%s\n' "${commands[@]}" | fzf --header="project:")

  case $selected in
    "build")
      echo ">>> Creating '$build_dir/$mode' directory"
      mkdir -p "$build_dir/$mode"

      echo ">>> generating tags"
      ctags --c++-kinds=+p --fields=+iaS --extras=+q --extras=+f -R *

      echo ">>> Building app - $mode"
      if [ "$mode" == "debug" ]; then
        mode_flags="-g -O0"
      elif [ "$mode" == "release" ]; then
        mode_flags="-O3"
      else
        echo "Unknown mode!"
      fi

      $compiler $general_compiler_flags $mode_flags $loader_flags $sdl_include_dir $sdl_include_libs_dir $imgui_dependencies $include_dirs $source_dirs $output
      ;;
    "debug")
      if [ "$mode" == "debug" ]; then
        echo ">>> Debugging $app"
        gdb --tui $build_dir/$mode/$app
      else
        echo "you're not in debug mode!"
      fi
      ;;
    "run")
      echo ">>> Running $app - $mode"
      ./$build_dir/$mode/$app &
      ;;
    "clean")
      echo ">>> Cleaning '$build_dir/$mode' directory"
      rm -r "$build_dir/$mode" ;;
    "generate tags")
      ctags --c++-kinds=+p --fields=+iaS --extras=+q --extras=+f -R *;;
    "cppcheck")
      cppcheck --enable=all --platform=unix64 src/main.cpp src/ui/*;;
    "valgrind")
      valgrind --leak-check=yes --show-leak-kinds=all -s -v $build_dir/$mode/$app;;
      #valgrind --leak-check=full src/*.cpp ;;
    "find strings in the binary")
      strings $build_dir/$mode/$app | less;;
    "list symbols from object files")
      nm $build_dir/$mode/$app | less;;
    *) ;;
  esac
}
