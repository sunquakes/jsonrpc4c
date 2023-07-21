#ifndef JSONRPC4C_TCP_SERVER_H
#define JSONRPC4C_TCP_SERVER_H

#include <iostream>
#include <unordered_set>

#include "server.h"
#include "properties.h"

namespace jsonrpc4c{
    class TcpServer : public Server {
    private:
        TcpOption option_;

        std::atomic<bool> isClosed_;

        std::mutex mtx_ = std::mutex();

        std::unordered_set<int> connectedClientSet_;

        std::vector<std::thread> clientThreads_;

        void Handler(int clientSocket);

    public:
        TcpServer(int port);

        ~TcpServer();

        void Start();

        void SetOption(TcpOption option) {
            this->option_ = option;
        };
    };
}

#endif // JSONRPC4C_TCP_SERVER_H
