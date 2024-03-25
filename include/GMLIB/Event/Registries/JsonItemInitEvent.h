#pragma once
#include "GMLIB/GMLIB.h"
#include "ll/api/event/Event.h"
#include "mc/world/item/components/ItemVersion.h"
#include "mc/world/item/registry/ItemRegistry.h"
#include "mc/world/level/storage/Experiments.h"

namespace GMLIB::Event::Registries {

using RegistryCall = std::function<
    void(class WeakPtr<class Item>&, class Json::Value&, class SemVersion const&, bool, class Experiments const&)>;

class JsonItemInitEvent : public ll::event::Event {
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

    GMLIB_API ItemRegistry& getRegistry() const;
    GMLIB_API RegistryCall& getRegistryCall()const;
    GMLIB_API Experiments const& getExperiments()const;
    GMLIB_API ::ItemVersion& getItemVersion() const;
};

} // namespace GMLIB::Event::Registries