#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/Event.h"
#include "mc/world/item/registry/ItemRegistryRef.h"

namespace GMLIB::Event::Registries {

class CreativeItemInitEvent final : public ll::event::Event {
protected:
    ItemRegistryRef& mRef;

public:
    constexpr explicit CreativeItemInitEvent(ItemRegistryRef& ref) : mRef(ref) {}

    GMLIB_NDAPI ItemRegistryRef& getItemRegistryRef() const;
};

} // namespace GMLIB::Event::Registries