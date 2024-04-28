#pragma once
#include "GMLIB/Macros.h"
#include "mc/resources/ResourcePackRepository.h"

namespace GMLIB::Mod {

class CustomPacks {
public:
    GMLIB_API static void addCustomPackPath(std::string  const& path);

    GMLIB_API static void setCustomPackPath(ResourcePackRepository& repo, std::string  const& path, PackType type);
};

} // namespace GMLIB::Mod