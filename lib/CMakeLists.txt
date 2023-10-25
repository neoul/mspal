file(GLOB LIBRARY_HEADERS *.h)
file(GLOB LIBRARY_SOURCES *.cpp)

set(ALL_FILES
    ${LIBRARY_HEADERS}
    ${LIBRARY_SOURCES}
)

add_library(Library STATIC ${ALL_FILES})
target_include_directories(Library PUBLIC "./")
target_link_libraries(Library PUBLIC coreclrpal)
