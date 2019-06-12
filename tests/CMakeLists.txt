cmake_minimum_required(VERSION 3.1.0)
project(vvvstdhelper_tests)

enable_testing()

set (CMAKE_CXX_STANDARD 14)
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS} .)

file(GLOB_RECURSE SRCS *.cpp)
file(GLOB_RECURSE HDRS *.h *.hpp)

include_directories(stdhelper/include)
add_executable(vvvstdhelper_test ${SRCS} ${HDRS})
target_link_libraries(vvvstdhelper_test ${GTEST_LIBRARIES} Threads::Threads)

add_test(vvvstdhelper_test vvvstdhelper_test)

