#pragma once
#include "GMLIB/Macros.h"

namespace GMLIB::Files::FileUtils {

GMLIB_API std::vector<std::string> getAllFileNameInDirectory(const std::string& path);

GMLIB_API std::vector<std::string> getAllFileFullNameInDirectory(const std::string& path);

GMLIB_API std::vector<std::string> getAllFilePathInDirectory(const std::string& path);

} // namespace GMLIB::Files::FileUtils
