#include "server.h"

int main()
{
    jsonrpc4c::Server server(9000);
    server.start();
    return 0;
}