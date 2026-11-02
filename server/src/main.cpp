#include "src/Server/Server.hpp"
#include <cstdlib>

int main(int, char**) {
    Server server;

    if (!server.init()) {
        server.cleanup();
        return 1;
    }

    server.runServer();
    return 0;
}