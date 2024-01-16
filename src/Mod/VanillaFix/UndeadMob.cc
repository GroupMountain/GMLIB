#include <GMLIB/Mod/VanillaFix.h>
#include "Global.h"

bool mEnableUndeadFix = false;

void GMLIB_VanillaFix::setFixCustomUndeadMobs(bool value = true) {
    mEnableUndeadFix = value;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    UndeadMobFix,
    HookPriority::Highest,
    Actor,  
    "?isInvertedHealAndHarm@Actor@@UEBA_NXZ",
    bool) 
{
    return mEnableUndeadFix ? this->hasFamily("undead") : origin();
}