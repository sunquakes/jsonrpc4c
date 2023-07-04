#ifndef JSONRPC4C_SERVER_H
#define JSONRPC4C_SERVER_H

namespace jsonrpc4c {
    class Server {
    private:

    public:
        Server(int port);

        ~Server();

        int port;

        void start();

    };
}

#endif // JSONRPC4C_SERVER_H
