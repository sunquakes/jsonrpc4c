#ifndef JSONRPC4C_SERVER_H
#define JSONRPC4C_SERVER_H

namespace jsonrpc4c {
    class Server {
    protected:
        int serverSocket;

        int port;

        void handler(int clientSocket);

    public:
        void start();
    };
};

#endif //JSONRPC4C_SERVER_H
