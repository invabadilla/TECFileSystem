cmake_minimum_required(VERSION 3.17)
project(ControllerNode)

set(CMAKE_CXX_STANDARD 17)

add_executable(ControllerNode main.cpp Divider.h Client.cpp Client.h tinyxml2.cpp tinyxml2.h Huffman.h)