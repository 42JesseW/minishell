cmake_minimum_required(VERSION 3.10)
project(tests C CXX)

set(HEADERS catch.hpp)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/includes .)

file(GLOB_RECURSE TEST_SOURCES ../srcs/executor/exe_paths.c ../srcs/executor/executor.c ../srcs/shell/*.c ../srcs/shell/**/*.c ../srcs/parser/*.c ../srcs/parser/**/*.c)

add_library(tester STATIC ${TEST_SOURCES})

file(GLOB_RECURSE SOURCES main.cpp unittests/*.cpp)

add_executable(tests ${SOURCES})

add_dependencies(tests ${LIBFT})

target_link_libraries(tests tester ft)
