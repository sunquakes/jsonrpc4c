#include "jsonrpc4c/tcp-server.h"

int main()
{
    jsonrpc4c::TcpServer server(9002);
    server.Start();
    return 0;
}