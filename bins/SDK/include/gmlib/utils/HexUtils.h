#pragma once
#include "Macros.h"
#include <string>
#include <vector>

namespace gmlib::utils::HexUtils {

GMLIB_NDAPI std::string toString(uintptr_t x);

GMLIB_NDAPI std::string toString(uint8_t x);

GMLIB_NDAPI uintptr_t toHex(const std::string& str);

GMLIB_NDAPI std::vector<uint8_t> splitHex8(const std::string& str);

GMLIB_NDAPI std::vector<uint16_t> splitHex(const std::string& str);

} // namespace gmlib::utils::HexUtils