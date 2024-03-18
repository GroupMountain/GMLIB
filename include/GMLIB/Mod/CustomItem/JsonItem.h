#pragma once
#include "GMLIB/GMLIB.h"
#include "nlohmann/json.hpp"

namespace GMLIB::Mod {

class JsonItem {
public:
    GMLIB_API static void loadJsonItem(std::string json);

    GMLIB_API static void loadJsonItem(nlohmann::json json);
};

} // namespace GMLIB::Mod