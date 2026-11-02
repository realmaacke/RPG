#pragma once
#include <string>
#include "RmlUi/Core/Context.h"
#include "RmlUi/Core/Element.h"
#include "RmlUi/Core/ElementDocument.h"
#include "RmlUi/Core/Event.h"
#include "RmlUi/Core/EventListener.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"

struct ExtendableLayerInjection {
    std::string name;
    int zIndex = 999;
    bool isVissible = true;
    std::string base;
    std::string document;
};

class ExtendableLayer : public Rml::EventListener {
public:
    std::string name;
    int zIndex;
    bool isVissible;
    std::string base;
    std::string document;

    ExtendableLayer(ExtendableLayerInjection conf);
    virtual ~ExtendableLayer() = default;


    virtual void init(SDL_Window* window, SDL_Renderer* renderer);
    virtual void render(SDL_Renderer* renderer);
    virtual void events(SDL_Event event);
    virtual void update();

    void ProcessEvent(Rml::Event& event) override;

protected:
    virtual void onClick(const std::string& id, Rml::Element& element) {}
    Rml::Element* element(const std::string& id);

    // ai slop.
    void setText(const std::string& id, const std::string& text);

private:
    SDL_Window* m_window = nullptr;
    Rml::Context* m_ctx = nullptr;
    Rml::ElementDocument* m_document = nullptr;
};