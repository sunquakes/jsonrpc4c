#include "tcp.h"
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <thread>
#include <vector>

using namespace jsonrpc4c;

TcpServer::TcpServer(int port) {
    this->port = port;
    this->option = Option{"\r\n", 1024 * 1024 * 2};
}

TcpServer::~TcpServer() {
    // Close the server socket
    close(serverSocket);
}

void TcpServer::start() {
    // Create the server socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    this->serverSocket = serverSocket;

    // Bind the server socket to a specific address and port
    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Accept connections on any available network interface
    serverAddress.sin_port = htons(port);

    bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));

    // Listen for incoming connections
    listen(serverSocket, 5); // Maximum 5 pending connections

    while (true) {
        // Accept client connections
        struct sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);

        std::cout << "Waiting accept client" << std::endl;
        int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddressLength);

        // Spawn a new thread to handle the client communication
        std::thread clientThread(&TcpServer::handler, this, clientSocket);
        clientThread.detach();
    }
}

void TcpServer::handler(int clientSocket) {
    bool isClose = false;
    std::cout << "The client connected" << std::endl;

    char buf[4096];
    std::vector<char> eofb;
    eofb.assign(option.packageEof.begin(), option.packageEof.end());
    int eofl = eofb.size();
    outer:
    while (!isClose) {
        std::vector<char> data;
        while (true) {
            // clear buffer
            memset(buf, 0, 4096);

            // wait for a message
            int bytesRecv = recv(clientSocket, buf, option.packageMaxLength, 0);
            if (bytesRecv == -1) {
                std::cerr << "There was a connection issue." << std::endl;
            }
            if (bytesRecv == 0) {
                std::cout << "The client disconnected" << std::endl;
                isClose = true;
                break;
            }
            std::copy(buf, buf + bytesRecv, std::back_inserter(data));
            std::vector<char> last(data.end() - eofl, data.end());
            if (last == eofb) {
                break;
            }
        }
        std::string reqData(data.begin(), data.end());
        std::cout << "Received: " << reqData;
    }
    // close socket
    close(clientSocket);
}