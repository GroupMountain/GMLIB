#pragma once
#include "GMLIB/GMLIB.h"
#include "mc/entity/utilities/ActorDamageCause.h"

namespace GMLIB::Mod {

class DamageCause {
public:
    GMLIB_API static void setCustomDamageCauseEnabled();

    GMLIB_API static bool registerDamageCause(std::string causeName);

    GMLIB_API static bool setVanillaCauseMessage(ActorDamageCause cause, std::string_view msg);

    GMLIB_API static ::ActorDamageCause getCauseFromName(std::string& causeName);
};

} // namespace GMLIB::Mod