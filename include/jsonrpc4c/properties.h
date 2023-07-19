#ifndef JSONRPC4C_PROPERTIES_H
#define JSONRPC4C_PROPERTIES_H

#include <iostream>

namespace jsonrpc4c {
    struct TcpOption {
        std::string packageEof;
        int packageMaxLength;
    };

    struct TcpAddress {
        std::string ip;
        int port;
    };

    struct PoolOption {
        int minIdle;
        int maxActive;
    };
};

#endif //JSONRPC4C_PROPERTIES_H
