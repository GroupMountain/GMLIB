#pragma once
#include "Macros.h"
#include <filesystem>
#include <vector>

namespace gmlib::utils::FileUtils {

GMLIB_NDAPI std::vector<std::string> getAllFileNameInDirectory(std::filesystem::path const& path);

GMLIB_NDAPI std::vector<std::string> getAllFileFullNameInDirectory(std::filesystem::path const& path);

GMLIB_NDAPI std::vector<std::string> getAllFilePathInDirectory(std::filesystem::path const& path);

} // namespace gmlib::utils::FileUtils
