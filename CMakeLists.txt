cmake_minimum_required(VERSION 3.14)
project(antwars)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)


find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(antwars
        src/main.cpp
        src/CAntQueen.cpp src/CAntQueen.hpp src/AAnt.cpp src/AAnt.hpp src/CAnt.cpp src/CAnt.hpp src/CGameState.cpp src/CGameState.hpp src/CMap.cpp src/CMap.hpp src/CAntHill.cpp src/CAntHill.hpp src/CMenu.cpp src/CMenu.hpp src/AMapObject.cpp src/AMapObject.hpp src/CEmptyTile.cpp src/CEmptyTile.hpp src/CObstacle.cpp src/CObstacle.hpp src/Difficulty.hpp)

set(CMAKE_CXX_FLAGS "-lcurses")
target_link_libraries(antwars ${CURSES_LIBRARIES})
