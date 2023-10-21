project(COREPAL)

# add_definitions(-DPAL_STDCPP_COMPAT)
message(STATUS "HOST_AMD64=${HOST_AMD64}")

include_directories(${COREPAL_SOURCE_DIR}/inc)
include_directories(${COREPAL_SOURCE_DIR}/src)
include_directories(${COREPAL_SOURCE_DIR}/../inc)

add_compile_options(-fexceptions)
add_definitions(-DUSE_STL)

add_subdirectory(src)
# add_subdirectory(tests)
