add_subdirectory(googletest)

add_executable(jsonrpc4c_test)
target_sources(jsonrpc4c_test PUBLIC
        tcp_test.cc
        boost_test.cc
        )

target_link_libraries(jsonrpc4c_test gtest gtest_main jsonrpc4c)
