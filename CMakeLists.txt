cmake_minimum_required(VERSION 3.5)
project(Almondbred)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wshadow -Werror -Wall -Wextra -pedantic")
set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)

add_executable(almondbread ./src/main.cpp)

set(SOURCE_FILES ./src/main.cpp)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(almondbread SDL2)
