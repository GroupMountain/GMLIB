#pragma once
#include "GMLIB/Macros.h"

namespace GMLIB::Files::FileUtils {

GMLIB_NDAPI std::vector<std::string> getAllFileNameInDirectory(std::filesystem::path const& path);

GMLIB_NDAPI std::vector<std::string> getAllFileFullNameInDirectory(std::filesystem::path const& path);

GMLIB_NDAPI std::vector<std::string> getAllFilePathInDirectory(std::filesystem::path const& path);

} // namespace GMLIB::Files::FileUtils
