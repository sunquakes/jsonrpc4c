#ifndef JSONRPC4C_TCP_CLIENT_H
#define JSONRPC4C_TCP_CLIENT_H

#include <unistd.h>
#include <sys/socket.h>
#include <typeinfo>

#include "client.h"
#include "properties.h"
#include "tcp-client-pool.h"

namespace jsonrpc4c {
    class TcpClient : public Client {
    private:
        TcpOption option_;

        TcpClientPool *pool_;

    public:
        TcpClient(std::string ip, int port);

        ~TcpClient();

        template<typename T>
        void Call(std::string method, T *params, T *result);

        void setOption(TcpOption option) {
            this->option_ = option;
        };
    };


    /**
     * Call the remote service to get the result.
     * @param method
     * @param params
     * @param result
     */
    template<typename T>
    void TcpClient::Call(std::string method, T *params, T *result) {
        std::size_t resultSize = sizeof(T);
        std::memcpy(result, params, resultSize);
        std::cout << "Received from server: " << typeid(params).name() << std::endl;
        // Create a socket
        int clientSocket = pool_->Borrow();

        // Send data to the server
        std::string message = "Hello, server!\r\n";
        if (send(clientSocket, message.c_str(), message.size(), 0) < 0) {
            std::cerr << "Failed to send data." << std::endl;
        }
        std::cerr << "Send data." << message;

        pool_->Release(clientSocket);
    }
};

#endif //JSONRPC4C_TCP_CLIENT_H
