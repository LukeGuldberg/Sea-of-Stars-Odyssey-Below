cmake_minimum_required(VERSION 3.20)
project(sdl)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-std=c++17 -g -Wall -Wextra)

set(SDL2_INCLUDES "C:/Users/lukeg/code/GameDev/SDL-SDL2/SDL-SDL2/include")
set(SDL2_LIBRARIES "C:/Users/lukeg/code/GameDev/SDL-SDL2/SDL2-2.26.2-win32-x64/SDL2.dll")

add_library(gamelib
  graphics.cpp
  player.cpp
  world.cpp
)
add_executable(main main.cpp)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(main PUBLIC gamelib ${SDL2_LIBRARIES})