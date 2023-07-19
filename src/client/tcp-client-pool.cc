#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sstream>
#include <string>
#include <vector>

#include "jsonrpc4c/tcp-client-pool.h"
#include "jsonrpc4c/properties.h"
#include "jsonrpc4c/string.h"

using namespace jsonrpc4c;

const int DEFAULT_PORT = 80;

/**
 * Initialize tcp client pool through an address string;
 * @param address: The format is 'ip:port', if there are multiple servers, use ',' to separate them.
 */
TcpClientPool::TcpClientPool(std::string address) {
    std::vector<std::string> addressVector = split(address, ',');
    for (const auto &addr: addressVector) {
        std::vector<std::string> addrVector = split(addr, ',');
        std::string ip;
        int port;
        ip = addrVector[0];
        if (addrVector.size() > 1) {
            port = std::stoi(addrVector[1]);
        } else {
            port = DEFAULT_PORT;
        }
        addressVector_.push_back(TcpAddress{ip, port});
    }

    Init();
}

/**
 * Initialize tcp client pool through an address string with PoolOption;
 * @param address: The format is 'ip:port', if there are multiple servers, use ',' to separate them.
 * @param option
 */
TcpClientPool::TcpClientPool(std::string address, PoolOption option) : TcpClientPool(address) {
    SetOption(option);
}

/**
 * Initialize tcp client pool through a ip and a port, it can only communicate with one server;
 * @param ip
 * @param port
 */
TcpClientPool::TcpClientPool(std::string ip, int port) {
    // Server address and port
    std::string serverAddress = ip; // Replace with your server's IP address
    int serverPort = port; // Replace with your server's port
    addressVector_.push_back(TcpAddress{ip, serverPort});

    Init();
}

/**
 * Initialize tcp client pool through a ip and a port with PoolOption, it can only communicate with one server;
 * @param ip
 * @param port
 * @param option
 */
TcpClientPool::TcpClientPool(std::string ip, int port, PoolOption option) : TcpClientPool(ip, port) {
    SetOption(option);
}

/**
 * Init the tcp connection pool.
 */
void TcpClientPool::Init() {
    for (int i = 0; i < option_.minIdle; i++) {
        int conn = Create();
        activeTotal_++;
        conns_.push(conn);
    }
}

/**
 * Set the pool option.
 * @param option
 */
void TcpClientPool::SetOption(PoolOption option) {
    this->option_ = option;
}

/**
 * Set the active address.
 * @return
 */
int TcpClientPool::ActiveAddress() {
    std::vector<TcpAddress> activeAddressVector;
    if (addressVector_.size() > 0) {
        this->activeAddressVector_ = addressVector_;
    }
    return activeAddressVector_.size();
}

/**
 * Determine which server to connect to.
 * @return
 */
int TcpClientPool::Create() {
    int size = activeAddressVector_.size();
    if (size == 0) {
        size = ActiveAddress();
    }
    int key = activeTotal_ % size;
    TcpAddress address = activeAddressVector_[key];

    std::vector<TcpAddress> activeAddressVector = activeAddressVector_;
    // Create a socket
    int clientSocket = Connect(address);
    return clientSocket;
}

/**
 * Connect to the server.
 * @param tcpAddress
 * @return
 */
int TcpClientPool::Connect(TcpAddress tcpAddress) {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
    }

    // Set up the server address structure
    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(tcpAddress.port);
    if (inet_pton(AF_INET, tcpAddress.ip.c_str(), &server.sin_addr) <= 0) {
        std::cerr << "Invalid server address." << std::endl;
    }

    // Connect to the server
    if (connect(clientSocket, reinterpret_cast<sockaddr *>(&server), sizeof(server)) < 0) {
        std::cerr << "Failed to connect to the server." << std::endl;
    }
    return clientSocket;
}

/**
 * Borrow a connection from connection queue.
 * @return
 */
int TcpClientPool::Borrow() {
    mtx_.lock();
    activeTotal_--;
    int clientSocket = conns_.front();
    mtx_.unlock();
    return clientSocket;
}

/**
 * Release the connection to the connection queue.
 * @param client
 */
void TcpClientPool::Release(int clientSocket) {
    mtx_.lock();
    activeTotal_++;
    conns_.push(clientSocket);
    mtx_.unlock();
}

/**
 * Remove the connection.
 * @param client
 */
void TcpClientPool::Remove(int clientSocket) {
    mtx_.lock();
    activeTotal_--;
    mtx_.unlock();
}
