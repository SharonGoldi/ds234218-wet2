cmake_minimum_required(VERSION 3.15)
project(wet2)

set(CMAKE_CXX_STANDARD 11)

add_executable(wet2 main.cpp ServerHashTable.cpp ServerHashTable.h Server.cpp Server.h DataCenter.cpp DataCenter.h AVLRankTree.h AVLRankTree.h)