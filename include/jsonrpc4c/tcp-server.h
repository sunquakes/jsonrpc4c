#ifndef JSONRPC4C_TCP_SERVER_H
#define JSONRPC4C_TCP_SERVER_H

#include <iostream>
#include "server.h"
#include "properties.h"

namespace jsonrpc4c{
    class TcpServer : public Server {
    private:
        TcpOption option;

        void Handler(int clientSocket);

        std::atomic<bool> isClosed;

        int socketPair[2];

    public:
        TcpServer(int port);

        ~TcpServer();

        void Start();

        void SetOption(TcpOption option) {
            this->option = option;
        };
    };
}

#endif // JSONRPC4C_TCP_SERVER_H
