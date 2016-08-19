cmake_minimum_required(VERSION 3.6)
project(untitled)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
set(SOURCE_FILES main.c)
add_executable(untitled ${SOURCE_FILES})

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

INCLUDE_DIRECTORIES($(SDL2_INCLUDE_DIRS))

target_link_libraries(untitled ${SDL2_LIBRARIES})