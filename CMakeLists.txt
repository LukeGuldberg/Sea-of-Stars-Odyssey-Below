
cmake_minimum_required(VERSION 3.20)
project(sdl)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-std=c++17 -g -Wall -Wextra)

find_package(SDL2 REQUIRED)

add_library(gamelib
  graphics.cpp
  player.cpp
  world.cpp
  vec.cpp
  camera.cpp
  tilemap.cpp
)
target_include_directories(gamelib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SDL2_INCLUDES})
target_link_libraries(gamelib PUBLIC SDL2::SDL2)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC gamelib)