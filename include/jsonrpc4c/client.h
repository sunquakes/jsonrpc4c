#ifndef JSONRPC4C_CLIENT_H
#define JSONRPC4C_CLIENT_H

#include <iostream>

namespace jsonrpc4c {
    class Client {
    protected:
        std::string ip;

        int port;

    };
};

#endif //JSONRPC4C_CLIENT_H
