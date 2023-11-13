cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(wpal_sample LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE CMAKE_VS_DEBUGGER_COMMAND)
endif()

set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# set(SHORT_WCHAR ON)

find_package(ODBC REQUIRED)

# add_compile_definitions(SQL_WCHART_CONVERT)
include(FetchContent)
FetchContent_Declare(
  poco
  GIT_REPOSITORY "https://github.com/pocoproject/poco.git"
  GIT_TAG "poco-1.11.8-release"
)
FetchContent_GetProperties(poco)

if(NOT Poco_POPULATED)
  FetchContent_Populate(poco)
  add_subdirectory(${poco_SOURCE_DIR} ${poco_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# FetchContent_MakeAvailable(poco)
add_subdirectory(coreclr)

file(GLOB wpal_sources mingw/*)
add_library(wpal STATIC ${wpal_sources})
add_dependencies(wpal Poco::Data Poco::DataODBC Poco::Foundation)
target_include_directories(wpal PUBLIC mingw Poco::Data Poco::DataODBC Poco::Foundation)
target_link_libraries(wpal PUBLIC Poco::Data Poco::DataODBC Poco::Foundation coreclrpal palrt)

if(SHORT_WCHAR)
  target_compile_options(palrt PUBLIC -fshort-wchar)
endif(SHORT_WCHAR)

add_executable(wpal_sample wpal_sample.cpp use_odbc.cpp odbc_connector.h odbc_connector.cpp)
target_link_libraries(wpal_sample PUBLIC wpal)
