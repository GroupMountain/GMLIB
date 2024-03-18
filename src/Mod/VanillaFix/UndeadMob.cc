#include "Global.h"
#include <GMLIB/Mod/Addons/VanillaFix.h>

namespace GMLIB::Mod {

bool mEnableUndeadFix = false;

LL_TYPE_INSTANCE_HOOK(UndeadMobFix, HookPriority::Highest, Actor, "?isInvertedHealAndHarm@Actor@@QEBA_NXZ", bool) {
    return mEnableUndeadFix ? this->hasFamily("undead") : origin();
}

void VanillaFix::setFixCustomUndeadMobsEnabled() {
    if (!mEnableUndeadFix) {
        ll::memory::HookRegistrar<UndeadMobFix>().hook();
        mEnableUndeadFix = true;
    }
}

} // namespace GMLIB::Mod