#pragma once
#include "GMLIB/Macros.h"
#include "mc/entity/utilities/ActorDamageCause.h"

namespace GMLIB::Mod {

class DamageCause {
public:
    GMLIB_API static void setCustomDamageCauseEnabled();

    GMLIB_API static bool registerDamageCause(std::string const& causeName);

    GMLIB_API static bool setVanillaCauseMessage(ActorDamageCause cause, std::string_view msg);

    GMLIB_NDAPI static ::ActorDamageCause getCauseFromName(std::string const& causeName);

    GMLIB_API static bool
    registerCustomDeathMessage(ActorDamageCause cause, std::string_view message, std::string_view killerType = "empty");
};

} // namespace GMLIB::Mod