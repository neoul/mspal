cmake_minimum_required(VERSION 3.20)

# SET (CMAKE_BUILD_TYPE Debug) # Debug, Release, RelWithDebInfo, MinSizeRel
# SET(CMAKE_VERBOSE_MAKEFILE true) # false if you don't want verbose output

# # build output directories
# top project가 아닐경우 동작 X
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# Set the project name
project(CoreCLR)

set(WCHAR_4BYTES ON)
if (WCHAR_4BYTES)
  add_definitions(-DWCHAR_4BYTES)
endif (WCHAR_4BYTES)

include(configuretarget.cmake)
include(configurepaths.cmake)
include(configuretools.cmake)
include(configurecompiler.cmake)

include_directories("${CLR_SRC_NATIVE_DIR}")
include_directories("${CLR_SRC_NATIVE_DIR}/inc")

if(MSVC)
  set(CMAKE_CXX_STANDARD_LIBRARIES "") # do not link against standard win32 libs i.e. kernel32, uuid, user32, etc.
endif (MSVC)

# Set commonly used directory names
set(CLR_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(VM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vm)
set(GENERATED_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/inc)

# Avoid logging when skipping up-to-date copies
set(CMAKE_INSTALL_MESSAGE LAZY)

set(CORECLR_SET_RPATH ON)
if(CORECLR_SET_RPATH)
    # Enable @rpath support for shared libraries.
    set(MACOSX_RPATH ON)
endif(CORECLR_SET_RPATH)

if(CLR_CMAKE_HOST_MACCATALYST OR CLR_CMAKE_HOST_IOS OR CLR_CMAKE_HOST_TVOS)
  set(FEATURE_STANDALONE_GC 0)
endif()

OPTION(CLR_CMAKE_ENABLE_CODE_COVERAGE "Enable code coverage" OFF)

#----------------------------------------------------
# Cross target Component build specific configuration
#----------------------------------------------------
if(CLR_CROSS_COMPONENTS_BUILD)
  add_definitions(-DCROSS_COMPILE)

  if(CLR_CMAKE_HOST_ARCH_AMD64 AND (CLR_CMAKE_TARGET_ARCH_ARM OR CLR_CMAKE_TARGET_ARCH_I386))
    set(FEATURE_CROSSBITNESS 1)
  endif(CLR_CMAKE_HOST_ARCH_AMD64 AND (CLR_CMAKE_TARGET_ARCH_ARM OR CLR_CMAKE_TARGET_ARCH_I386))
endif(CLR_CROSS_COMPONENTS_BUILD)

#-------------------------
# Enable C++ EH with SEH
#-------------------------
if (MSVC)
  set_property(DIRECTORY PROPERTY CLR_EH_OPTION /EHa) # enable C++ EH (w/ SEH exceptions)
endif()

# #-------------------------------
# # Include libraries native shims
# #-------------------------------
# if(NOT CLR_CROSS_COMPONENTS_BUILD)
#     set(STATIC_LIBS_ONLY 1)
#     add_subdirectory(${CLR_SRC_NATIVE_DIR}/libs libs-native)
# endif(NOT CLR_CROSS_COMPONENTS_BUILD)

#-----------------------------------------
# Add Projects
#     - project which require platform header not clr's
#     - do not depend on clr's compile definitions
#-----------------------------------------
if(CLR_CMAKE_HOST_UNIX)
    if(CLR_CMAKE_TARGET_ANDROID AND NOT DEFINED CMAKE_ANDROID_NDK)
        find_library(LZMA NAMES lzma)
        if(LZMA STREQUAL LZMA-NOTFOUND)
           message(FATAL_ERROR "Cannot find liblzma.")
        endif(LZMA STREQUAL LZMA-NOTFOUND)

        find_library(ANDROID_GLOB NAMES android-glob)
        if(ANDROID_GLOB STREQUAL ANDROID_GLOB-NOTFOUND)
          message(FATAL_ERROR "Cannot find android-glob.")
        endif()
    endif()

    if(CLR_CMAKE_TARGET_LINUX AND (CLR_CMAKE_TARGET_ARCH_I386 OR CLR_CMAKE_TARGET_ARCH_RISCV64))
      add_link_options(LINKER:-z,notext)
    endif()

    if(NOT CLR_CMAKE_HOST_MACCATALYST AND NOT CLR_CMAKE_HOST_IOS AND NOT CLR_CMAKE_HOST_TVOS)
      add_subdirectory(pal)
      # add_subdirectory(hosts)
    endif()
else()
    if(CLR_CMAKE_TARGET_UNIX)
        add_subdirectory(${CLR_SRC_NATIVE_DIR}/external/libunwind_extras ${CLR_ARTIFACTS_OBJ_DIR}/external/libunwind)
    endif()
endif()

# # Add this subdir. We install the headers for the jit.
# add_subdirectory(pal/prebuilt/inc)

# # These need to happen before the VM and debug-pal includes.
# set(EP_GENERATED_HEADER_PATH "${GENERATED_INCLUDE_DIR}")
# include (${CLR_SRC_NATIVE_DIR}/eventpipe/configure.cmake)

# if(NOT CLR_CMAKE_HOST_MACCATALYST AND NOT CLR_CMAKE_HOST_IOS AND NOT CLR_CMAKE_HOST_TVOS)
#   add_subdirectory(debug/debug-pal)
# endif()

add_subdirectory(minipal)

if(CLR_CMAKE_TARGET_WIN32)
  add_subdirectory(gc/sample)
endif()

#-------------------------------------
# Include directory directives
#-------------------------------------
# Include the basic prebuilt headers - required for getting fileversion resource details.
include_directories("pal/prebuilt/inc")
include_directories(${CLR_ARTIFACTS_OBJ_DIR})

# Above projects do not build with these compile options
# All of the compiler options are specified in file compileoptions.cmake
# Do not add any new options here. They should be added in compileoptions.cmake
if(CLR_CMAKE_HOST_WIN32)
  add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:/Zl>) # omit default library name in .OBJ
endif(CLR_CMAKE_HOST_WIN32)

if (CLR_CMAKE_HOST_UNIX)
  include_directories("pal/inc")
  include_directories("pal/inc/rt")
  include_directories("pal/src/safecrt")
endif (CLR_CMAKE_HOST_UNIX)

#------------------------------
# Add Product Directories
#------------------------------
include_directories("inc")
include_directories("debug/inc")
include_directories("debug/inc/${ARCH_SOURCES_DIR}")
include_directories("debug/inc/dump")
include_directories("md/inc")
include_directories("classlibnative/bcltype")
include_directories("classlibnative/inc")
include_directories("${GENERATED_INCLUDE_DIR}")
include_directories("hosts/inc")
include_directories("minipal")

if(NOT CLR_CMAKE_HOST_MACCATALYST AND NOT CLR_CMAKE_HOST_IOS AND NOT CLR_CMAKE_HOST_TVOS)
  # add_subdirectory(utilcode)
  # add_subdirectory(inc)

  if(CLR_CMAKE_HOST_UNIX)
      add_subdirectory(palrt)
  endif(CLR_CMAKE_HOST_UNIX)

  # add_subdirectory(ilasm)
  # add_subdirectory(ildasm)
  # add_subdirectory(gcinfo)
  # add_subdirectory(jit)
  # add_subdirectory(vm)
  # add_subdirectory(md)
  # add_subdirectory(debug)
  # add_subdirectory(binder)
  # add_subdirectory(classlibnative)
  # add_subdirectory(dlls)
  # add_subdirectory(tools)
  # add_subdirectory(unwinder)
  # add_subdirectory(interop)

  # if(CLR_CMAKE_HOST_WIN32)
  #   add_subdirectory(hosts)
  # endif(CLR_CMAKE_HOST_WIN32)
endif(NOT CLR_CMAKE_HOST_MACCATALYST AND NOT CLR_CMAKE_HOST_IOS AND NOT CLR_CMAKE_HOST_TVOS)

