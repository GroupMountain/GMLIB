#pragma once
#include "Macros.h"
#include <iostream>

namespace gmlib::utils::StringUtils {

GMLIB_NDAPI std::string toSnakeCase(std::string_view str, bool withSpace = false);

GMLIB_NDAPI std::string toCamelCase(std::string_view str);

} // namespace gmlib::utils::StringUtils