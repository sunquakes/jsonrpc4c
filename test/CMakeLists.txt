add_subdirectory(googletest)

add_executable(jsonrpc4c_test tcpserver_test.cc)

target_link_libraries(jsonrpc4c_test gtest gtest_main)