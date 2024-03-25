#pragma once
#include "GMLIB/GMLIB.h"
#include "ll/api/event/Event.h"
#include "mc/resources/ResourcePackRepository.h"

namespace GMLIB::Event::Registries {

class ResourcePackInitEvent : public ll::event::Event {
protected:
    ResourcePackRepository& mRepo;

public:
    constexpr explicit ResourcePackInitEvent(ResourcePackRepository& repo) : mRepo(repo) {}

    GMLIB_API ResourcePackRepository& getResourcePackRepository() const;
};

} // namespace GMLIB::Event::Registries