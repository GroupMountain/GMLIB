#pragma once
#include "GMLIB/GMLIB.h"
#include "nlohmann/json.hpp"

namespace GMLIB::Mod {

class RapidJsonItem {
public:
    GMLIB_API static bool loadJsonItem(std::string json);

    GMLIB_API static bool loadJsonItem(nlohmann::json json);
};

} // namespace GMLIB::Mod