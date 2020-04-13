cmake_minimum_required(VERSION 3.5)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project(Tetris)

# Define preprocessor symbol (for 'mvaddwstr')
add_definitions(-D_XOPEN_SOURCE_EXTENDED)

# Ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# add the executable
add_executable(Tetris src/Tetris.cpp)

# Link Ncurses library
target_link_libraries(Tetris ${CURSES_LIBRARIES})

