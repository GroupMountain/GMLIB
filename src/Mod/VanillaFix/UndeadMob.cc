#include "GMLIB/Mod/VanillaFix.h"
#include "Global.h"

namespace GMLIB::Mod {

bool mEnableUndeadFix = false;

void VanillaFix::setFixCustomUndeadMobs(bool value) { mEnableUndeadFix = value; }

LL_AUTO_TYPE_INSTANCE_HOOK(UndeadMobFix, HookPriority::Highest, Actor, "?isInvertedHealAndHarm@Actor@@QEBA_NXZ", bool) {
    return mEnableUndeadFix ? this->hasFamily("undead") : origin();
}

} // namespace GMLIB::Mod