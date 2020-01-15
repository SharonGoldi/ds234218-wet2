cmake_minimum_required(VERSION 3.15)
project(wet2)

set(CMAKE_CXX_STANDARD 11)

add_executable(wet2 main.cpp library2.cpp ServerHashTable.cpp ServerHashTable.h Server.cpp Server.h DataCenter.cpp DataCenter.h AVLRankTree.h AVLRankTree.h DCManager.cpp DCManager.h UnionFind.cpp)