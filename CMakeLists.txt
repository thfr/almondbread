cmake_minimum_required(VERSION 3.11)
project(Almondbread)

include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wshadow -Werror -Wall -Wextra -pedantic -msse4")
set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)

include(FetchContent)
FetchContent_Declare(sdl URL https://www.libsdl.org/release/SDL2-2.0.14.tar.gz)
FetchContent_MakeAvailable(sdl)

set(SOURCE_FILES ./src/main.cpp ./src/Mandelbrot.hpp)

add_executable(almondbread ${SOURCE_FILES})
target_link_libraries(almondbread PRIVATE SDL2-static pthread)


enable_testing()
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/release-1.10.0.tar.gz)
FetchContent_MakeAvailable(googletest)

add_executable(almondbreadtest ./test/AlmondbreadTest.cpp)
target_link_libraries(almondbreadtest gtest_main gtest)
target_include_directories(almondbreadtest PRIVATE ./src/)
add_test(NAME almondbreadtest COMMAND almondbreadtest)
