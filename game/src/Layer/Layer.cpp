#include "Layer.hpp"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "src/Layer/ExtendableLayer.hpp"
#include "src/Output/Output.hpp"
#include <algorithm>
#include <optional>

void Layer::init(SDL_Window* window, SDL_Renderer* renderer) {
    for (ExtendableLayer* layer : this->m_layers) {
        layer->init(window, renderer);
    }
}

void Layer::render(SDL_Renderer* renderer) {
    this->sortLayers();
    for (ExtendableLayer* layer : this->m_layers) {
        if (!layer->isVissible) continue;
        layer->render(renderer);
    }
}

void Layer::event(SDL_Event event) {
    for (ExtendableLayer* layer : this->m_layers) {
        if (!layer->isVissible) continue;
        layer->events(event);
    }
}

void Layer::update() {
    for (ExtendableLayer* layer : this->m_layers) {
        if (!layer->isVissible) continue;
        layer->update();
    }
}

// native layer methods below
std::optional<ExtendableLayer*> Layer::findLayerByName(const std::string& name) {
    for (ExtendableLayer* layer : this->m_layers) {
        if (layer->name == name) return layer;
    }
    return std::nullopt;
}

void Layer::toggleLayer(const std::string& name) {
    std::optional<ExtendableLayer*> layerOpt= this->findLayerByName(name);
    if (!layerOpt.has_value()) return;

    ExtendableLayer* layer = layerOpt.value();

    layer->isVissible = !layer->isVissible;
}

void Layer::enableLayer(const std::string& name) {
    std::optional<ExtendableLayer*> layerOpt= this->findLayerByName(name);
    if (!layerOpt.has_value()) return;

    ExtendableLayer* layer = layerOpt.value();
    layer->isVissible = true;
}

void Layer::disableLayer(const std::string& name) {
    std::optional<ExtendableLayer*> layerOpt= this->findLayerByName(name);
    if (!layerOpt.has_value()) return;

    ExtendableLayer* layer = layerOpt.value();
    layer->isVissible = false;
}

void Layer::addLayer(ExtendableLayer& layer) {
    if (this->findLayerByName(layer.name)) {
        Output::error("Layer with that name already exists");
        return;
    }
    this->m_layers.emplace_back(&layer);
}

bool Layer::compareIndex(const ExtendableLayer* a, const ExtendableLayer* b) {
    return a->zIndex > b->zIndex;
}

void Layer::sortLayers() {
    std::stable_sort(
        this->m_layers.begin(),
        this->m_layers.end(),
        this->compareIndex
    );
}

