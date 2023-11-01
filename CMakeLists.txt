cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# # add_compile_definitions(SQL_WCHART_CONVERT)
# include(FetchContent)
# FetchContent_Declare(
#   poco
#   GIT_REPOSITORY "https://github.com/pocoproject/poco.git"
#   GIT_TAG "poco-1.11.8-release"
# )
# FetchContent_GetProperties(poco)
# if(NOT poco_POPULATED)
#   FetchContent_Populate(poco)
#   # target_compile_definitions(poco INTERFACE SQL_WCHART_CONVERT)  
#   # set(SQL_WCHART_CONVERT ON)
  
#   add_subdirectory(${poco_SOURCE_DIR} ${poco_BINARY_DIR} EXCLUDE_FROM_ALL)
#   set_property(DIRECTORY ${poco_SOURCE_DIR} APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS SQL_WCHART_CONVERT)
# endif()
# FetchContent_MakeAvailable(poco)

project(wpal_sample LANGUAGES CXX)

# set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_subdirectory(coreclr)
add_executable(wpal_sample wpal_sample.cpp)
target_link_libraries(wpal_sample PUBLIC wpal)
# message(WARNING "CMAKE_BINARY_DIR/lib = ${CMAKE_BINARY_DIR}/lib")
# target_link_directories(wpal_sample PUBLIC ${CMAKE_BINARY_DIR}/lib)
