#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/Event.h"
#include "mc/resources/ResourcePackRepository.h"

namespace GMLIB::Event::Registries {

class ResourcePackInitEvent final : public ll::event::Event {
protected:
    ResourcePackRepository& mRepo;

public:
    constexpr explicit ResourcePackInitEvent(ResourcePackRepository& repo) : mRepo(repo) {}

    GMLIB_NDAPI ResourcePackRepository& getResourcePackRepository() const;
};

} // namespace GMLIB::Event::Registries