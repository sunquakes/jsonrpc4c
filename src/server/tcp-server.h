#ifndef JSONRPC4C_TCP_SERVER_H
#define JSONRPC4C_TCP_SERVER_H

#include <iostream>
#include "server.h"

struct Option {
    std::string packageEof;
    int packageMaxLength;
};

namespace jsonrpc4c{
    class TcpServer : public Server {
    private:
        Option option;

        void handler(int clientSocket);

    public:
        TcpServer(int port);

        ~TcpServer();

        void start();

        void setOption(Option option) {
            this->option = option;
        };
    };
}

#endif // JSONRPC4C_TCP_SERVER_H
