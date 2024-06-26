#include "Global.h"
#include <GMLIB/Mod/Addons/CustomDamageCause.h>
#include <mc/world/actor/ActorDamageByActorSource.h>
#include <mc/world/actor/ActorDamageByBlockSource.h>
#include <mc/world/actor/ActorDamageByChildActorSource.h>
#include <mc/world/actor/EnderCrystal.h>

ll::schedule::GameTickScheduler scheduler;

namespace GMLIB::Mod {

using DEATH_MESSAGE = std::pair<std::string, std::vector<std::string>>;
using namespace ll::chrono_literals;

phmap::flat_hash_map<int64, int>            mFallHeightMap;
phmap::flat_hash_map<int64, ActorUniqueID>  mHurtByEntityMap;
int                                         mMaxCauseId = 34;
std::vector<std::pair<std::string, int>>    mCustomCauseMap;
phmap::flat_hash_map<int, std::string_view> mVanillaCauseMessage;

phmap::flat_hash_map<ActorDamageCause, phmap::flat_hash_map<std::string_view, std::string_view>> mHardCodedDeathMessage;

bool isCrystal = false;

bool isCustomDefinition(int cause) {
    if (cause >= 35) {
        return true;
    }
    if (mVanillaCauseMessage.count(cause)) {
        return true;
    }
    return false;
}

bool isCustomCauseExist(std::string const& causeName) {
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

::ActorDamageCause getCustomCause(std::string const& causeName) {
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

std::string getCause(::ActorDamageCause cause) {
    auto id = (int)cause;
    if (isCustomCauseExist(id)) {
        return getCustomCause(cause);
    }
    if (mVanillaCauseMessage.count(id)) {
        return std::string(mVanillaCauseMessage[id]);
    }
    return "";
}

::ActorDamageCause DamageCause::getCauseFromName(std::string const& causeName) {
    auto cause = getCustomCause(causeName);
    if (cause != ActorDamageCause::None) {
        return cause;
    }
    return ActorDamageSource::lookupCause(causeName);
}

Actor* getDamagingEntity(ActorDamageSource* ads) {
    auto id = ads->getDamagingEntityUniqueID();
    return ll::service::getLevel()->fetchEntity(id);
}

// %entity.warden.name
std::string getResourcePackKey(std::string const& name, Actor* ac) {
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
    if (isCustomDefinition(cause) || isHardCodedMessage) {
        std::string msg = "death.attack.damageCause.item";
        ll::utils::string_utils::replaceAll(msg, "damageCause", getCause(ActorDamageCause(cause)));
        if (isHardCodedMessage) {
            msg = deathMessage.first;
        }
        // 没有武器名不使用.item结尾
        if (weaponName.empty()) {
            // 试图逃离 xx 使用.player
            if (killer) {
                if (isEscaping) {
                    ll::utils::string_utils::replaceAll(msg, ".item", ".player");
                }
            } else {
                ll::utils::string_utils::replaceAll(msg, ".item", "");
            }
        }
        DEATH_MESSAGE res = {msg, {name}};
        if (killer) {
            if (killer->isPlayer()) {
                res.second.push_back(((Player*)killer)->getName());
            } else {
                res.second.push_back(getResourcePackKey(name, killer));
            }
            auto weaponname = killer->getCarriedItem().getCustomName();
            if (!weaponname.empty()) {
                weaponname = "[" + weaponname + "§r]";
                res.second.push_back(weaponname);
            } else {
                ll::utils::string_utils::replaceAll(res.first, ".item", "");
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
    // 硬编码爆炸信息
    case ActorDamageCause::BlockExplosion: {
        if (!isCrystal) {
            deathMessage.first = "death.attack.badRespawnPoint.message";
        } else {
            deathMessage.first = "death.attack.explosion.item";
        }
        return makeDeathMessage((int)cause, deathMessage, name, killer, weaponName, isEscaping, true);
    }
    case ActorDamageCause::Contact: {
        switch (ll::hash_utils::doHash(deathMessage.first)) {
        case ll::hash_utils::doHash("death.attack.cactus"):
            deathMessage.first = "death.attack.cactus.item";
            break;
        case ll::hash_utils::doHash("death.attack.sweetBerry"):
            deathMessage.first = "death.attack.sweetBerry.item";
            break;
        default:
            deathMessage.first = "death.attack.generic.item";
            break;
        }
        return makeDeathMessage((int)cause, deathMessage, name, killer, weaponName, isEscaping, true);
    }
    }
    if (mHardCodedDeathMessage.contains(cause)) {
        if (directKiller.empty()) {
            directKiller = "empty";
        }
        if (mHardCodedDeathMessage[cause].contains(directKiller)) {
            deathMessage.first = std::string(mHardCodedDeathMessage[cause][directKiller]);
            return makeDeathMessage((int)cause, deathMessage, name, killer, weaponName, isEscaping, true);
        } else if (mHardCodedDeathMessage[cause].contains("all")) {
            deathMessage.first = std::string(mHardCodedDeathMessage[cause]["all"]);
            return makeDeathMessage((int)cause, deathMessage, name, killer, weaponName, isEscaping, true);
        }
    }
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

struct DamageCause_Impl {
    ll::memory::HookRegistrar<
        DeathMessageHook1,
        DeathMessageHook2,
        DeathMessageHook3,
        DeathMessageHook4,
        MobDieHook,
        CryatslHurtHook>
        r;
};

std::unique_ptr<DamageCause_Impl> impl;

void DamageCause::setCustomDamageCauseEnabled() {
    if (!impl) {
        impl = std::make_unique<DamageCause_Impl>();
    }
}

int getNextCauseId() {
    mMaxCauseId++;
    return mMaxCauseId;
}

bool DamageCause::registerDamageCause(std::string const& causeName) {
    if (!isCustomCauseExist(causeName)) {
        setCustomDamageCauseEnabled();
        auto id = getNextCauseId();
        mCustomCauseMap.push_back({causeName, id});
        return true;
    }
    return false;
}

bool DamageCause::setVanillaCauseMessage(ActorDamageCause cause, std::string_view msg) {
    auto id = (int)cause;
    if (!mVanillaCauseMessage.count(id)) {
        mVanillaCauseMessage[id] = msg;
        return true;
    }
    return false;
}

bool DamageCause::registerCustomDeathMessage(
    ActorDamageCause cause,
    std::string_view message,
    std::string_view killerType
) {
    if (mHardCodedDeathMessage.contains(cause)) {
        if (mHardCodedDeathMessage[cause].contains(killerType)) {
            return false;
        }
    }
    mHardCodedDeathMessage[cause][killerType] = message;
    return true;
}

} // namespace GMLIB::Mod