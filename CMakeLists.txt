cmake_minimum_required(VERSION 3.25)
project(jsonrpc4c)

set(CMAKE_CXX_STANDARD 17)

add_executable(jsonrpc4c_e main.cpp server/tcp.cpp)

add_library(jsonrpc4c main.cpp main.h server/tcp.cpp server/tcp.h)
