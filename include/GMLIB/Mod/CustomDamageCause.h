#pragma once
#include "GMLIB/GMLIB.h"

namespace GMLIB::Mod {

class DamageCause {
public:
    GMLIB_API static void setCustomDamageCauseEnabled();

    GMLIB_API static bool registerDamageCause(int causeId, std::string causeName);
};

} // namespace GMLIB::Mod