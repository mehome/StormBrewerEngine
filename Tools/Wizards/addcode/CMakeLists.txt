cmake_minimum_required(VERSION 3.1.0)

include_directories(. ../../. ../.././External ../)
set(CMAKE_CXX_STANDARD 17)

add_library(addcodelib STATIC main.cpp)

