#include "jsonrpc4c/tcp-client.h"
#include "jsonrpc4c/tcp-client-pool.h"

using namespace jsonrpc4c;

TcpClient::TcpClient(std::string ip, int port) {
    pool_ = new TcpClientPool(ip, port);

    // Receive response from the server
    // char buffer[4096];
    // memset(buffer, 0, sizeof(buffer));
    // if (recv(clientSocket, buffer, sizeof(buffer) - 1, 0) < 0) {
    //     std::cerr << "Failed to receive data." << std::endl;
    // }
    //
    // std::cout << "Received from server: " << buffer << std::endl;

    // Close the socket
}

TcpClient::~TcpClient() {

}
