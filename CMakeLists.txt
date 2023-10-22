# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(hello LANGUAGES CXX)
add_subdirectory(coreclr)
SET(CMAKE_VERBOSE_MAKEFILE true) # false if you don't want verbose output

add_executable(hello hello-world.cpp)
target_link_libraries(hello PUBLIC coreclrpal)
