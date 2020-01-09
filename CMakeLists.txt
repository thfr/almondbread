cmake_minimum_required(VERSION 3.5)
project(Almondbred)

include(ExternalProject)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -O3 -Wshadow -Werror -Wall -Wextra -pedantic")
set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)

add_subdirectory(./thirdparty/SDL2-2.0.10/)

set(SOURCE_FILES ./src/main.cpp ./src/Mandelbrot.hpp)

add_executable(almondbread ${SOURCE_FILES})
add_dependencies(almondbread SDL2-static)
target_link_libraries(almondbread SDL2-static pthread)

enable_testing()
find_package(GTest REQUIRED)
add_executable(almondbreadtest ./test/AlmondbreadTest.cpp)
target_link_libraries(almondbreadtest GTest::GTest GTest::Main)
target_include_directories(almondbreadtest PRIVATE ./src/)
add_test(NAME almondbreadtest COMMAND almondbreadtest)
