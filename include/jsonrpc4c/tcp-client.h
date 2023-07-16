#ifndef JSONRPC4C_TCP_CLIENT_H
#define JSONRPC4C_TCP_CLIENT_H

#include "client.h"
#include "properties.h"

namespace jsonrpc4c {
    class TcpClient : public Client {
    private:
        TcpOption option;

    public:
        TcpClient(std::string ip, int port);

        ~TcpClient();

        void setOption(TcpOption option) {
            this->option = option;
        };
    };
};

#endif //JSONRPC4C_TCP_CLIENT_H
