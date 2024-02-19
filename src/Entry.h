#pragma once

#include <ll/api/plugin/NativePlugin.h>

namespace GMLIB {

[[nodiscard]] auto getSelfPluginInstance() -> ll::plugin::NativePlugin&;

} // namespace GMLIB
