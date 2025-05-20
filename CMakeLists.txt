cmake_minimum_required(VERSION 3.11)
project(Almondbread)

include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -O3 -Wshadow -Werror -Wall -Wextra -pedantic -msse4")
set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)

# add dependencies
include(${CMAKE_CURRENT_LIST_DIR}/cmake/CPM.cmake)

CPMFindPackage(
    NAME SDL2
    VERSION 2.32.6
    TAG release-2.32.6
    URI"gh:libsdl-org/SDL"
)

set(SOURCE_FILES ./src/main.cpp ./src/Mandelbrot.hpp)

add_executable(almondbread ${SOURCE_FILES})
target_link_libraries(almondbread PRIVATE SDL2::SDL2 pthread)

enable_testing()

CPMFindPackage(
    NAME GTest
    VERSION 1.17.0
    TAG v1.17.0 
    URI "gh:google/googletest"
)

add_executable(almondbreadtest ./test/AlmondbreadTest.cpp)
target_link_libraries(almondbreadtest GTest::gtest_main GTest::gtest)
target_include_directories(almondbreadtest PRIVATE ./src/)
add_test(NAME almondbreadtest COMMAND almondbreadtest)
