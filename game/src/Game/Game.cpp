#include <memory>
#include <string>

#include "SDL3/SDL_error.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_filesystem.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"

#include "Game.hpp"
#include "src/ActionEvent/ActionEvent.hpp"
#include "src/Layer/ExtendableLayer.hpp"
#include "src/Output/Output.hpp"
#include "src/Views/GameView.hpp"
#include "src/Views/MenuView.hpp"

Game::Game() {
    m_settings = {
        1280,
        720,
        "Game title"
    };
}

int Game::init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        Output::error(
            "Failed to init SDL: " + std::string(SDL_GetError())
        );
        SDL_Quit();
        return 1;
    }

    if (!this->m_net.init()) {
        Output::error("Failed to start ENet");
        SDL_Quit();
        return 1;
    }

    this->m_window = SDL_CreateWindow(
        this->m_settings.title.c_str(),
        this->m_settings.width,
        this->m_settings.height,
        0
    );

    if (!this->m_window) {
        Output::error(
            "Failed to create window: " + std::string(SDL_GetError())
        );
        SDL_Quit();
        return 1;
    }

    this->m_renderer = SDL_CreateRenderer(this->m_window, nullptr);
    
    if (!this->m_renderer) {
        Output::error(
            "Failed to create renderer" + std::string(SDL_GetError())
        );
        this->cleanup();
        return 1;
    }

    const std::string base = SDL_GetBasePath();

    this->m_rmlSystem = std::make_unique<SystemInterface_SDL>(this->m_window);
    this->m_rmlRender = std::make_unique<RenderInterface_SDL>(this->m_renderer);

    Rml::SetSystemInterface(this->m_rmlSystem.get());
    Rml::SetRenderInterface(this->m_rmlRender.get());
    
    if (!Rml::Initialise()) {
        Output::error("Failed to initialize rml");
        this->cleanup();
        return 1;
    }

    if (!Rml::LoadFontFace(base + "assets/fonts/OpenSans/OpenSans-Medium.ttf")) {
        Output::error("Failed to load font");
        this->cleanup();
        return 1;
    }

    // Views here
    this->m_game_view = std::make_unique<GameView>(ExtendableLayerInjection{
        "GameView",
        50,
        false,
        SDL_GetBasePath(),
        "ui/game/game.rml"
    });
    this->m_layers.addLayer(*this->m_game_view);

    this->m_menu_view = std::make_unique<MenuView>(ExtendableLayerInjection{
        "MenuView",
        10,
        true,
        SDL_GetBasePath(),
        "ui/menu/menu.rml"
    });
    this->m_layers.addLayer(*this->m_menu_view);
    
    this->m_layers.init(this->m_window, this->m_renderer);

    
    ActionEvent& actions = ActionEvent::get();
    actions.game = this;
    actions.layer = &this->m_layers;
    actions.net = &this->m_net;

    actions.load(base + "assets/cfg/Binds.cfg");

    return 0;
}

int Game::start() {
    SDL_Event event;
    while (this->isRunning) {
        this->update();
        this->event(event);
        this->render();
    }
    this->cleanup();
    return 0;
}

void Game::update() {
    this->m_net.update();
    this->m_layers.update();
}

void Game::event(SDL_Event &event) {
    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            this->isRunning = false;
        }
        ActionEvent::get().handleEvent(event);
        this->m_layers.event(event);
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(this->m_renderer, 255, 255, 255, 255);
    SDL_RenderClear(this->m_renderer);

    this->m_rmlRender->BeginFrame();
    this->m_layers.render(this->m_renderer);
    this->m_rmlRender->EndFrame();

    SDL_RenderPresent(this->m_renderer);
}

void Game::quit() {
    this->isRunning = false;
}

void Game::cleanup() {
    ActionEvent::get().game = nullptr;
    ActionEvent::get().layer = nullptr;
    ActionEvent::get().net = nullptr;
    
    Rml::Shutdown();
    m_rmlRender.reset();
    m_rmlSystem.reset();
    SDL_DestroyRenderer(this->m_renderer);
    SDL_DestroyWindow(this->m_window);
    SDL_Quit();
}