#include "jsonrpc4c/tcp-client.h"
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace jsonrpc4c;

TcpClient::TcpClient(std::string ip, int port) {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
    }

    // Server address and port
    std::string serverAddress = ip;  // Replace with your server's IP address
    int serverPort = port;                   // Replace with your server's port

    // Set up the server address structure
    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(serverPort);
    if (inet_pton(AF_INET, serverAddress.c_str(), &server.sin_addr) <= 0) {
        std::cerr << "Invalid server address." << std::endl;
    }

    // Connect to the server
    if (connect(clientSocket, reinterpret_cast<sockaddr *>(&server), sizeof(server)) < 0) {
        std::cerr << "Failed to connect to the server." << std::endl;
    }

    // Send data to the server
    std::string message = "Hello, server!\r\n";
    if (send(clientSocket, message.c_str(), message.size(), 0) < 0) {
        std::cerr << "Failed to send data." << std::endl;
    }
    std::cerr << "Send data." << message;

    // Receive response from the server
    // char buffer[4096];
    // memset(buffer, 0, sizeof(buffer));
    // if (recv(clientSocket, buffer, sizeof(buffer) - 1, 0) < 0) {
    //     std::cerr << "Failed to receive data." << std::endl;
    // }
    //
    // std::cout << "Received from server: " << buffer << std::endl;

    // Close the socket
    close(clientSocket);
}

TcpClient::~TcpClient() {
}
