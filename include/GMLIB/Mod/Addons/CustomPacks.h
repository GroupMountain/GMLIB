#pragma once
#include "GMLIB/GMLIB.h"
#include "mc/resources/ResourcePackRepository.h"

namespace GMLIB::Mod {

class CustomPacks {
public:
    GMLIB_API static void addCustomPackPath(std::string path);

    GMLIB_API static void setCustomPackPath(ResourcePackRepository& repo, std::string path, PackType type);
};

} // namespace GMLIB::Mod