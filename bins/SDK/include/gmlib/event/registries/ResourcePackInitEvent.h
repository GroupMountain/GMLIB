#pragma once
#include "Macros.h"
#include "ll/api/event/Event.h"
#include "mc/resources/ResourcePackRepository.h"

namespace gmlib::event::registries {

class ResourcePackInitEvent final : public ll::event::Event {
protected:
    ResourcePackRepository& mRepo;

public:
    constexpr explicit ResourcePackInitEvent(ResourcePackRepository& repo) : mRepo(repo) {}

    GMLIB_NDAPI ResourcePackRepository& getResourcePackRepository() const;
};

} // namespace gmlib::event::registries