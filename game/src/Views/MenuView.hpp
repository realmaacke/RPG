#pragma once
#include "RmlUi/Core/Element.h"
#include "RmlUi/Core/Event.h"
#include "RmlUi/Core/EventListener.h"
#include "src/Layer/ExtendableLayer.hpp"

class MenuView : public ExtendableLayer {
public:
    using ExtendableLayer::ExtendableLayer;

    void update() override;
    
protected:
    void onClick(const std::string& id, Rml::Element& element) override;

private:
    void refreshQueuePopup();

    std::string m_popupKey;
};