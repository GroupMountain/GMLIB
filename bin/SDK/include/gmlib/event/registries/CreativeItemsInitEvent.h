#pragma once
#include "Macros.h"
#include "ll/api/event/Event.h"
#include "mc/world/item/registry/ItemRegistryRef.h"

namespace gmlib::event::registries {

class CreativeItemInitEvent final : public ll::event::Event {
protected:
    ItemRegistryRef& mRef;

public:
    constexpr explicit CreativeItemInitEvent(ItemRegistryRef& ref) : mRef(ref) {}

    GMLIB_NDAPI ItemRegistryRef& getItemRegistryRef() const;
};

} // namespace gmlib::event::registries