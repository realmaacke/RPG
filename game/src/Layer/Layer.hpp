#pragma once

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "src/Layer/ExtendableLayer.hpp"
#include <optional>
#include <vector>
class Layer {
public:
    // overrides
    void init(SDL_Window* window, SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    void event(SDL_Event event);
    void update();

    // Layer native methods.
    void toggleLayer(const std::string& name);
    void enableLayer(const std::string& name);
    void disableLayer(const std::string& name);

    void addLayer(ExtendableLayer& layer);

private:
    static bool compareIndex(const ExtendableLayer* a, const ExtendableLayer* b);
    std::optional<ExtendableLayer*> findLayerByName(const std::string& name);
    void sortLayers();


    std::vector<ExtendableLayer*> m_layers;
};