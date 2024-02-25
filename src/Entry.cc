#include "Entry.h"
#include "Global.h"

namespace GMLIB {

namespace {

std::unique_ptr<std::reference_wrapper<ll::plugin::NativePlugin>> selfPluginInstance;

auto disable(ll::plugin::NativePlugin& self) -> bool {
    return false;
}

auto enable(ll::plugin::NativePlugin& self) -> bool {
    enableLib();
    return true;
}

auto load(ll::plugin::NativePlugin& self) -> bool {
    selfPluginInstance = std::make_unique<std::reference_wrapper<ll::plugin::NativePlugin>>(self);
    loadLib();
    return true;
}

auto unload(ll::plugin::NativePlugin& self) -> bool {
    logger.error("Do NOT use \"ll disable\" or \"ll unload\" to disable or unload GMLIB!");
    logger.error("Disabling or unloading GMLIB may cause issues with other plugins, or even result in crash!");
    logger.error("If you no longer need this library, please close your server and uninstall it!");
    return false;
}

} // namespace

auto getSelfPluginInstance() -> ll::plugin::NativePlugin& {
    if (!selfPluginInstance) {
        throw std::runtime_error("selfPluginInstance is null");
    }
    return *selfPluginInstance;
}

} // namespace GMLIB

extern "C" {
_declspec(dllexport) auto ll_plugin_disable(ll::plugin::NativePlugin& self) -> bool { return GMLIB::disable(self); }
_declspec(dllexport) auto ll_plugin_enable(ll::plugin::NativePlugin& self) -> bool { return GMLIB::enable(self); }
_declspec(dllexport) auto ll_plugin_load(ll::plugin::NativePlugin& self) -> bool { return GMLIB::load(self); }
_declspec(dllexport) auto ll_plugin_unload(ll::plugin::NativePlugin& self) -> bool { return GMLIB::unload(self); }
}