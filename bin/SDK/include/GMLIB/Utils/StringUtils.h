#pragma once
#include "GMLIB/Macros.h"
#include <iostream>

namespace GMLIB::StringUtils {

GMLIB_NDAPI std::string toSnakeCase(std::string_view str, bool withSpace = false);

GMLIB_NDAPI std::string toCamelCase(std::string_view str);

}