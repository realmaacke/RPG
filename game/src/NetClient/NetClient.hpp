#pragma once


#include "enet/enet.h"
#include "enet/types.h"
#include <cstddef>
#include <string>
class NetClient {
public:
    int init();
    void connect();
    void disconnect();

private:
    ENetHost* client;
    ENetAddress address;

    ENetPeer* connection;

    std::string hostName;

    size_t MAX_CONNECTIONS = 1;
    size_t MAX_CHANNELS = 2;
    
    enet_uint32 INC_BANDWITH = 0;
    enet_uint32 out_BANDWITH = 0;

    size_t TIMEOUT_IN_MS = 5000;
};