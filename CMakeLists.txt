cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(DeepRemove)
 
set(CMAKE_MODULE_PATH "CMake;${CMAKE_MODULE_PATH}")
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
 
find_package(Nuke REQUIRED)
 
add_nuke_plugin(DeepRemove src/DeepRemove.cpp)