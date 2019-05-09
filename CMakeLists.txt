cmake_minimum_required(VERSION 3.1)
project(stdhelper)

set (CMAKE_CXX_STANDARD 14)

# Set include path
include_directories(src)

# Set Source files
file(GLOB_RECURSE HDRS src/*.hpp)
add_library(stdhelper STATIC ${SRCS} ${HDRS})
set_target_properties(stdhelper PROPERTIES LINKER_LANGUAGE CXX)


enable_testing()
add_subdirectory(tests)

# Installation
set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "..." FORCE)
set_target_properties(stdhelper PROPERTIES PUBLIC_HEADER "${HDRS}")
INSTALL(TARGETS stdhelper
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include/vvvstdhelper
)

