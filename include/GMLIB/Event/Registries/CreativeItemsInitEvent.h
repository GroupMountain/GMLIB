#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/Event.h"

namespace GMLIB::Event::Registries {

class CreativeItemInitEvent : public ll::event::Event {

public:
    constexpr explicit CreativeItemInitEvent() {}
};

} // namespace GMLIB::Event::Registries