#!/usr/bin/bash
#c++  -I .. -I ../.. -I ../../backends main.cpp ../../backends/imgui_impl_sdl.cpp ../../backends/imgui_impl_opengl3.cpp ../../imgui*.cpp `sdl2-config --libs` -lGL -ldl

p() {
  compiler="clang++"

  src="src/*.c"
  source_dirs="src/*.cpp src/ui/*.cpp dependencies/include/imgui/ImGuiFileDialog.cpp"

  imgui_dependencies="dependencies/include/imgui/backends/imgui_impl_sdl.cpp dependencies/include/imgui/backends/imgui_impl_opengl3.cpp dependencies/include/imgui/imgui*.cpp"
  include_dirs="-I src/ -I src/ui -I src/fonts -I dependencies/include/imgui -I dependencies/include/imgui/backends"

  debug_only_flags="-g"
  general_flags="-Wall -std=c++17"
  loader_flags="-lGL -ldl"
  sdl_include_dir="`sdl2-config --cflags`"
  sdl_include_libs_dir="`sdl2-config --libs`"

  output_debug="-o build/output/linux/debug/e4"
  output_release="-o build/output/linux/release/e4"

  commands=("build(debug)" "debug" "clean(debug)" "build(release)" "run" "clean(release)" "generate tags")
  selected=$(printf '%s\n' "${commands[@]}" | fzf --header="project:")

  case $selected in
    "build(debug)")
      echo ">>> Creating 'build/output/linux/debug' directory"
      mkdir -p "build/output/linux/debug"
      echo ">>> Building app - (debug) mode"
      $compiler $general_compiler_flags $debug_only_flags $loader_flags $sdl_include_dir $sdl_include_libs_dir $imgui_dependencies $include_dirs $source_dirs $output_debug
      ;;
    "debug")
      echo ">>> Debugging e4"
      ./build/output/linux/debug/e4 &
      ;;
    "clean(debug)")
      echo ">>> Cleaning 'build/output/linux/debug' directory"
      rm -r "build/output/linux/debug" ;;
    "build(release)")
      echo ">>> Creating 'build/output/linux/release' directory"
      mkdir -p "build/output/linux/release"
      echo ">>> Building app - (release) mode"
      $compiler $general_compiler_flags $loader_flags $sdl_include_dir $sdl_include_libs_dir $imgui_dependencies $include_dirs $source_dirs $output_release
      ;;
    "run")
      echo ">>> Running e4"
      cd "build/output/linux/release"
      ./e4 &
      cd ../../../..
      ;;
    "clean(release)")
      echo ">>> Cleaning 'build/output/linux/release' directory"
      rm -r "build/output/linux/release" ;;
    "generate tags")
      ctags -R *;;
    *) ;;
  esac
}
