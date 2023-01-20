# Cross-Platform build script
# TODO:
# 1. make it debug/release aware.
# 2. make it windows/linux aware.

echo ">>> Creating 'build/output/linux/debug' directory"
mkdir build/output/linux/debug

echo ">>> Building app (Debug mode)"

let compiler = "clang++"

########################
let debug_flags_list = [
  "-g"
]
let debug_flags = ($debug_flags_list | str join ' ')

########################
let general_compiler_flags_list = [
  "-Wall"
  "-std=c++11"
]
let general_compiler_flags = ($general_compiler_flags_list | str join ' ')

#########################
let clang_flags_list = [
  "-MJ temp.txt"
]
let clang_flags = ($clang_flags_list | str join ' ')

#########################
let loader_flags_list = [
  "-lGL"
  "-ldl"
]
let loader_flags = ($loader_flags_list | str join ' ')

########################
let sdl_include_dir = ((sdl2-config --cflags) | str trim)
let sdl_include_libs_dir = ((sdl2-config --libs) | str trim)

########################
let output = "-o build/output/linux/debug/e4"

########################
let imgui_dependencies_list = [
  "dependencies/include/imgui/backends/imgui_impl_sdl.cpp"
  "dependencies/include/imgui/backends/imgui_impl_opengl3.cpp"
  "dependencies/include/imgui/imgui*.cpp"
]
let imgui_dependencies = ($imgui_dependencies_list | str join ' ')

########################
let include_dirs_list = [
  "-I src/"
  "-I src/ui"
  "-I src/fonts"
  "-I dependencies/include/imgui"
  "-I dependencies/include/imgui/backends"
]
let include_dirs = ($include_dirs_list | str join ' ')

########################
let source_dirs_list = [
  "src/*.cpp"
  "src/ui/*.cpp"
]
let source_dirs = ($source_dirs_list | str join ' ')

########################
let full_command = ($compiler
                  + " " + $debug_flags
                  + " " + $general_compiler_flags
                  + " " + $clang_flags
                  + " " + $loader_flags
                  + " " + $sdl_include_dir
                  + " " + $sdl_include_libs_dir
                  + " " + $imgui_dependencies
                  + " " + $output
                  + " " + $include_dirs
                  + " " + $source_dirs)
nu -c $full_command

echo ">>> Generating compile_commands.json"
open temp.txt | str trim | $"[\n($in)\n]" | save compile_commands.txt
mv compile_commands.txt compile_commands.json
rm temp.txt
