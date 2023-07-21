#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <thread>
#include <vector>
#include <memory>

#include "jsonrpc4c/tcp-server.h"

using namespace jsonrpc4c;

TcpServer::TcpServer(int port) {
    this->port = port;
    this->option_ = TcpOption{"\r\n", 1024 * 1024 * 2};
}

TcpServer::~TcpServer() {
    // Close the server socket
    isClosed_ = true;

    // Signal all client handlers to stop gracefully
    {
        std::lock_guard<std::mutex> lock(mtx_);
        for (int clientSocket: connectedClientSet_) {
            shutdown(clientSocket, SHUT_RDWR); // Send a shutdown signal to the client
        }
    }

    // Wait for all client threads to finish and join them
    for (std::thread &clientThread : clientThreads_) {
        if (clientThread.joinable()) {
            clientThread.join();
        }
    }

    // Close the server socket
    {
        std::lock_guard<std::mutex> lock(mtx_);
        for (int clientSocket: connectedClientSet_) {
            close(clientSocket);
        }
        connectedClientSet_.clear();
    }

    close(serverSocket);

    std::cout << "close server0" << std::endl;
    std::cout << "close server1" << std::endl;
}

void TcpServer::Start() {
    // Create the server socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Set SO_REUSEADDR option to allow reusing the local address (port)
    int optval = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    this->serverSocket = serverSocket;

    isClosed_ = false;

    // Bind the server socket to a specific address and port
    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Accept connections on any available network interface
    serverAddress.sin_port = htons(port);

    bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));

    // Listen for incoming connections
    listen(serverSocket, 100); // Maximum 5 pending connections

    while (!isClosed_) {
        // Accept client connections
        struct sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);

        std::cout << "Waiting accept client" << std::endl;
        int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddressLength);
        if (isClosed_) {
            break;
        }
        // Spawn a new thread to handle the client communication
        std::thread clientThread(&TcpServer::Handler, this, clientSocket);
        clientThreads_.emplace_back(std::move(clientThread));
        // clientThreads_.emplace_back(&TcpServer::Handler, this, clientSocket);

        // std::thread clientThread(&TcpServer::Handler, this, clientSocket);
        // clientThread.detach();
    }
}

/**
 * Handler the client socket.
 * @param clientSocket
 */
void TcpServer::Handler(int clientSocket) {
    std::atomic<bool> isClientClosed;
    std::cout << "The client connected" << std::endl;
    std::cout << std::to_string(isClosed_) << std::endl;

    char buf[4096];
    std::vector<char> eofb;
    eofb.assign(option_.packageEof.begin(), option_.packageEof.end());
    int eofl = eofb.size();

    {
        std::lock_guard<std::mutex> lock(mtx_);
        // Insert the client to the set.
        connectedClientSet_.insert(clientSocket);
        std::cout << "======" << std::endl;
    }

    while (!isClosed_ && !isClientClosed) {
        std::vector<char> data;
        while (true) {
            // clear buffer
            memset(buf, 0, 4096);

            // wait for a message
            int bytesRecv = recv(clientSocket, buf, option_.packageMaxLength, 0);
            if (bytesRecv == -1) {
                std::cerr << "There was a connection issue." << std::endl;
                break;
            }
            if (bytesRecv == 0) {
                std::cout << "The client disconnected" << std::endl;
                isClientClosed = true;
                break;
            }
            // Check if it's the shutdown signal
//            if (bytesRecv == shutdownSignal.size() && memcmp(buf, shutdownSignal.data(), bytesRecv) == 0) {
//                std::cout << "Received a shutdown signal. Closing the client gracefully." << std::endl;
//                isClientClosed = true;
//                break;
//            }

            std::copy(buf, buf + bytesRecv, std::back_inserter(data));
            std::vector<char> last(data.end() - eofl, data.end());
            if (last == eofb) {
                break;
            }
        }
        std::string reqData(data.begin(), data.end());
        std::cout << "Received: " << reqData << std::endl;
    }

    {
        std::lock_guard<std::mutex> lock(mtx_);
        // close socket
        close(clientSocket);
        // Erase the client from the set.
        connectedClientSet_.erase(clientSocket);
    }
    std::cout << "------" << std::endl;
}