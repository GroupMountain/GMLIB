#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/Event.h"
#include "mc/world/item/components/ItemVersion.h"
#include "mc/world/item/registry/ItemRegistry.h"
#include "mc/world/level/storage/Experiments.h"

namespace GMLIB::Event::Registries {

using RegistryCall = std::function<
    void(class WeakPtr<class Item>&, class Json::Value&, class SemVersion const&, bool, class Experiments const&)>;

class JsonItemInitEvent final : public ll::event::Event {
protected:
    ItemRegistry&      mRegistry;
    RegistryCall&      mCall;
    Experiments const& mExperiments;
    ::ItemVersion&     mVersion;

public:
    constexpr explicit JsonItemInitEvent(
        ItemRegistry&      registry,
        RegistryCall&      call,
        Experiments const& experiments,
        ::ItemVersion&     version
    )
    : mRegistry(registry),
      mCall(call),
      mExperiments(experiments),
      mVersion(version) {}

    GMLIB_NDAPI ItemRegistry&      getRegistry() const;
    GMLIB_NDAPI RegistryCall&      getRegistryCall() const;
    GMLIB_NDAPI Experiments const& getExperiments() const;
    GMLIB_NDAPI ::ItemVersion&     getItemVersion() const;
};

} // namespace GMLIB::Event::Registries