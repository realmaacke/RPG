#include "NetClient.hpp"
#include "enet/enet.h"
#include "src/Output/Output.hpp"

int NetClient::init() {
    if (enet_initialize() != 0) {
        Output::error("Could not initialize ENet");
        return 1;
    }

    this->client = enet_host_create(
        nullptr,
        this->MAX_CONNECTIONS,
        this->MAX_CHANNELS,
        this->INC_BANDWITH,
        this->out_BANDWITH
    );

    if (this->client == nullptr) {
        Output::error("Could not create NetClient");
        return 1;
    }

    enet_address_set_host(&this->address, "127.0.0.1");
    this->address.port = 7777;

    return 0; // Host created successfully; connect() will initiate the connection
}

void NetClient::connect() {
    if (this->client == nullptr) return;

    // Create peer connection request
    this->connection = enet_host_connect(
        this->client,
        &this->address,
        this->MAX_CHANNELS,
        0
    );

    if (this->connection == nullptr) {
        Output::error("No peers available on client host");
        return;
    }

    ENetEvent event;
    // Service network to complete handshake
    if (enet_host_service(this->client, &event, this->TIMEOUT_IN_MS) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT) {

        this->sendPacket("Test");
        return;
    }

    Output::error("Could not connect to server");
    enet_peer_reset(this->connection);
    this->connection = nullptr;
}

void NetClient::disconnect() {
    if (this->connection != nullptr) {
        Output::error("Disconnected from server");
        enet_peer_reset(this->connection);
        this->connection = nullptr;
    }
}

void NetClient::update() {
    if (this->client == nullptr) return;

    ENetEvent event;
    while (enet_host_service(this->client, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                Output::print("Received packet from server");
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                Output::error("Server disconnected client");
                this->connection = nullptr;
                break;

            default:
                break;
        }
    }
}

void NetClient::sendPacket(const std::string& message) {
    if (this->connection == nullptr) {
        Output::error("Cannot send packet: Not connected");
        return;
    }

    ENetPacket* packet = enet_packet_create(
        message.c_str(),
        message.length() + 1,
        ENET_PACKET_FLAG_RELIABLE
    );

    enet_peer_send(this->connection, 0, packet);
    enet_host_flush(this->client);
}