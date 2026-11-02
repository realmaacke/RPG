#pragma once
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "src/Layer/ExtendableLayer.hpp"

class GameView : public ExtendableLayer {
public:
    using ExtendableLayer::ExtendableLayer;
    
    void init(SDL_Window* window, SDL_Renderer* render) override;
    void update() override;
    void events(SDL_Event event) override;
    void render(SDL_Renderer *renderer) override;
};