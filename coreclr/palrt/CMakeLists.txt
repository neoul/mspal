
set(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_VERBOSE_MAKEFILE true) # false if you don't want verbose output

set(PALRT_SOURCES
    comem.cpp
    guid.cpp
    memorystream.cpp
    variant.cpp
)

add_library(palrt
    ${PALRT_SOURCES}
)

if (CLR_CMAKE_HOST_ARCH_AMD64)
  target_compile_definitions(palrt PUBLIC -DHOST_AMD64 -DHOST_64BIT)
elseif (CLR_CMAKE_HOST_ARCH_I386)
  target_compile_definitions(palrt PUBLIC -DHOST_X86)
elseif (CLR_CMAKE_HOST_ARCH_ARM)
  target_compile_definitions(palrt PUBLIC -DHOST_ARM)
elseif (CLR_CMAKE_HOST_ARCH_ARMV6)
  target_compile_definitions(palrt PUBLIC -DHOST_ARM)
  target_compile_definitions(palrt PUBLIC -DHOST_ARMV6)
elseif (CLR_CMAKE_HOST_ARCH_ARM64)
  target_compile_definitions(palrt PUBLIC -DHOST_ARM64 -DHOST_64BIT)
elseif (CLR_CMAKE_HOST_ARCH_LOONGARCH64)
  target_compile_definitions(palrt PUBLIC -DHOST_LOONGARCH64 -DHOST_64BIT)
elseif (CLR_CMAKE_HOST_ARCH_RISCV64)
  target_compile_definitions(palrt PUBLIC -DHOST_RISCV64 -DHOST_64BIT)
elseif (CLR_CMAKE_HOST_ARCH_S390X)
  target_compile_definitions(palrt PUBLIC -DHOST_S390X -DHOST_64BIT -DBIGENDIAN)
elseif (CLR_CMAKE_HOST_ARCH_WASM)
  target_compile_definitions(palrt PUBLIC -DHOST_WASM -DHOST_32BIT=1)
elseif (CLR_CMAKE_HOST_ARCH_MIPS64)
  target_compile_definitions(palrt PUBLIC -DHOST_MIPS64 -DHOST_64BIT=1)
elseif (CLR_CMAKE_HOST_ARCH_POWERPC64)
  target_compile_definitions(palrt PUBLIC -DHOST_POWERPC64 -DHOST_64BIT)
else ()
  clr_unknown_arch()
endif ()
target_compile_definitions(palrt PUBLIC -DPAL_STDCPP_COMPAT)
target_include_directories(palrt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../pal/inc)
target_include_directories(palrt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../pal/inc/rt)
target_include_directories(palrt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../inc)


# # Install the static PAL library for VS
# install_clr(TARGETS palrt DESTINATIONS lib)
