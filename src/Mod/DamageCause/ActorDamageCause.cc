#include "Global.h"
#include <GMLIB/Mod/CustomDamageCause.h>
#include <mc/world/actor/ActorDamageByActorSource.h>
#include <mc/world/actor/ActorDamageByBlockSource.h>
#include <mc/world/actor/ActorDamageByChildActorSource.h>
#include <mc/world/actor/EnderCrystal.h>

ll::schedule::GameTickScheduler scheduler;

namespace GMLIB::Mod {

using DEATH_MESSAGE = std::pair<std::string, std::vector<std::string>>;
using namespace ll::chrono_literals;

std::unordered_map<int64, int>           mFallHeightMap;
std::unordered_map<int64, ActorUniqueID> mHurtByEntityMap;
bool                                     mDamageCauseDefinitionEnabled = false;
int                                      mMaxCauseId                   = 34;
std::vector<std::pair<std::string, int>> mCustomCauseMap;

bool isCrystal = false;

bool isCustomCauseExist(std::string& causeName) {
    for (auto& key : mCustomCauseMap) {
        if (key.first == causeName) {
            return true;
        }
    }
    return false;
}

bool isCustomCauseExist(int cause) {
    for (auto& key : mCustomCauseMap) {
        if (key.second == cause) {
            return true;
        }
    }
    return false;
}

::ActorDamageCause getCustomCause(std::string& causeName) {
    for (auto& key : mCustomCauseMap) {
        if (key.first == causeName) {
            return (ActorDamageCause)key.second;
        }
    }
    return ActorDamageCause::None;
}

std::string getCustomCause(::ActorDamageCause cause) {
    for (auto& key : mCustomCauseMap) {
        if ((ActorDamageCause)key.second == cause) {
            return key.first;
        }
    }
    return "none";
}

::ActorDamageCause DamageCause::getCauseFromName(std::string& causeName) {
    auto cause = ActorDamageSource::lookupCause(causeName);
    if (cause != ActorDamageCause::None) {
        return cause;
    }
    return getCustomCause(causeName);
}

Actor* getDamagingEntity(ActorDamageSource* ads) {
    auto id = ads->getDamagingEntityUniqueID();
    return ll::service::getLevel()->fetchEntity(id);
}

// %entity.warden.name
std::string getResourcePackKey(std::string name, Actor* ac) {
    if (ac->hasCategory(ActorCategory::Player)) {
        return name;
    }
    auto type = ac->getTypeName();
    ll::utils::string_utils::replaceAll(type, "minecraft:", "");
    std::string res = "%entity." + type + ".name";
    return res;
}

// 构造死亡信息输出
DEATH_MESSAGE makeDeathMessage(
    int            cause,
    DEATH_MESSAGE& deathMessage,
    std::string&   name,
    Actor*         killer,
    std::string&   weaponName,
    bool           isEscaping,
    bool           isHardCodedMessage = false
) {
    // 自定义类型构造
    if (cause >= 35 || isHardCodedMessage) {
        std::string msg = "death.attack.damageCause.item";
        ll::utils::string_utils::replaceAll(msg, "damageCause", getCustomCause(ActorDamageCause(cause)));
        if (isHardCodedMessage) {
            msg = deathMessage.first;
        }
        // 没有武器名不使用.item结尾
        if (weaponName.empty()) {
            // 试图逃离 xx 使用.player
            if (killer && isEscaping) {
                ll::utils::string_utils::replaceAll(msg, ".item", ".player");
            } else {
                ll::utils::string_utils::replaceAll(msg, ".item", "");
            }
        }
        DEATH_MESSAGE res = {msg, {name}};
        if (killer) {
            if (killer->hasCategory(ActorCategory::Player)) {
                res.second.push_back(((Player*)killer)->getName());
            } else {
                res.second.push_back(getResourcePackKey(name, killer));
            }
            auto weaponname = killer->getCarriedItem().getCustomName();
            if (!weaponname.empty()) {
                weaponname = "[" + weaponname + "§r]";
                res.second.push_back(weaponname);
            }
        } else {
            // 如果没有击杀者和试图逃离，那么不可能使用 .player 结尾
            ll::utils::string_utils::replaceAll(res.first, ".player", "");
        }
        return res;
    }
    // 未定义返回vanilla值
    return deathMessage;
}

DEATH_MESSAGE tryTranslateHardCodedFallingDeathMessage(
    DEATH_MESSAGE& deathMessage,
    std::string&   name,
    Actor*         killer,
    std::string&   directKiller,
    std::string&   weaponName,
    bool           isEscaping,
    int64          deadUniqueId
) {
    if (!directKiller.empty()) {
        deathMessage.first = "death.fell.finish.item";
    }
    // 低处
    else if (deathMessage.first == "death.attack.fall") {
        deathMessage.first = "death.attack.fall.item";
    }
    // 高处
    else if (deathMessage.first == "death.fell.accident.generic") {
        // 试图逃离而亡
        if (killer) {
            std::string weapon = killer->getCarriedItem().getCustomName();
            if (!weapon.empty()) {
                deathMessage.first = "death.fell.assist.item";
            } else {
                deathMessage.first = "death.fell.assist";
            }
        }
        // 超高
        else if (mFallHeightMap[deadUniqueId] >= 100) {
            deathMessage.first = "death.fell.killer";
        } else {
            deathMessage.first = "death.fell.accident.generic";
        }
    } else {
        deathMessage.first = "death.attack.generic";
    }
    return makeDeathMessage((int)ActorDamageCause::Fall, deathMessage, name, killer, weaponName, isEscaping, true);
}

std::optional<DEATH_MESSAGE> tryTranslateHardCodedDeathMessage(
    ActorDamageCause cause,
    DEATH_MESSAGE&   deathMessage,
    std::string&     name,
    Actor*           killer,
    std::string&     directKiller,
    std::string&     weaponName,
    bool             isEscaping,
    int64            deadUniqueId
) {
    switch (cause) {
    // 硬编码摔死信息
    case ActorDamageCause::Fall: {
        return tryTranslateHardCodedFallingDeathMessage(
            deathMessage,
            name,
            killer,
            directKiller,
            weaponName,
            isEscaping,
            deadUniqueId
        );
    }
    // 硬编码弹射物信息
    case ActorDamageCause::Projectile: {
        switch (ll::hash_utils::do_hash(directKiller)) {
        case ll::hash_utils::do_hash("minecraft:small_fireball"):
            deathMessage.first = "death.attack.fireball.item";
            break;
        case ll::hash_utils::do_hash("minecraft:thrown_trident"):
            deathMessage.first = "death.attack.trident.item";
            break;
        case ll::hash_utils::do_hash("minecraft:llama_spit"):
            deathMessage.first = "death.attack.spit";
            break;
        case ll::hash_utils::do_hash("minecraft:shulker_bullet"):
            deathMessage.first = "death.attack.bullet";
            break;
        case ll::hash_utils::do_hash("minecraft:arrow"):
            deathMessage.first = "death.attack.arrow.item";
            break;
        default:
            deathMessage.first = "death.attack.thrown.item";
            break;
        }
        return makeDeathMessage((int)cause, deathMessage, name, killer, weaponName, isEscaping, true);
    }
    // 硬编码直接击杀信息
    case ActorDamageCause::EntityAttack: {
        switch (ll::hash_utils::do_hash(directKiller)) {
        case ll::hash_utils::do_hash("minecraft:player"):
            deathMessage.first = "death.attack.player.item";
            break;
        case ll::hash_utils::do_hash("minecraft:bee"):
            deathMessage.first = "death.attack.sting.item";
            break;
        default:
            deathMessage.first = "death.attack.mob.item";
            break;
        }
        return makeDeathMessage((int)cause, deathMessage, name, killer, weaponName, isEscaping, true);
    }
    // 硬编码接触死亡信息
    case ActorDamageCause::Contact: {
        switch (ll::hash_utils::do_hash(deathMessage.first)) {
        case ll::hash_utils::do_hash("death.attack.cactus"):
            deathMessage.first = "death.attack.cactus.item";
            break;
        case ll::hash_utils::do_hash("death.attack.sweetBerry"):
            deathMessage.first = "death.attack.sweetBerry.item";
            break;
        default:
            deathMessage.first = "death.attack.generic.item";
            break;
        }
        return makeDeathMessage((int)cause, deathMessage, name, killer, weaponName, isEscaping, true);
    }
    // 硬编码爆炸信息
    case ActorDamageCause::BlockExplosion: {
        if (!isCrystal) {
            deathMessage.first = "death.attack.badRespawnPoint";
        } else {
            deathMessage.first = "death.attack.explosion.item";
        }
        return makeDeathMessage((int)cause, deathMessage, name, killer, weaponName, isEscaping, true);
    }
    case ActorDamageCause::EntityExplosion: {
        switch (ll::hash_utils::do_hash(directKiller)) {
        case ll::hash_utils::do_hash("minecraft:wither_skull"):
        case ll::hash_utils::do_hash("minecraft:wither_skull_dangerous"):
            deathMessage.first = "death.attack.witherSkull.item";
            break;
        default: {
            deathMessage.first = "death.attack.explosion.item";
            break;
        }
        }
        return makeDeathMessage((int)cause, deathMessage, name, killer, weaponName, isEscaping, true);
    }
    // Magic
    case ActorDamageCause::Magic: {
        if (killer) {
            deathMessage.first = "death.attack.indirectMagic.item";
        } else {
            deathMessage.first = "death.attack.magic";
        }
        return makeDeathMessage((int)cause, deathMessage, name, killer, weaponName, isEscaping, true);
    }
    // SonicBoom
    case ActorDamageCause::All: {
        if (directKiller == "minecraft:warden") {
            deathMessage.first = "death.attack.sonicBoom";
        } else {
            deathMessage.first = "death.attack.sonicBoom.item";
        }
        return makeDeathMessage((int)cause, deathMessage, name, killer, weaponName, isEscaping, true);
    }
    // Thorns
    case ActorDamageCause::Thorns: {
        if (directKiller.empty()) {
            deathMessage.first = "death.attack.generic";
        } else {
            deathMessage.first = "death.attack.thorns.item";
        }
        return makeDeathMessage((int)cause, deathMessage, name, killer, weaponName, isEscaping, true);
    }
    }
    // 非硬编码死亡信息
    return {};
}

DEATH_MESSAGE translateDeathMessage(DEATH_MESSAGE origin, std::string name, Actor* dead, ActorDamageSource* ads) {
    name = getResourcePackKey(name, dead);
    if (origin.first.length() > 65565) {
        return {"death.attack.even_more_magic", {name}};
    }
    auto        damageCause = ads->getCause(); // 伤害类型
    Actor*      killer      = nullptr;         // 击杀者
    std::string directKiller;                  // 直接击杀实体
    std::string weaponName;                    // 是否持有武器
    bool        isEscaping = false;
    int64       uid        = dead->getOrCreateUniqueID().id;
    if (ads->isEntitySource()) {
        directKiller = getDamagingEntity(ads)->getTypeName();
        if (ads->isChildEntitySource()) {
            killer = getDamagingEntity(ads)->getOwner();
        } else {
            killer = getDamagingEntity(ads);
        }
    }
    if (killer == nullptr && mHurtByEntityMap.count(uid)) {
        killer     = ll::service::getLevel()->fetchEntity(mHurtByEntityMap[uid]);
        isEscaping = true;
    }
    if (killer != nullptr) {
        weaponName = killer->getCarriedItem().getCustomName();
    }
    // Vanilla的硬编码死亡信息类型
    auto result =
        tryTranslateHardCodedDeathMessage(damageCause, origin, name, killer, directKiller, weaponName, isEscaping, uid);
    if (result.has_value()) {
        return result.value();
    }
    return makeDeathMessage((int)damageCause, origin, name, killer, weaponName, isEscaping);
}

LL_TYPE_INSTANCE_HOOK(
    MobDieHook,
    HookPriority::Normal,
    Mob,
    "?die@Mob@@UEAAXAEBVActorDamageSource@@@Z",
    void,
    ActorDamageSource* ads
) {
    auto uid = getOrCreateUniqueID();
    if (hasCategory(ActorCategory::Player) || isTame()) {
        if (getLastHurtByMobTime() != 0) {
            auto hm                  = getLastHurtByMob();
            mHurtByEntityMap[uid.id] = hm->getOrCreateUniqueID();
        }
        if (ads->getCause() == ActorDamageCause::Fall) {
            mFallHeightMap[uid.id] = getFallDistance();
        }
    }
    origin(ads);
    scheduler.add<ll::schedule::DelayTask>(1_tick, [&] {
        auto pos1 = mHurtByEntityMap.find(uid.id);
        mHurtByEntityMap.clear();
        mFallHeightMap.clear();
    });
}

LL_TYPE_INSTANCE_HOOK(
    CryatslHurtHook,
    HookPriority::Normal,
    EnderCrystal,
    "?_hurt@EnderCrystal@@MEAA_NAEBVActorDamageSource@@M_N1@Z",
    bool,
    ActorDamageSource& a1,
    float              a2,
    bool               a3,
    bool               a4
) {
    auto res  = origin(a1, a2, a3, a4);
    isCrystal = true;
    scheduler.add<ll::schedule::DelayTask>(1_tick, [] { isCrystal = false; });
    return res;
}

LL_TYPE_INSTANCE_HOOK(
    DeathMessageHook1,
    HookPriority::Lowest,
    ActorDamageSource,
    "?getDeathMessage@ActorDamageSource@@UEBA?AU?$pair@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?"
    "$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_"
    "traits@D@std@@V?$allocator@D@2@@std@@@2@@2@@std@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@3@"
    "PEAVActor@@@Z",
    DEATH_MESSAGE,
    std::string a1,
    Actor*      a2
) {
    auto originl = origin(a1, a2);
    auto res     = translateDeathMessage(originl, a1, a2, this);
    return res;
}

LL_TYPE_INSTANCE_HOOK(
    DeathMessageHook2,
    HookPriority::Lowest,
    ActorDamageByActorSource,
    "?getDeathMessage@ActorDamageByActorSource@@UEBA?AU?$pair@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@"
    "std@@V?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$"
    "char_traits@D@std@@V?$allocator@D@2@@std@@@2@@2@@std@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@3@"
    "PEAVActor@@@Z",
    DEATH_MESSAGE,
    std::string a1,
    Actor*      a2
) {
    auto originl = origin(a1, a2);
    auto res     = translateDeathMessage(originl, a1, a2, this);
    return res;
}

LL_TYPE_INSTANCE_HOOK(
    DeathMessageHook3,
    HookPriority::Lowest,
    ActorDamageByBlockSource,
    "?getDeathMessage@ActorDamageByBlockSource@@UEBA?AU?$pair@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@"
    "std@@V?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$"
    "char_traits@D@std@@V?$allocator@D@2@@std@@@2@@2@@std@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@3@"
    "PEAVActor@@@Z",
    DEATH_MESSAGE,
    std::string a1,
    Actor*      a2
) {
    auto originl = origin(a1, a2);
    auto res     = translateDeathMessage(originl, a1, a2, this);
    return res;
}

LL_TYPE_INSTANCE_HOOK(
    DeathMessageHook4,
    HookPriority::Lowest,
    ActorDamageByChildActorSource,
    "?getDeathMessage@ActorDamageByChildActorSource@@UEBA?AU?$pair@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@"
    "D@2@@std@@V?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?"
    "$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@2@@std@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@3@"
    "PEAVActor@@@Z",
    DEATH_MESSAGE,
    std::string a1,
    Actor*      a2
) {
    auto originl = origin(a1, a2);
    auto res     = translateDeathMessage(originl, a1, a2, this);
    return res;
}

void DamageCause::setCustomDamageCauseEnabled() {
    if (!mDamageCauseDefinitionEnabled) {
        ll::memory::HookRegistrar<DeathMessageHook1>().hook();
        ll::memory::HookRegistrar<DeathMessageHook2>().hook();
        ll::memory::HookRegistrar<DeathMessageHook3>().hook();
        ll::memory::HookRegistrar<DeathMessageHook4>().hook();
        ll::memory::HookRegistrar<MobDieHook>().hook();
        ll::memory::HookRegistrar<CryatslHurtHook>().hook();
        mDamageCauseDefinitionEnabled = true;
    }
}

int getNextCauseId() {
    mMaxCauseId++;
    return mMaxCauseId;
}

bool DamageCause::registerDamageCause(std::string causeName) {
    if (!isCustomCauseExist(causeName)) {
        setCustomDamageCauseEnabled();
        auto id = getNextCauseId();
        mCustomCauseMap.push_back({causeName, id});
        return true;
    }
    return false;
}

} // namespace GMLIB::Mod