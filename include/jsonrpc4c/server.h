#ifndef JSONRPC4C_SERVER_H
#define JSONRPC4C_SERVER_H

namespace jsonrpc4c {
    class Server {
    protected:
        int serverSocket;

        int port;

        void Handler(int clientSocket);

    public:
        void Start();
    };
};

#endif //JSONRPC4C_SERVER_H
