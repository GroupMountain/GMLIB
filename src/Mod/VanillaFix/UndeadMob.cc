#include "Global.h"
#include <GMLIB/Mod/Addons/VanillaFix.h>

namespace GMLIB::Mod {

LL_TYPE_INSTANCE_HOOK(UndeadMobFix, HookPriority::Highest, Actor, "?isInvertedHealAndHarm@Actor@@QEBA_NXZ", bool) {
    return this->hasFamily("undead");
}

struct Undead_Impl {
    ll::memory::HookRegistrar<UndeadMobFix> r;
};

std::unique_ptr<Undead_Impl> impl;

void VanillaFix::setFixCustomUndeadMobsEnabled() {
    if (!impl) {
        impl = std::make_unique<Undead_Impl>();
    }
}

} // namespace GMLIB::Mod