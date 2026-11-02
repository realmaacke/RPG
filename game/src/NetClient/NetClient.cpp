#include "NetClient.hpp"
#include "enet/enet.h"
#include "src/Output/Output.hpp"

int NetClient::init() {
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

    this->connection = enet_host_connect(
        this->client,
        &this->address,
        this->MAX_CHANNELS,
        0
    );

    if (this->connection == nullptr) {
        Output::error("No peers available for NetClient");
        return 1;
    }

    return 0;
}

void NetClient::connect() {
    ENetEvent event;

    if (enet_host_service(this->client, &event, this->TIMEOUT_IN_MS)> 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT) {
        // connected
        return;
    }
    Output::error("Could not connect to server");
    enet_peer_reset(this->connection);
}

void NetClient::disconnect() {
    Output::error("Disconnected from server");
    enet_peer_reset(this->connection);
}