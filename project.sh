#!/usr/bin/bash
#
# we're our terminal/shell as our IDE! it's a unixy way to develop software :)
# there's a project.sh in roof of the project. before doing anything, source it: . project.sh
#
# tools:
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
# debugging: gdb build/output/linux/debug/e4

p() {
  # raw command for building:
  # c++  -I .. -I ../.. -I ../../backends main.cpp ../../backends/imgui_impl_sdl.cpp ../../backends/imgui_impl_opengl3.cpp ../../imgui*.cpp `sdl2-config --libs` -lGL -ldl

  compiler="clang++"

  src="src/*.c"
  source_dirs="src/*.cpp src/ui/*.cpp dependencies/include/imgui/ImGuiFileDialog.cpp"

  imgui_dependencies="dependencies/include/imgui/backends/imgui_impl_sdl.cpp dependencies/include/imgui/backends/imgui_impl_opengl3.cpp dependencies/include/imgui/imgui*.cpp"
  include_dirs="-I src/ -I src/ui -I src/fonts -I dependencies/include/imgui -I dependencies/include/imgui/backends"

  general_flags="-Wall -std=c++17"
  debug_only_flags="-g -O0"
  release_only_flags="-O3"

  loader_flags="-lGL -ldl"
  sdl_include_dir="`sdl2-config --cflags`"
  sdl_include_libs_dir="`sdl2-config --libs`"

  output_debug="-o build/output/linux/debug/e4"
  output_release="-o build/output/linux/release/e4"

  commands=("build(debug)" "debug" "run(debug)" "clean(debug)" "build(release)" "run(release)" "clean(release)" "generate tags")
  selected=$(printf '%s\n' "${commands[@]}" | fzf --header="project:")

  case $selected in
    "build(debug)")
      echo ">>> Creating 'build/output/linux/debug' directory"
      mkdir -p "build/output/linux/debug"

      echo ">>> generating tags"
      ctags --c++-kinds=+p --fields=+iaS --extras=+q --extras=+f -R *

      echo ">>> Building app - (debug) mode"
      $compiler $general_compiler_flags $debug_only_flags $loader_flags $sdl_include_dir $sdl_include_libs_dir $imgui_dependencies $include_dirs $source_dirs $output_debug
      ;;
    "debug")
      echo ">>> Debugging e4"
      gdb build/output/linux/debug/e4
      ;;
    "run(debug)")
      echo ">>> Running e4(debug mode)"
      ./build/output/linux/debug/e4 &
      ;;
    "clean(debug)")
      echo ">>> Cleaning 'build/output/linux/debug' directory"
      rm -r "build/output/linux/debug" ;;
    "build(release)")
      echo ">>> Creating 'build/output/linux/release' directory"
      mkdir -p "build/output/linux/release"

      echo ">>> generating tags"
      ctags --c++-kinds=+p --fields=+iaS --extras=+q --extras=+f -R *

      echo ">>> Building app - (release) mode"
      $compiler $general_compiler_flags $release_only_flags $loader_flags $sdl_include_dir $sdl_include_libs_dir $imgui_dependencies $include_dirs $source_dirs $output_release
      ;;
    "run(release)")
      echo ">>> Running e4(release mode)"
      ./build/output/linux/release/e4
      ;;
    "clean(release)")
      echo ">>> Cleaning 'build/output/linux/release' directory"
      rm -r "build/output/linux/release" ;;
    "generate tags")
      ctags --c++-kinds=+p --fields=+iaS --extras=+q --extras=+f -R *;;
    *) ;;
  esac
}
