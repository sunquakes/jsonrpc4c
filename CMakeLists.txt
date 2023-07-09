cmake_minimum_required(VERSION 3.25)
project(jsonrpc4c)

set(CMAKE_CXX_STANDARD 17)

add_executable(jsonrpc4c_e src/main.cc src/server/tcp-server.cc)

add_library(jsonrpc4c src/main.cc src/main.h src/server/tcp-server.cc src/server/tcp-server.h src/server/server.h src/client/tcp-client.cc src/client/tcp-client.h)

add_subdirectory(test)