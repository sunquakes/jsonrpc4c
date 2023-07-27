#ifndef JSONRPC4C_SERVER_H
#define JSONRPC4C_SERVER_H

#include <boost/describe.hpp>
#include <boost/json.hpp>

#define JSONRPC4C_REGISTER(C, Members) \
    BOOST_DESCRIBE_STRUCT(C, (), Members)

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
        using namespace boost::json;
        value jv = parse( "[1, 2, 3]" );
    }
};

#endif //JSONRPC4C_SERVER_H
