cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(logger_test main.cpp Logger.h Logger.cpp WriteLock.h WriteLock.cpp)

