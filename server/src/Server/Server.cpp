#include <cstdlib>
#include <enet/enet.h>
#include <string>
#include <vector>
#include "Server.hpp"
#include "enet/types.h"
#include "src/Data/User.hpp"
#include "src/Output/Output.hpp"

bool Server::init() {
    if (enet_initialize() != 0){
        Output::error("Could not initialize ENet");
        return false;
    }

    this->address.host = ENET_HOST_ANY;
    this->address.port = 7777; // change

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
    return true;
}

std::vector<UserData::UserConnect> dummyDB = {
    {"marcus", "password"}
};

std::vector<UserData::User> account = {
    {
        "marcus", 
        std::vector<UserData::Characters>{ 
            {
                "darkmark",
                10,
            },
            {
                "lightbringer",
                10
            }
        }
    }
};


void Server::onEvent() {
    ENetEvent event;

    while (enet_host_service(this->serv, &event, this->EVENT_TIMEOUT) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                Output::print(
                    "Client connected: " + std::to_string(event.peer->address.host)
                );

                UserData::UserConnect* connectionData
                    = static_cast<UserData::UserConnect*>(event.peer->data);
                bool isConnected = false;


                // replace all this with actual logic.
                for (const UserData::UserConnect& db : dummyDB) {
                    if (connectionData->username == db.username && connectionData->password == db.password) {
                        isConnected = true;
                    }
                }

                if (!isConnected) {
                    enet_peer_disconnect(event.peer, 1);
                    enet_packet_destroy(event.packet);
                    break;
                }

                UserData::User* user;

                for (UserData::User& acc : account) {
                    if (connectionData->username == acc.username) {
                        user = &acc;
                    }
                }

                event.peer->data = user;
                
                enet_packet_destroy(event.packet);
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT: {
                Output::print("Disconnected user");
                event.peer->data = NULL;
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
                        event.packet -> dataLength,
                        event.packet -> data,
                        event.peer -> data,
                        event.channelID);
        
                /* Clean up the packet now that we're done using it. */
                enet_packet_destroy (event.packet);
            }
            case ENET_EVENT_TYPE_NONE: {
            }
        }
    }
    this->cleanup();
    return;
}


void Server::cleanup() {
    enet_deinitialize();
    enet_host_destroy(this->serv);
}