#ifndef JSONRPC4C_TCP_CLIENT_POOL_H
#define JSONRPC4C_TCP_CLIENT_POOL_H

#include <iostream>
#include <queue>
#include <vector>
#include <mutex>

#include "properties.h"
#include "discovery.h"

namespace jsonrpc4c {
    class TcpClientPool {
    private:
        std::queue<int> conns_;

        std::vector<TcpAddress> addressVector_;

        std::vector<TcpAddress> activeAddressVector_;

        PoolOption option_ = PoolOption{5, 10};

        int activeTotal_ = 0;

        std::mutex mtx_;

        void SetOption(PoolOption option);

        int Create();

        int ActiveAddress();

        int Connect(TcpAddress tcpAddress);

        void Init();

    public:
        TcpClientPool(std::string address);

        TcpClientPool(std::string address, PoolOption option);

        TcpClientPool(std::string ip, int port);

        TcpClientPool(std::string ip, int port, PoolOption option);

        // TcpClientPool(Discovery discovery);

        // ~TcpClientPool();

        int Borrow();

        void Release(int clientSocket);

        void Remove(int clientSocket);

        // int BorrowAfterRemove(int conn);

        // int Remove(int conn);

    };
};

#endif //JSONRPC4C_TCP_CLIENT_POOL_H
