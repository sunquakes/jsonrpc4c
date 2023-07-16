#include "jsonrpc4c/tcp-server.h"
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
    this->option = TcpOption{"\r\n", 1024 * 1024 * 2};
}

TcpServer::~TcpServer() {
    // Close the server socket
    char buf[1] = {0};
    write(socketPair[0], buf, sizeof(buf));
    close(socketPair[0]);
    close(socketPair[1]);
    isClosed = true;
    close(serverSocket);
}

void TcpServer::Start() {
    // Create the server socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    this->serverSocket = serverSocket;

    isClosed = false;

    // Bind the server socket to a specific address and port
    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Accept connections on any available network interface
    serverAddress.sin_port = htons(port);

    bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));

    // Listen for incoming connections
    listen(serverSocket, 5); // Maximum 5 pending connections

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, socketPair) == -1) {
        // TODO: throw exception
    }

    while (!isClosed) {
        // Accept client connections
        struct sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);

        std::cout << "Waiting accept client" << std::endl;
        int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddressLength);

        // Spawn a new thread to handle the client communication
        std::thread clientThread(&TcpServer::Handler, this, clientSocket);
        clientThread.detach();
    }
}

void TcpServer::Handler(int clientSocket) {
    std::atomic<bool> isClientClosed;
    std::cout << "The client connected" << std::endl;

    char buf[4096];
    std::vector<char> eofb;
    eofb.assign(option.packageEof.begin(), option.packageEof.end());
    int eofl = eofb.size();

    while (!isClosed && !isClientClosed) {
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
                isClientClosed = true;
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