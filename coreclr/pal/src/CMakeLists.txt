if(CLR_CMAKE_TARGET_OSX OR CLR_CMAKE_TARGET_FREEBSD)
  # On OSX and *BSD, we use the libunwind that's part of the OS
  set(CLR_CMAKE_USE_SYSTEM_LIBUNWIND 1)
endif(CLR_CMAKE_TARGET_OSX OR CLR_CMAKE_TARGET_FREEBSD)

if(NOT DEFINED ENV{ROOTFS_DIR})
  include_directories(SYSTEM /usr/local/include)
elseif (CLR_CMAKE_TARGET_FREEBSD)
  include_directories(SYSTEM $ENV{ROOTFS_DIR}/usr/local/include)
endif()

if(NOT CLR_CMAKE_USE_SYSTEM_LIBUNWIND)
  include_directories(${CLR_SRC_NATIVE_DIR}/external/libunwind/include)
  include_directories(${CLR_SRC_NATIVE_DIR}/external/libunwind/include/tdep)
  include_directories(${CLR_ARTIFACTS_OBJ_DIR}/external/libunwind/include)
  include_directories(${CLR_ARTIFACTS_OBJ_DIR}/external/libunwind/include/tdep)

  add_subdirectory(${CLR_SRC_NATIVE_DIR}/external/libunwind_extras ${CLR_ARTIFACTS_OBJ_DIR}/external/libunwind)
elseif(NOT CLR_CMAKE_TARGET_OSX)
  find_unwind_libs(UNWIND_LIBS)
else()
  add_subdirectory(${CLR_SRC_NATIVE_DIR}/external/libunwind_extras ${CLR_ARTIFACTS_OBJ_DIR}/external/libunwind)
endif(NOT CLR_CMAKE_USE_SYSTEM_LIBUNWIND)

include(configure.cmake)

project(coreclrpal)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(CORECLR_SET_RPATH)
    # Enable @rpath support for shared libraries.
    set(MACOSX_RPATH ON)
endif(CORECLR_SET_RPATH)

# Include directories

include_directories(include)

# Compile options

if(CLR_CMAKE_HOST_ARCH_AMD64)
  set(PAL_ARCH_SOURCES_DIR amd64)
elseif(CLR_CMAKE_HOST_ARCH_ARM)
  set(PAL_ARCH_SOURCES_DIR arm)
elseif(CLR_CMAKE_HOST_ARCH_ARMV6)
  set(PAL_ARCH_SOURCES_DIR arm)
  add_definitions(-D__armv6__)
elseif(CLR_CMAKE_HOST_ARCH_ARM64)
  set(PAL_ARCH_SOURCES_DIR arm64)
elseif(CLR_CMAKE_HOST_ARCH_LOONGARCH64)
  set(PAL_ARCH_SOURCES_DIR loongarch64)
elseif(CLR_CMAKE_HOST_ARCH_RISCV64)
  set(PAL_ARCH_SOURCES_DIR riscv64)
elseif(CLR_CMAKE_HOST_ARCH_I386)
  set(PAL_ARCH_SOURCES_DIR i386)
elseif(CLR_CMAKE_HOST_ARCH_S390X)
  set(PAL_ARCH_SOURCES_DIR s390x)
elseif(CLR_CMAKE_HOST_ARCH_POWERPC64)
  set(PAL_ARCH_SOURCES_DIR ppc64le)
endif()

if(CLR_CMAKE_USE_SYSTEM_LIBUNWIND)
  add_definitions(-DFEATURE_USE_SYSTEM_LIBUNWIND)
endif(CLR_CMAKE_USE_SYSTEM_LIBUNWIND)

if(CLR_CMAKE_TARGET_OSX)
  add_definitions(-DTARGET_OSX)
  if(CLR_CMAKE_TARGET_ARCH_AMD64)
    add_definitions(-DXSTATE_SUPPORTED)
  endif()
  set(PLATFORM_SOURCES
    arch/${PAL_ARCH_SOURCES_DIR}/context.S
    arch/${PAL_ARCH_SOURCES_DIR}/dispatchexceptionwrapper.S
    exception/machexception.cpp
    exception/machmessage.cpp
  )
endif(CLR_CMAKE_TARGET_OSX)

if (FEATURE_ENABLE_NO_ADDRESS_SPACE_RANDOMIZATION)
  add_definitions(-DFEATURE_ENABLE_NO_ADDRESS_SPACE_RANDOMIZATION)
endif(FEATURE_ENABLE_NO_ADDRESS_SPACE_RANDOMIZATION)
add_definitions(-DLP64COMPATIBLE)
add_definitions(-DCORECLR)
add_definitions(-DPIC)

if(CLR_CMAKE_HOST_ARCH_AMD64 AND CLR_CMAKE_TARGET_LINUX AND NOT CLR_CMAKE_TARGET_LINUX_MUSL)
  # Currently the _xstate is not available on Alpine Linux
  add_definitions(-DXSTATE_SUPPORTED)
endif(CLR_CMAKE_HOST_ARCH_AMD64 AND CLR_CMAKE_TARGET_LINUX AND NOT CLR_CMAKE_TARGET_LINUX_MUSL)

if(CLR_CMAKE_TARGET_LINUX_MUSL)
  # Setting RLIMIT_NOFILE breaks debugging of coreclr on Alpine Linux for some reason
  add_definitions(-DDONT_SET_RLIMIT_NOFILE)
  # On Alpine Linux, we need to ensure that the reported stack range for the primary thread is
  # larger than the initial committed stack size.
  add_definitions(-DENSURE_PRIMARY_STACK_SIZE)
endif(CLR_CMAKE_TARGET_LINUX_MUSL)

# # turn off capability to remove unused functions (which was enabled in debug build with sanitizers)
# add_link_options($<$<AND:$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>,$<CONFIG:DEBUG>>:LINKER:--no-gc-sections>)

set(ARCH_SOURCES
  arch/${PAL_ARCH_SOURCES_DIR}/context2.S
  arch/${PAL_ARCH_SOURCES_DIR}/debugbreak.S
  arch/${PAL_ARCH_SOURCES_DIR}/exceptionhelper.S
)

if(NOT CLR_CMAKE_TARGET_OSX)
  list(APPEND PLATFORM_SOURCES
    arch/${PAL_ARCH_SOURCES_DIR}/callsignalhandlerwrapper.S
    arch/${PAL_ARCH_SOURCES_DIR}/signalhandlerhelper.cpp
  )
endif(NOT CLR_CMAKE_TARGET_OSX)

if(CLR_CMAKE_HOST_ARCH_ARM)
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set_source_files_properties(exception/seh.cpp PROPERTIES COMPILE_FLAGS -Wno-error=inline-asm)
  endif()
endif(CLR_CMAKE_HOST_ARCH_ARM)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND (CLR_CMAKE_HOST_ARCH_AMD64 OR CLR_CMAKE_HOST_ARCH_I386))
  add_compile_options(-Wa,--divide)
endif()

set(SOURCES
  cruntime/file.cpp
  cruntime/filecrt.cpp
  cruntime/malloc.cpp
  cruntime/math.cpp
  cruntime/misc.cpp
  cruntime/printfcpp.cpp
  cruntime/string.cpp
  cruntime/stringtls.cpp
  cruntime/thread.cpp
  cruntime/wchar.cpp
  debug/debug.cpp
  exception/seh.cpp
  exception/signal.cpp
  file/directory.cpp
  file/file.cpp
  file/filetime.cpp
  file/find.cpp
  file/path.cpp
  handlemgr/handleapi.cpp
  handlemgr/handlemgr.cpp
  init/pal.cpp
  init/sxs.cpp
  loader/module.cpp
  locale/unicode.cpp
  locale/unicodedata.cpp
  ${CLR_SRC_NATIVE_DIR}/minipal/utf8.c
  map/common.cpp
  map/map.cpp
  map/virtual.cpp
  misc/cgroup.cpp
  misc/dbgmsg.cpp
  misc/environ.cpp
  misc/error.cpp
  misc/errorstrings.cpp
  misc/fmtmessage.cpp
  misc/miscpalapi.cpp
  misc/perfjitdump.cpp
  misc/strutil.cpp
  misc/sysinfo.cpp
  misc/time.cpp
  misc/utils.cpp
  objmgr/palobjbase.cpp
  objmgr/shmobject.cpp
  objmgr/shmobjectmanager.cpp
  safecrt/makepath_s.cpp
  safecrt/memcpy_s.cpp
  safecrt/memmove_s.cpp
  safecrt/mbusafecrt.cpp
  safecrt/safecrt_input_s.cpp
  safecrt/safecrt_winput_s.cpp
  safecrt/splitpath_s.cpp
  safecrt/sprintf_s.cpp
  safecrt/sscanf_s.cpp
  safecrt/strcat_s.cpp
  safecrt/strcpy_s.cpp
  safecrt/strlen_s.cpp
  safecrt/strncat_s.cpp
  safecrt/strncpy_s.cpp
  safecrt/strtok_s.cpp
  safecrt/vsprintf.cpp
  safecrt/wcscat_s.cpp
  safecrt/wcscpy_s.cpp
  safecrt/wcslen_s.cpp
  safecrt/wcslwr_s.cpp
  safecrt/wcsncat_s.cpp
  safecrt/wcsncpy_s.cpp
  safecrt/wcstok_s.cpp
  safecrt/wmakepath_s.cpp
  safecrt/xtoa_s.cpp
  safecrt/xtow_s.cpp
  sharedmemory/sharedmemory.cpp
  shmemory/shmemory.cpp
  sync/cs.cpp
  synchobj/event.cpp
  synchobj/semaphore.cpp
  synchobj/mutex.cpp
  synchmgr/synchcontrollers.cpp
  synchmgr/synchmanager.cpp
  synchmgr/wait.cpp
  thread/context.cpp
  thread/process.cpp
  thread/thread.cpp
  thread/threadsusp.cpp
)

if(NOT CLR_CMAKE_USE_SYSTEM_LIBUNWIND)
  set(LIBUNWIND_OBJECTS $<TARGET_OBJECTS:libunwind>)
endif(NOT CLR_CMAKE_USE_SYSTEM_LIBUNWIND)

add_library(coreclrpal
  ${SOURCES}
  ${ARCH_SOURCES}
  ${PLATFORM_SOURCES}
  ${LIBUNWIND_OBJECTS}
)

#------------------------------------
# Definitions (for platform)
#-----------------------------------
# message(WARNING "CLR_CMAKE_HOST_ARCH_AMD64 = ${CLR_CMAKE_HOST_ARCH_AMD64}")
if (CLR_CMAKE_HOST_ARCH_AMD64)
  target_compile_definitions(coreclrpal PUBLIC -DHOST_AMD64 -DHOST_64BIT)
elseif (CLR_CMAKE_HOST_ARCH_I386)
  target_compile_definitions(coreclrpal PUBLIC -DHOST_X86)
elseif (CLR_CMAKE_HOST_ARCH_ARM)
  target_compile_definitions(coreclrpal PUBLIC -DHOST_ARM)
elseif (CLR_CMAKE_HOST_ARCH_ARMV6)
  target_compile_definitions(coreclrpal PUBLIC -DHOST_ARM)
  target_compile_definitions(coreclrpal PUBLIC -DHOST_ARMV6)
elseif (CLR_CMAKE_HOST_ARCH_ARM64)
  target_compile_definitions(coreclrpal PUBLIC -DHOST_ARM64 -DHOST_64BIT)
elseif (CLR_CMAKE_HOST_ARCH_LOONGARCH64)
  target_compile_definitions(coreclrpal PUBLIC -DHOST_LOONGARCH64 -DHOST_64BIT)
elseif (CLR_CMAKE_HOST_ARCH_RISCV64)
  target_compile_definitions(coreclrpal PUBLIC -DHOST_RISCV64 -DHOST_64BIT)
elseif (CLR_CMAKE_HOST_ARCH_S390X)
  target_compile_definitions(coreclrpal PUBLIC -DHOST_S390X -DHOST_64BIT -DBIGENDIAN)
elseif (CLR_CMAKE_HOST_ARCH_WASM)
  target_compile_definitions(coreclrpal PUBLIC -DHOST_WASM -DHOST_32BIT=1)
elseif (CLR_CMAKE_HOST_ARCH_MIPS64)
  target_compile_definitions(coreclrpal PUBLIC -DHOST_MIPS64 -DHOST_64BIT=1)
elseif (CLR_CMAKE_HOST_ARCH_POWERPC64)
  target_compile_definitions(coreclrpal PUBLIC -DHOST_POWERPC64 -DHOST_64BIT)
else ()
  clr_unknown_arch()
endif ()
target_compile_definitions(coreclrpal PUBLIC -DWCHAR_4BYTES)
target_include_directories(coreclrpal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../native)
target_include_directories(coreclrpal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../inc)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()


# There is only one function exported in 'tracepointprovider.cpp' namely 'PAL_InitializeTracing',
# which is guarded with '#if defined(__linux__)'. On macOS, Xcode issues the following warning:
#
# > warning: /Applications/Xcode-9.4.1.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib:
# > warning for library: libtracepointprovider.a the table of contents is empty (no object file members in the library define global symbols)
#
if(CLR_CMAKE_TARGET_LINUX)
  add_library(tracepointprovider OBJECT misc/tracepointprovider.cpp)
endif(CLR_CMAKE_TARGET_LINUX)

if(CLR_CMAKE_TARGET_OSX)
  find_library(COREFOUNDATION CoreFoundation)
  find_library(CORESERVICES CoreServices)
  find_library(SECURITY Security)
  find_library(SYSTEM System)
  target_link_libraries(coreclrpal
    PUBLIC
    ${COREFOUNDATION}
    ${CORESERVICES}
    ${SECURITY}
    ${SYSTEM}
  )
endif(CLR_CMAKE_TARGET_OSX)

if(CLR_CMAKE_TARGET_FREEBSD)
  target_link_libraries(coreclrpal
    PUBLIC
    pthread
    rt
    ${UNWIND_LIBS}
  )
endif(CLR_CMAKE_TARGET_FREEBSD)

if(CLR_CMAKE_TARGET_LINUX)
  # On Android, we don't need to link with gcc_s, pthread and rt
  if(NOT CLR_CMAKE_TARGET_ANDROID)
    target_link_libraries(coreclrpal
      PUBLIC
      gcc_s
      pthread
      rt
    )
  else(NOT CLR_CMAKE_TARGET_ANDROID)
    target_link_libraries(coreclrpal
      PUBLIC
      ${ANDROID_GLOB}
      ${LZMA})
  endif(NOT CLR_CMAKE_TARGET_ANDROID)

  target_link_libraries(coreclrpal
    PUBLIC
    dl
  )

  if(CLR_CMAKE_USE_SYSTEM_LIBUNWIND)
    target_link_libraries(coreclrpal PUBLIC ${UNWIND_LIBS})
  endif(CLR_CMAKE_USE_SYSTEM_LIBUNWIND)

  # bundled libunwind requires using libucontext on alpine and x86 and ppc64le
  if(CLR_CMAKE_TARGET_LINUX_MUSL AND (CLR_CMAKE_TARGET_ARCH_I386 OR CLR_CMAKE_TARGET_ARCH_POWERPC64))
    target_link_libraries(coreclrpal PUBLIC ucontext)
  endif(CLR_CMAKE_TARGET_LINUX_MUSL AND (CLR_CMAKE_TARGET_ARCH_I386 OR CLR_CMAKE_TARGET_ARCH_POWERPC64))

endif(CLR_CMAKE_TARGET_LINUX)

if(CLR_CMAKE_TARGET_NETBSD)
  if (CLR_CMAKE_USE_SYSTEM_LIBUNWIND)
    find_library(UNWIND unwind)
  endif()
  add_definitions(-D_KMEMUSER)
  find_library(KVM kvm)
  target_link_libraries(coreclrpal
    PUBLIC
    pthread
    rt
    ${UNWIND}
    ${KVM}
  )
endif(CLR_CMAKE_TARGET_NETBSD)

if(CLR_CMAKE_TARGET_SUNOS)
  target_link_libraries(coreclrpal
    PUBLIC
    pthread
    rt
  )
endif(CLR_CMAKE_TARGET_SUNOS)

# if(FEATURE_EVENT_TRACE)
#   add_subdirectory(eventprovider)
# endif(FEATURE_EVENT_TRACE)

# # Install the static PAL library for VS
# install_clr (TARGETS coreclrpal DESTINATIONS lib)
