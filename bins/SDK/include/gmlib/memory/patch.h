#pragma once
#include "Macros.h"

namespace gmlib::memory {

GMLIB_API void Patch(
    char const*                  symbol,
    std::vector<uint16_t> const& origin,
    std::vector<uint8_t> const&  replace,
    uint64_t                     size = 114514
);

GMLIB_API void Patch(char const* symbol, std::string const& origin, std::string const& replace, uint64_t size = 114514);

} // namespace gmlib::memory