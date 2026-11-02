#pragma once

#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/EventListener.h>
#include <utility>

template <typename Func>
inline void BindEvent(Rml::Element* element, Rml::EventId id, Func&& fn) {
    if (!element) return;

    struct LambdaListener final : public Rml::EventListener {
        std::decay_t<Func> callback;

        explicit LambdaListener(Func&& f) : callback(std::forward<Func>(f)) {}

        void ProcessEvent(Rml::Event& event) override {
            callback(event);
        }

        // RmlUi calls OnDetach when the element is destroyed, cleaning up memory
        void OnDetach(Rml::Element* /*element*/) override {
            delete this;
        }
    };

    element->AddEventListener(id, new LambdaListener(std::forward<Func>(fn)));
}