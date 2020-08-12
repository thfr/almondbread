cmake_minimum_required(VERSION 3.11)
project(Almondbread)

include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -O3 -Wshadow -Werror -Wall -Wextra -pedantic")
set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)

# Application
FetchContent_Declare(
    sdl2
    URL https://www.libsdl.org/release/SDL2-2.0.12.tar.gz
)

FetchContent_GetProperties(sdl2)
if(NOT sdl2_POPULATED)
    FetchContent_Populate(sdl2)
    add_subdirectory(${sdl2_SOURCE_DIR} ${sdl2_BINARY_DIR})
endif()
# FetchContent_MakeAvailable(sdl2)

set(SOURCE_FILES ./src/main.cpp ./src/Mandelbrot.hpp)

add_executable(almondbread ${SOURCE_FILES})
target_link_libraries(almondbread PRIVATE SDL2-static pthread)

# Tests
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/release-1.10.0.tar.gz
)
FetchContent_MakeAvailable(googletest)

add_executable(almondbreadtest ./test/AlmondbreadTest.cpp)
target_include_directories(almondbreadtest PRIVATE ./src/)
target_link_libraries(almondbreadtest PRIVATE gtest gtest_main)

enable_testing()
add_test(NAME almondbreadtest COMMAND almondbreadtest)
