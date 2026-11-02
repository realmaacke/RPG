#pragma once
#include "enet/enet.h"
#include "enet/types.h"
#include <cstddef>
#include <string>

#include "src/Output/Output.hpp"

class NetClient {
public:
    int init();
    void update();


    void sendPacket(const std::string& message);

    template <typename T>
    void sendStruct(const T& packetData) {
        if (this->connection == nullptr) {
            Output::error("Cannot send packet: Not connected");
            return;
        }

        ENetPacket* packet = enet_packet_create(
            &packetData,           // Pointer to raw struct memory
            sizeof(T),             // Size of struct in bytes
            ENET_PACKET_FLAG_RELIABLE
        );

        enet_peer_send(this->connection, 0, packet);
        enet_host_flush(this->client);
    };
    
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