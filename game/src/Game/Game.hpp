#pragma once
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "include/Rmlui/RmlUi_Platform_SDL.h"
#include "include/Rmlui/RmlUi_Renderer_SDL.h"
#include "src/Layer/Layer.hpp"
#include "src/NetClient/NetClient.hpp"
#include "src/Views/GameView.hpp"
#include "src/Views/MenuView.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <string>

struct WindowSettings {
    int width, height;
    std::string title;
};

class Game {
public:
    Game();
    int init();

    int start();
    void update();
    void event(SDL_Event &event);
    void render();

    void quit();
    void cleanup();
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    // Event handling

    // RMLUI
    std::unique_ptr<SystemInterface_SDL> m_rmlSystem;
    std::unique_ptr<RenderInterface_SDL> m_rmlRender;
    
    Layer m_layers;
    NetClient m_net;

    
    WindowSettings m_settings;
    bool isRunning = true;

    // Views
    std::unique_ptr<GameView> m_game_view;
    std::unique_ptr<MenuView> m_menu_view;
};