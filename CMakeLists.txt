cmake_minimum_required(VERSION 3.25)
project(jsonrpc4c)

set(CMAKE_CXX_STANDARD 17)

add_library(jsonrpc4c include/jsonrpc4c/tcp-client-pool.h src/client/tcp-client-pool.cc include/jsonrpc4c/discovery.h include/jsonrpc4c/string.h)

target_sources(jsonrpc4c PUBLIC
        include/jsonrpc4c/properties.h
        include/jsonrpc4c/server.h
        include/jsonrpc4c/client.h
        include/jsonrpc4c/tcp-server.h
        include/jsonrpc4c/tcp-client.h
        src/server/tcp-server.cc
        src/client/tcp-client.cc
        )

set(jsonrpc4c_build_include_dirs
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${jsonrpc4c_build_include_dirs})

add_subdirectory(test)
