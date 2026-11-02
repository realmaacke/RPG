#pragma once

#include "enet/enet.h"
#include "enet/types.h"
#include <cstddef>
class Server {
public:
    bool init();

    void runServer();
    void onEvent(ENetEvent& event);

    void cleanup();

    bool getState();
    void setState(bool state);
private:
    bool state = true;

    ENetAddress address;
    ENetHost* serv;

    size_t MAX_CLIENTS = 32;
    size_t MAX_CHANNELS = 2;

    // Assumed amount of in & out bandwidth.
    enet_uint32 INC_BADNWIDTH = 0;
    enet_uint32 OUT_BANDWIDTH = 0;

    // Counted in ms.
    enet_uint32 EVENT_TIMEOUT = 1000;
protected:

};