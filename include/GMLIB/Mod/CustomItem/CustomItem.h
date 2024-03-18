#pragma once
#include "GMLIB/GMLIB.h"

namespace GMLIB::Mod {

class CustomItem {
public:
    GMLIB_API static void unregisterItem(std::string id);
};

} // namespace GMLIB::Mod