#ifndef JSONRPC4C_DISCOVERY_H
#define JSONRPC4C_DISCOVERY_H

#include <iostream>

namespace jsonrpc4c {
    class Discovery {
    private:

    public:
        Discovery(std::string url);

        Discovery(std::string ip, int port);

        ~Discovery();

    };
};

#endif //JSONRPC4C_DISCOVERY_H
