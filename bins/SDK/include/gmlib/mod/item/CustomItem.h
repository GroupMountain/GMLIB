#pragma once
#include "Macros.h"
#include <string>

namespace gmlib::mod::item {

class ItemRegistry {
public:
    GMLIB_API static void unregisterItem(std::string const& id);
};

} // namespace gmlib::mod::item