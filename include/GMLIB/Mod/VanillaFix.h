#pragma once
#include "GMLIB/GMLIB.h"

class GMLIB_VanillaFix {
public:
    GMLIB_API std::unordered_set<std::string> static getUnknownBlockLegacyNameList();

    GMLIB_API void static setAutoCleanUnknownBlockEnabled(bool value = true);

    GMLIB_API void static setFixCustomUndeadMobs(bool value = true);
};
