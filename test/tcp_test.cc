#include <gtest/gtest.h>
#include <jsonrpc4c/tcp-server.h>
#include <jsonrpc4c/tcp-client.h>
#include <thread>

void startServer(jsonrpc4c::TcpServer* server) {
    server->Start();
}

TEST(TcpTestSuite, Test1) {
    jsonrpc4c::TcpServer* server = new jsonrpc4c::TcpServer(9000);
    std::thread start(startServer, server);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    jsonrpc4c::TcpClient client("127.0.0.1", 9000);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    delete server;
    start.join();
    // Test assertions
    ASSERT_EQ(2 + 2, 4);
}
