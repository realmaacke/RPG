#include "ExtendableLayer.hpp"
#include "SDL3/SDL_video.h"
#include "src/Output/Output.hpp"
#include "include/Rmlui/RmlUi_Platform_SDL.h"

ExtendableLayer::ExtendableLayer(ExtendableLayerInjection conf) {
    this->name = conf.name;
    this->zIndex = conf.zIndex;
    this->isVissible = conf.isVissible;
    this->base = conf.base;
    this->document = conf.document;
}

void ExtendableLayer::init(SDL_Window* window, SDL_Renderer* renderer) {
    if (this->document.empty()) return;

    this->m_window = window;

    int w = 0, h = 0;
    SDL_GetRenderOutputSize(renderer, &w, &h);

    this->m_ctx = Rml::CreateContext(this->name, Rml::Vector2i(w, h));
    if (!this->m_ctx) {
        Output::error("Failed to create context for " + this->name);
        return;
    }

    this->m_document = this->m_ctx->LoadDocument(
        this->base + "assets/" + this->document
    );
    if (!this->m_document) {
        Output::error("Failed to load " + this->document);
        return;
    }

    this->m_document->Show();

    // slop
    this->m_document->AddEventListener("click", this);
}

void ExtendableLayer::update() {
    if (this->m_ctx) this->m_ctx->Update();
}

void ExtendableLayer::render(SDL_Renderer*) {
    if (this->m_ctx) this->m_ctx->Render();
}

void ExtendableLayer::events(SDL_Event event) {
    if (!this->m_ctx || !this->isVissible) return;
    RmlSDL::InputEventHandler(this->m_ctx, this->m_window, event);
}

// Slop
void ExtendableLayer::ProcessEvent(Rml::Event& event) {
    Rml::Element* el = event.GetTargetElement();
    while (el && el->GetId().empty()) el = el->GetParentNode();
    if (!el) return;
    this->onClick(el->GetId(), *el);
}

Rml::Element* ExtendableLayer::element(const std::string& id) {
    return this->m_document ? this->m_document->GetElementById(id) : nullptr;
}

// slop
void ExtendableLayer::setText(const std::string& id, const std::string& text) {
    if (Rml::Element* el = this->element(id)) el->SetInnerRML(text);  // parsed as RML; escape untrusted text
}