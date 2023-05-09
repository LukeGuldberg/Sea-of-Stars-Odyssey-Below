
cmake_minimum_required(VERSION 3.20)
project(sdl)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-std=c++17 -g -Wall -Wextra)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_library(gamelib
  graphics.cpp
  player.cpp
  world.cpp
  vec.cpp
  camera.cpp
  tilemap.cpp
  physics.cpp
  fsm.cpp
  command.cpp
  engine.cpp
  settings.cpp
  sprite.cpp
  audio.cpp
  animatedsprite.cpp
  randomness.cpp
  level.cpp
  object.cpp
  enemy.cpp
  enemytype.cpp
  combat.cpp
  quadtree.cpp
  projectile.cpp
  objecttype.cpp
  animatedobject.cpp
  loadscreen.cpp
  
) 
target_include_directories(gamelib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})
target_link_libraries(gamelib PUBLIC SDL2::SDL2 SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC gamelib)

# add_executable(quadtree_test quadtree_test.cpp)
# target_link_libraries(quadtree_test PUBLIC gamelib)

# add_executable(perf_quadtree perf_quadtree.cpp)
# target_link_libraries(perf_quadtree PUBLIC gamelib)

# add_executable(animate_quadtree animate_quadtree.cpp)
# target_link_libraries(animate_quadtree PUBLIC gamelib)