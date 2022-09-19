cmake_minimum_required(VERSION 3.21)
project(Minesweeper)

set(CMAKE_CXX_STANDARD 11)
set(SFML_DIR "<sfml root prefix>/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_executable(Minesweeper main.cpp Board.cpp Board.h Tile.cpp Tile.h TextureManager.cpp TextureManager.h Random.cpp Random.h)
target_link_libraries(Minesweeper sfml-graphics sfml-audio)
