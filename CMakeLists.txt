cmake_minimum_required(VERSION 3.10)
project(minishell C)

set(CMAKE_C_STANDARD 99)
set(LIBFT_DIR ${PROJECT_SOURCE_DIR}/libft)
set(LIBFT libft.a)

set(GCC_NORM_COMPILE_FLAGS      "-Wall -Wextra -Werror")
set(GCC_SANITIZER_COMPILE_FLAGS "-fsanitize=address")
set(${CMAKE_C_FLAGS} "${CMAKE_C_FLAGS} ${GCC_NORM_COMPILE_FLAGS} ${GCC_SANITIZER_COMPILE_FLAGS}")

include_directories(includes ${LIBFT_DIR}/includes $ENV{HOME}/.brew/opt/readline/include)
link_directories(${LIBFT_DIR} ${HOME}/.brew/opt/readline/lib)

add_custom_target(
    ${LIBFT} ALL
    COMMAND make
    WORKING_DIRECTORY ${LIBFT_DIR}
)

file(GLOB_RECURSE SOURCES srcs/main.c srcs/**/*.c)

add_executable(minishell ${SOURCES})

add_dependencies(minishell ${LIBFT})

target_link_libraries(minishell ft termcap readline)

add_subdirectory(tests)
