#pragma once
#include "Macros.h"
#include "mc/resources/ResourcePackRepository.h"

namespace gmlib::tools {

class AddonsLoader {
public:
    GMLIB_API static void addCustomPackPath(std::filesystem::path const& path);

    GMLIB_API static void
    setCustomPackPath(ResourcePackRepository& repo, std::filesystem::path const& path, PackType type);
};

} // namespace gmlib::tools