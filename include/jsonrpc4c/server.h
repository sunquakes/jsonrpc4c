#ifndef JSONRPC4C_SERVER_H
#define JSONRPC4C_SERVER_H

#include <iostream>

namespace jsonrpc4c {
    class Server {
    protected:
        int serverSocket;

        int port;

        void Handler(int clientSocket);

    public:
        void Start();

        inline void Handler(std::string body);
    };

    void Server::Handler(std::string body) {

    }
};

#endif //JSONRPC4C_SERVER_H
