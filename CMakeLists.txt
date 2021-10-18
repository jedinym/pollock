cmake_minimum_required(VERSION 3.19)
project(pollock CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(pollock
        src/pollock.cpp
        src/pollock.hpp
        src/render.cpp
        src/render.hpp)

set_target_properties(pollock PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(pollock PRIVATE -pedantic -Wall -Wextra)
target_link_options(pollock PRIVATE -lm -lpthread -lpthread -lSDL2)

