#include "Global.h"
#include <GMLIB/Mod/Addons/VanillaFix.h>

namespace GMLIB::Mod {

LL_TYPE_INSTANCE_HOOK(UndeadMobFix, HookPriority::Highest, Actor, "?isInvertedHealAndHarm@Actor@@QEBA_NXZ", bool) {
    return this->hasFamily("undead");
}

struct Impl {
    ll::memory::HookRegistrar<UndeadMobFix> r;
};

std::unique_ptr<Impl> impl;

void VanillaFix::setFixCustomUndeadMobsEnabled() {
    if (!impl) {
        impl = std::make_unique<Impl>();
    }
}

} // namespace GMLIB::Mod