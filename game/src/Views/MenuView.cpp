#include <functional>
#include <unordered_map>

#include "RmlUi/Core/Element.h"

#include "MenuView.hpp"
#include "src/ActionEvent/ActionEvent.hpp"
#include "src/ActionEvent/DefinedActions.hpp"
#include "src/Layer/ExtendableLayer.hpp"
#include "src/Output/Output.hpp"

#include "src/Utility.hpp"

void MenuView::init(SDL_Window *window, SDL_Renderer *renderer) {
    ExtendableLayer::init(window, renderer);

    this->registerListeners();
}

void MenuView::update() {
    this->refreshQueuePopup();
    ExtendableLayer::update();
}

void MenuView::registerListeners() {
    if (element("menu-form")) {
        BindEvent(element("menu-form"), Rml::EventId::Submit, [](Rml::Event& event) {
            Rml::String user = event.GetParameter<Rml::String>("username", "");
            Rml::String pass = event.GetParameter<Rml::String>("password", "");

            if (user.empty() || pass.empty()) return;

            PacketLogin loginPacket{};
            std::strncpy(loginPacket.username, user.c_str(), sizeof(loginPacket.username) - 1);
            std::strncpy(loginPacket.password, pass.c_str(), sizeof(loginPacket.password) - 1);

            ActionEvent::get().run("connect_server", loginPacket);
        });
    }
}

void MenuView::refreshQueuePopup() {

}

void MenuView::onClick(const std::string& id, Rml::Element& pressedElement) {
    if (!this->isVissible) return;
    
    static const std::unordered_map<std::string, std::function<void()>> actions = {
        {"connect", []() {
            ActionEvent::get().run("connect_server");
        }},
        {"options", []() {
            Output::print("Options clicked");
        }},
        {"create-account", []() {
            Output::print("create-account clicked");
        }},
        {"my-account", []() {
            Output::print("my-account clicked");
        }},
        {"official-site", []() {
            Output::print("official-site clicked");
        }}, 
        {"quit", []() {
            ActionEvent::get().run("quit_game");
        }},
        {"diconeect-from-queue", [this](){
            ActionEvent::get().run("net_disconnect");
            element("queue-overlay")->SetClass("hidden", true);
        }},
    };

    if (auto it = actions.find(id); it != actions.end()) {
        it->second();
    }
}
