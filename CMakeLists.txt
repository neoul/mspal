# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(my_windows_app LANGUAGES CXX)

add_executable(my_windows_app hello-world.cpp)
