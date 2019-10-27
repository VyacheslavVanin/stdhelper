cmake_minimum_required(VERSION 3.1)
project(vvvstdhelper)

set (CMAKE_CXX_STANDARD 14)


# Set Source files
file(GLOB_RECURSE HDRS include/vvvstdhelper/*.hpp)

add_library(vvvstdhelper INTERFACE)


option(WITH_TESTS "BUILD WITH TESTS" OFF)
if(WITH_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif(WITH_TESTS)

# Installation
set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "..." FORCE)
install(DIRECTORY include/vvvstdhelper
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp")
