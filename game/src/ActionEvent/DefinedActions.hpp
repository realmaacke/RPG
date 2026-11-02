#pragma once
#include <any>

#include "ActionEvent.hpp"
#include "src/NetClient/NetClient.hpp"
#include "src/NetClient/Protocol.hpp"
#include "src/Output/Output.hpp"

struct ActionStructs {
    struct Payload {
        const char* name;
        void (*fn)(const std::any&);
    };
    
    struct Prefix {
        const char* prefix;
        void (*fn)(const std::string&);
    };
    
    struct Normal {
        const char* name;
        void (*fn)();
    };
};

// Action with payload
static const ActionStructs::Payload predefinedActionWithPayload[] = {
{"connect_server", [](const std::any& payload)
{
    
    ActionEvent& a = ActionEvent::get();
    if (!a.net) return;

    const PacketLogin& loginData = std::any_cast<const PacketLogin&>(payload);

    a.net->connect();
    a.net->sendStruct(loginData);
}},
};

// Normal Actions.
static const ActionStructs::Normal predefinedActions[] = {
{"quit_game",[]()
{
    ActionEvent& a = ActionEvent::get();
    if (!a.game) return;
    a.game->quit();       
}},
{"start_game",[]()
{
    ActionEvent& a = ActionEvent::get();
    if (!a.layer) return;
    a.layer->disableLayer("MenuView");
    a.layer->enableLayer("GameView");
}},

{"net_disconnect", []()
{
    ActionEvent& a = ActionEvent::get();
    if (!a.layer) return;
    if (!a.net) return;
    a.net->disconnect();
    a.layer->disableLayer("GameView");
    a.layer->enableLayer("MenuView");
}},
{"net_admitted", []()
{
    ActionEvent::get().run("start_game");
}},
};

// Prefix Actions.
static const ActionStructs::Prefix predefinedPrefixes[] = {
    {"display_", [](const std::string& layer) {
        ActionEvent& a = ActionEvent::get();
        if (a.layer) a.layer->toggleLayer(layer);
    }},
};