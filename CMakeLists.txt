cmake_minimum_required(VERSION 3.20)
project(sdl)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-std=c++17 -g -Wall -Wextra)
find_package(SDL2 REQUIRED)
# find_library(SDL2_LIBRARIES NAMES libSDL2 libSDL2.so libSDL2.a libSDL2.dylib)

add_executable(main main.cpp graphics.cpp player.cpp world.cpp)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SDL2_INCLUDES})
# target_link_libraries(main PUBLIC ${SDL2_LIBRARIES})
target_link_libraries(main PUBLIC SDL2::SDL2)