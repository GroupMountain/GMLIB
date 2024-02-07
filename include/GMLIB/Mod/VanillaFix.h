#pragma once
#include "GMLIB/GMLIB.h"

namespace GMLIB::Mod {

class VanillaFix {
public:
    GMLIB_API std::unordered_set<std::string> static getUnknownBlockLegacyNameList();

    GMLIB_API void static setAutoCleanUnknownBlockEnabled();

    GMLIB_API void static setFixCustomUndeadMobsEnabled();

    GMLIB_API void static setFixI18nEnabled();

};

}