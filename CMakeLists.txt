cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(wpal_sample LANGUAGES CXX)

# set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_subdirectory(coreclr)
add_executable(wpal_sample wpal_sample.cpp)
target_link_libraries(wpal_sample PUBLIC wpal)
