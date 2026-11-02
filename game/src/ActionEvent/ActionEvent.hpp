#pragma once
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "src/Game/Game.hpp"
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <any>
#include <unordered_map>

// forward declare
class NetClient;

struct FileActions {
    std::optional<std::string> operation;
    std::optional<std::string> key;
    std::optional<std::string> target;
};

// Singelton obj, kinda god object.
// never any logic here besindes for binding.
class ActionEvent {
public:
    // Singleton stuff here
    static ActionEvent& get();
    ActionEvent(const ActionEvent&) = delete;
    ActionEvent& operator=(const ActionEvent&) = delete;

    // All instances.
    Game* game = nullptr;
    Layer* layer = nullptr;
    NetClient* net = nullptr;


    // here the actual class starts
    using Action = std::function<void()>;

    using PrefixAction = std::function<void(const std::string&)>;

    
    bool load(const std::string& path);
    bool save() const;
    
    // Bind a key to an action. Key by name ("F7") or by scancode.
    void addBind(const std::string& keyName, const std::string& action);
    void addBind(SDL_Scancode key, const std::string& action);
    void rmBind(SDL_Scancode key);

    // register action with params
    template<typename T>
    void registerAction(
        const std::string& action,
        std::function<void(const T&)> fn
    ) {
        this->m_actions_params[action] = [fn](const std::any& payload) {
            fn(std::any_cast<const T&>(payload));
        };
    }
    
    // Exact action: "quit" -> fn()
    void registerAction(const std::string& action, Action fn);
    // Prefix action: "display_" matches "display_GameView" -> fn("GameView")
    void registerPrefix(const std::string& prefix, PrefixAction fn);
    
    void handleEvent(const SDL_Event& event);
    
    void run(const std::string& action);

    // run with template
    template <typename T>
    void run(const std::string& action, const T& payload) {
        ActionMapParams::const_iterator it = this->m_actions_params.find(action);
        
        if (it != this->m_actions_params.end()) {
            it->second(payload);
            return;
        };

        this->runPredefinedWithPayload(action, payload);
    }

    bool runPredefinedWithPayload(const std::string& action, const std::any& payload);

private:
    ActionEvent() = default;
    
    using BindMap = std::map<SDL_Scancode, std::string>;
    using ActionMap = std::unordered_map<std::string, Action>;
    using ActionMapParams = std::unordered_map<std::string, std::function<void(const std::any&)>>;
    using PrefixMap = std::unordered_map<std::string, PrefixAction>;
    
    
    std::string m_path;
    BindMap m_binds;
    ActionMap m_actions;
    ActionMapParams m_actions_params;
    PrefixMap m_prefixes;
};