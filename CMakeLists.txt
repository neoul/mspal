# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(bstr LANGUAGES CXX)
add_subdirectory(coreclr)
set(CMAKE_VERBOSE_MAKEFILE true) # false if you don't want verbose output
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# add_subdirectory(lib)
add_executable(bstr bstr.cpp)
target_compile_definitions(bstr PUBLIC USE_STD_LIBRARY)
target_link_libraries(bstr PUBLIC coreclrpal palrt)
