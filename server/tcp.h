#ifndef JSONRPC4C_TCP_H
#define JSONRPC4C_TCP_H

#include <iostream>

struct Option {
    std::string packageEof;
    int packageMaxLength;
};

namespace jsonrpc4c {
    class TcpServer {
    private:
        int serverSocket;

        int port;

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

#endif // JSONRPC4C_TCP_H
