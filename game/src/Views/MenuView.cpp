#include "MenuView.hpp"
#include "RmlUi/Core/Element.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "src/ActionEvent/ActionEvent.hpp"
#include "src/Layer/ExtendableLayer.hpp"
#include "src/NetClient/NetClient.hpp"
#include "src/Output/Output.hpp"
#include <functional>
#include <unordered_map>

static std::string formatTime(unsigned int seconds) {
    const unsigned int minutes = seconds / 60;
    const unsigned int rest = seconds % 60;
    return std::to_string(minutes) + ":" + (rest < 10 ? "0" : "") + std::to_string(rest);
}

struct MenuOptions {
    std::string name;
    std::function<void()> fn;
};

void MenuView::update() {
    this->refreshQueuePopup();
    ExtendableLayer::update();
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
