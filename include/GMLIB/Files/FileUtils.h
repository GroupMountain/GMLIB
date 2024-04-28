#pragma once
#include "GMLIB/Macros.h"

namespace GMLIB::Files::FileUtils {

GMLIB_API std::vector<std::string> getAllFileNameInDirectory(std::string const& path);

GMLIB_API std::vector<std::string> getAllFileFullNameInDirectory(std::string const& path);

GMLIB_API std::vector<std::string> getAllFilePathInDirectory(std::string const& path);

} // namespace GMLIB::Files::FileUtils
