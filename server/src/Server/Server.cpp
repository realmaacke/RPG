#include "Server.hpp"
#include "src/Data/User.hpp"
#include "src/Output/Output.hpp" 
#include <cstdlib>
#include <enet/enet.h>
#include <string>
#include <vector>

bool Server::init() {
    if (enet_initialize() != 0){
        Output::error("Could not initialize ENet");
        return false;
    }

    this->address.host = ENET_HOST_ANY;
    this->address.port = 7777;

    this->serv = enet_host_create(
        &this->address,
        this->MAX_CLIENTS,
        this->MAX_CHANNELS,
        this->INC_BADNWIDTH,
        this->OUT_BANDWIDTH
    );

    if (this->serv == NULL) {
        Output::error("Could not create ENet server");
        return false;
    }
    Output::print("Server started");
    return true;
}

void Server::runServer() {
    ENetEvent event;

    while (this->getState()) {
        int status = enet_host_service(this->serv, &event, this->EVENT_TIMEOUT);

        if (status > 0) {
            this->onEvent(event);

            while (enet_host_check_events(this->serv, &event) > 0) {
                this->onEvent(event);
            }
        } else if (status < 0) {
            Output::error("ENet error within the loop");
        }
    }
    this->cleanup();
}

void Server::onEvent(ENetEvent& event) {
    // Process the event directly — DO NOT call enet_host_service here
    switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT: {
            Output::print(
                "Client connected: " + std::to_string(event.peer->address.host)
            );
            break;
        }
        case ENET_EVENT_TYPE_DISCONNECT: {
            Output::print("Disconnected user");
            event.peer->data = NULL;
            break;
        }
        case ENET_EVENT_TYPE_RECEIVE: {
            printf("A packet of length %zu containing '%s' was received on channel %u.\n",
                event.packet->dataLength,
                reinterpret_cast<char*>(event.packet->data),
                event.channelID);

            enet_packet_destroy(event.packet);
            break; // Added missing break
        }
        case ENET_EVENT_TYPE_NONE: {
            break;
        }
    }
}

bool Server::getState() { return this->state; }
void Server::setState(bool state) { this->state = state; }

void Server::cleanup() {
    if (this->serv != nullptr) {
        enet_host_destroy(this->serv);
        this->serv = nullptr;
    }
    enet_deinitialize(); // Deinit AFTER host is destroyed
}