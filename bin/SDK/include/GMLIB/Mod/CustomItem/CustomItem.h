#pragma once
#include "GMLIB/Macros.h"

namespace GMLIB::Mod {

class CustomItem {
public:
    GMLIB_API static void unregisterItem(std::string const&  id);
};

} // namespace GMLIB::Mod