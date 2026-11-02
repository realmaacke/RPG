#include "GameView.hpp"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_video.h"
#include "src/Layer/ExtendableLayer.hpp"

void GameView::init(SDL_Window* window, SDL_Renderer* renderer) {
    ExtendableLayer::init(window, renderer);
}

void GameView::update() {
    ExtendableLayer::update();
}

void GameView::events(SDL_Event event) {
    ExtendableLayer::events(event);
}

void GameView::render(SDL_Renderer* renderer) {
    

    
    ExtendableLayer::render(renderer);
}