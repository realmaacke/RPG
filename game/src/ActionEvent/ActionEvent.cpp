#include "ActionEvent.hpp"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_scancode.h"
#include "src/Output/Output.hpp"
#include <fstream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "../NetClient/NetClient.hpp"

struct PredefinedPrefix {
    const char* prefix;
    void (*fn)(const std::string&);
};

struct PreDefinedAction {
    const char* name;
    void (*fn)();
};

static const PreDefinedAction predefinedActions[] = {
{"quit_game",[]() {
            ActionEvent& a = ActionEvent::get();
            if (!a.game) return;
            a.game->quit();
    }},
{"start_game",[](){
        ActionEvent& a = ActionEvent::get();
        if (!a.layer) return;
        a.layer->disableLayer("MenuView");
        a.layer->enableLayer("GameView");
    }},
{"connect_server", []() {
        ActionEvent& a = ActionEvent::get();
        // if (a.net) a.net->connect("127.0.0.1", 7777);
    }},
{"net_disconnect", [](){
        ActionEvent& a = ActionEvent::get();
        if (!a.layer) return;
        // if (!a.net) return;
        // a.net->disconnect();
        a.layer->disableLayer("GameView");
        a.layer->enableLayer("MenuView");
    }},
{"net_admitted", [](){
        ActionEvent::get().run("start_game");
    }},
};

static const PredefinedPrefix predefinedPrefixes[] = {
    {"display_", [](const std::string& layer) {
        ActionEvent& a = ActionEvent::get();
        if (a.layer) a.layer->toggleLayer(layer);
    }},
};

ActionEvent& ActionEvent::get() {
    static ActionEvent instance;
    return instance;
}

bool ActionEvent::load(const std::string& path) {
    this->m_path = path;

    std::ifstream file(path);
    if (!file) {
        Output::error("Failed to open: " + path);
        return false;
    }

    this->m_binds.clear();

    std::string line;
    while (std::getline(file, line)) {
        if (line.rfind('#') != std::string::npos) continue;
        if (line.empty()) continue;

        std::string section;
        std::stringstream ss(line);
        FileActions fileActions;

        while(std::getline(ss, section, ' ')) {
            if (section == "bind" || section == "unbind") {
                fileActions.operation = section;
                continue;
            }

            if (fileActions.operation.has_value() && !fileActions.key.has_value()) {
                SDL_Scancode sc = SDL_GetScancodeFromName(section.c_str());

                if (sc == SDL_SCANCODE_UNKNOWN) {
                    Output::error(
                        "Key is invalid: " + section
                    );
                    break;
                }
                fileActions.key = section;
                continue;
            }
            fileActions.target = section;
        }

        if (fileActions.operation == "bind") {
            if (!fileActions.key.has_value() || !fileActions.target.has_value()) {
                Output::error("Incomplete bind: " + line);
                continue;
            }
            this->addBind(fileActions.key.value(), fileActions.target.value());
        } else if (fileActions.operation == "unbind") {
            if (!fileActions.key.has_value()) {
                Output::error("Incomplete bind: " + line);
                continue;
            }
            SDL_Scancode sc = SDL_GetScancodeFromName(fileActions.key.value().c_str());
            if (sc == SDL_SCANCODE_UNKNOWN) {
                Output::error("Key is unknown: " + fileActions.key.value());
                continue;
            }
            this->rmBind(sc);
        } else {
            Output::error("Unknown operation: " + fileActions.operation.value());
        }
    }
    return true;
}

bool ActionEvent::save() const {
    std::vector<std::string> lines;
    {
        std::ifstream in(this->m_path);
        std::string line;
        while (std::getline(in, line)) lines.push_back(line);
    }

    std::set<SDL_Scancode> written;
    std::vector<std::string> out;

    for (const std::string& line : lines) {
        if (line.empty() || line.rfind('#') != std::string::npos) {
            out.push_back(line);
            continue;
        }
        
        std::stringstream ss(line);
        std::string operation;
        std::string key;
        ss >> operation >> key;

                const SDL_Scancode sc = SDL_GetScancodeFromName(key.c_str());

        if (operation == "bind" && sc != SDL_SCANCODE_UNKNOWN) {
            const BindMap::const_iterator bind = this->m_binds.find(sc);
            if (bind == this->m_binds.end()) continue;  // removed
            if (written.count(sc) > 0) continue;        // duplicate line

            written.insert(sc);
            out.push_back("bind " + key + " " + bind->second);
        } else if (operation == "unbind" && sc != SDL_SCANCODE_UNKNOWN) {
            // If the key is bound again, keeping the unbind would undo it on next load
            if (this->m_binds.find(sc) != this->m_binds.end()) continue;
            out.push_back(line);
        } else {
            out.push_back(line);
        }
    }

    for (const BindMap::value_type& bind : this->m_binds) {
        if (written.count(bind.first) > 0) continue;
        out.push_back(std::string("bind ") + SDL_GetScancodeName(bind.first) + " " + bind.second);
    }

    std::ofstream file(this->m_path);
    if (!file) {
        Output::error("Failed to write: " + this->m_path);
        return false;
    }
    for (const std::string& line : out) {
        file << line << "\n";
    }
    return true;
}

void ActionEvent::addBind(const std::string& key, const std::string& target) {
    const SDL_Scancode sc = SDL_GetScancodeFromName(key.c_str());
    if (sc == SDL_SCANCODE_UNKNOWN) {
        Output::error("Key is unknown: " + key);
        return;
    }
    this->addBind(sc, target);
}

void ActionEvent::addBind(SDL_Scancode key, const std::string& target) {
    this->m_binds[key] = target;
}

void ActionEvent::rmBind(SDL_Scancode key) {
    this->m_binds.erase(key);
}

void ActionEvent::registerAction(const std::string& action, Action fn) {
    this->m_actions[action] = fn;
}

void ActionEvent::registerPrefix(const std::string& prefix, PrefixAction fn) {
    this->m_prefixes[prefix] = fn;
}

void ActionEvent::handleEvent(const SDL_Event& event) {
    if (event.type != SDL_EVENT_KEY_DOWN || event.key.repeat) return;

    const BindMap::const_iterator bind = this->m_binds.find(event.key.scancode);
    if (bind == this->m_binds.end()) return;

    this->run(bind->second);
}

void ActionEvent::run(const std::string& action) {
    const ActionMap::const_iterator exact = this->m_actions.find(action);
    if (exact != this->m_actions.end()) {
        exact->second();
        return;
    }

    for (const PreDefinedAction& entry : predefinedActions) {
        if (action == entry.name) {
            entry.fn();
            return;
        }
    }

    for (const PredefinedPrefix& entry : predefinedPrefixes) {
        const std::string prefix = entry.prefix;
        if (action.rfind(prefix, 0) == 0) {
            entry.fn(action.substr(prefix.size()));
            return;
        }
    }

    for (const PrefixMap::value_type& entry : this->m_prefixes) {
        if (action.rfind(entry.first, 0) == 0) {
            entry.second(action.substr(entry.first.size()));
            return;
        }
    }
}