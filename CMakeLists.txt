cmake_minimum_required(VERSION 3.19)
project(pollock C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(pollock
        )

target_compile_options(pollock PRIVATE -pedantic -Wall -Wextra)
target_link_options(pollock PRIVATE -lm -lpthread -lpthread -lSDL2)

