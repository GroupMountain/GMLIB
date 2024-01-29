#include "Global.h"
#include <GMLIB/Mod/CustomDamageCause.h>
/*

using DEATH_MESSAGE = std::pair<std::string, std::vector<std::string>>;

nlohmann::json DamageCauseDefinition = {};

bool          EnableDamageDef = false;
float         fallHeight      = -1;
bool          isCrystal       = false;
ActorUniqueID uid             = ActorUniqueID(-1);
bool          shouldDeathLog  = false;
bool          fixWeaponName   = false;

// 构造资源包实体键名
// %entity.warden.name
std::string getResourcePackName(std::string type) {
    ll::utils::string_utils::replaceAll(type, "minecraft:", "");
    std::string res = "%entity." + type + ".name";
    return res;
}

// 修复死亡信息无法正常翻译部分实体
// 修复显示 %entity.unknown.name （未知）
std::string fixActorName(std::string name, Actor* ac) {
    if (name == "%entity.unknown.name") {
        return getResourcePackName(ac->getTypeName());
    }
    return name;
}

// 构造死亡信息输出
DEATH_MESSAGE makeDeathMessage(std::string msg, std::string name, Actor* killer, std::string weapon, bool escape) {
    // 没有武器名不使用.item结尾
    if (weapon == "") {
        // 试图逃离 xx 使用.player
        if (killer && escape) {
            ll::utils::string_utils::replaceAll(msg, ".item", ".player");
        } else {
            ll::utils::string_utils::replaceAll(msg, ".item", "");
        }
    }
    DEATH_MESSAGE res = {msg, {name}};
    if (killer) {
        if (killer->isPlayer()) {
            res.second.push_back(((Player*)killer)->getName());
        } else {
            res.second.push_back(getResourcePackName(killer->getTypeName()));
        }
        auto weaponname = killer->getHandSlot()->getCustomName();
        if (weaponname != "") {
            if (fixWeaponName == true) {
                weaponname = "[" + weaponname + "§r]";
            }
            res.second.push_back(weaponname);
        }
    } else {
        // 如果没有击杀者和试图逃离，那么不可能使用 .player 结尾
        ll::utils::string_utils::replaceAll(res.first, ".player", "");
    }
    return res;
}

// 生成原始字符串
std::string getMessage(std::string cause, std::string direct_killer, std::string die) {
    std::string msg = "death.attack.damageCause.item";
    // 定义了字符串
    if (DamageCauseDefinition[cause][direct_killer][die].contains("msg")) {
        return DamageCauseDefinition[cause][direct_killer][die]["msg"].get<std::string>();
    }
    // 获取Cause类型
    auto custom_cause = DamageCauseDefinition[cause][direct_killer][die]["cause"].get<std::string>();
    // 正常翻译
    ll::utils::string_utils::replaceAll(msg, "damageCause", custom_cause);
    return msg;
}

// 构造原始字符串
// 如death.attack
std::string makeOriginalMessage(std::string cause, std::string direct_killer, std::string die, std::string original) {
    // VanillaCause有定义
    if (DamageCauseDefinition.contains(cause)) {
        // 存在且定义了直接击杀实体
        if (DamageCauseDefinition[cause].contains(direct_killer)) {
            // 如果定义了死者
            if (DamageCauseDefinition[cause][direct_killer].contains(die)) {
                return getMessage(cause, direct_killer, die);
            }
            // 未定义死者
            return getMessage(cause, direct_killer, "default");
        } else if (DamageCauseDefinition[cause].contains("default")) {
            // 如果定义了死者
            if (DamageCauseDefinition[cause]["default"].contains(die)) {
                return getMessage(cause, "default", die);
            }
            // 未定义死者
            return getMessage(cause, "default", "default");
        }
        // 未定义全局
        // 不修改
        return original;
    }
    // VanillaCause无定义
    // 不修改
    return original;
}

// 构造原始摔死信息
std::string makeOriginalFallMessage(
    std::string cause,
    std::string direct_killer,
    Actor*      killer,
    std::string die,
    std::string original
) {
    // 有死亡源伤害
    if (direct_killer != "null") {
        return makeOriginalMessage(cause, killer ? killer->getTypeName() : "default", die, original);
    }
    // 正常摔死
    // 低处
    else if (original == "death.attack.fall") {
        return makeOriginalMessage(cause, "low", die, original);
    }
    // 高处
    else if (original == "death.fell.accident.generic") {
        // 试图逃离而亡
        if (killer != nullptr) {
            std::string weapon = killer->getCarriedItem().getCustomName();
            if (weapon != "") {
                return makeOriginalMessage(cause, "escaping_high_weapon", die, original);
            }
            return makeOriginalMessage(cause, "escaping_high", die, original);
        }
        // 正常摔死
        // 超高
        if (fallHeight >= 100) {
            return makeOriginalMessage(cause, "high", die, original);
        }
        return makeOriginalMessage(cause, "mid", die, original);
    } else {
        return makeOriginalMessage(cause, "null", die, original);
    }
}

DEATH_MESSAGE translateDeathMessage(DEATH_MESSAGE origin, std::string name, Actor* ac, ActorDamageSource* ads) {
    name = fixActorName(name, ac);
    if (origin.first.length() > 65565) {
        return {"death.attack.even_more_magic", {name}};
    }
    // 伤害类型
    auto        VanillaCause  = S((int)(ads->getCause()));
    Actor*      killer        = nullptr; // 击杀者
    std::string direct_killer = "null";  // 直接击杀实体（弹射物等）
    std::string weapon        = "";      // 持有武器
    bool        escape        = false;
    if (ads->isEntitySource()) {
        direct_killer = ads->getEntity()->getTypeName();
        if (ads->isChildEntitySource()) {
            killer = ads->getEntity()->getOwner();
        } else {
            killer = ads->getEntity();
        }
    }
    if (killer == nullptr && uid != -1) {
        killer = Global<Level>->getEntity(uid);
        escape = true;
    }
    if (killer != nullptr) {
        weapon = killer->getHandSlot()->getCustomName();
    }
    // 已构造基本信息（name/ac 死者， killer 击杀/试图逃离， weapon 武器名）
    auto        vamsg = origin.first;
    auto        die   = ac->getTypeName();
    std::string fdmsg = origin.first;
    if (isCrystal && ads->getCause() == ActorDamageCause::BlockExplosion) {
        fdmsg = makeOriginalMessage(VanillaCause, "crystal", die, vamsg);
    } else if (ads->getCause() == ActorDamageCause::Fall) {
        fdmsg = makeOriginalFallMessage(VanillaCause, direct_killer, killer, die, vamsg);
    } else if (vamsg == "death.attack.cactus" && direct_killer == "null") {
        fdmsg = makeOriginalMessage(VanillaCause, "cactus", die, vamsg);
    } else if (vamsg == "death.attack.sweetBerry" && direct_killer == "null") {
        fdmsg = makeOriginalMessage(VanillaCause, "sweetBerry", die, vamsg);
    } else {
        fdmsg = makeOriginalMessage(VanillaCause, direct_killer, die, vamsg);
    }
    return makeDeathMessage(fdmsg, name, killer, weapon, escape);
}

void GMLib_Mod::addDamageCause(
    ActorDamageCause vanilla_cause,
    std::string      cause_name,
    std::string      direct_killer_type,
    std::string      die_type
) {
    EnableDamageDef   = true;
    auto VanillaCause = S((int)(vanilla_cause));
    if (DamageCauseDefinition[VanillaCause][direct_killer_type][die_type].contains("cause")) {
        Logger("Server [Damage Cause]").error("Damage Cause '{}' is already exist!", cause_name);
        return;
    }
    DamageCauseDefinition[VanillaCause][direct_killer_type][die_type]["cause"] = cause_name;
}

void GMLib_Mod::addDamageCauseWithMessage(
    ActorDamageCause vanilla_cause,
    std::string      cause_name,
    std::string      custom_message,
    std::string      direct_killer_type,
    std::string      die_type
) {
    EnableDamageDef   = true;
    auto VanillaCause = S((int)(vanilla_cause));
    if (DamageCauseDefinition[VanillaCause][direct_killer_type][die_type].contains("cause")) {
        Logger("Server [Damage Cause]").error("Damage Cause '{}' is already exist!", cause_name);
        return;
    }
    DamageCauseDefinition[VanillaCause][direct_killer_type][die_type]["cause"] = cause_name;
    DamageCauseDefinition[VanillaCause][direct_killer_type][die_type]["msg"]   = custom_message;
}

void GMLib_Mod::setFixWeaponName() {
    EnableDamageDef = true;
    fixWeaponName   = true;
}

void GMLib_Mod::setEnableDeathLog(bool logToFile) {
    EnableDamageDef = true;
    shouldDeathLog  = true;
    if (logToFile == true) {
        deathlogger.setFile("logs/DeathLog.log", true);
        if (!std::filesystem::exists("logs/DeathLog.log")) {
            WriteAllFile("logs/DeathLog.log", "//Death Log");
        }
    }
}

void deathLog(DEATH_MESSAGE msg) {
    if (shouldDeathLog == true) {
        auto str = I18n::get(msg.first, msg.second);
        while (str.find("§") != std::string::npos) {
            str = str.replace(str.find("§"), 3, "");
        }
        deathlogger.info("{}", str);
    }
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    MobDie,
    HookPriority::Normal,
    Mob,
    "?die@Mob@@UEAAXAEBVActorDamageSource@@@Z",
    void,
    ActorDamageSource* ads
) {
    if (isPlayer() || isTame()) {
        if (getLastHurtByMobTime() != 0) {
            auto hm = getLastHurtByMob();
            uid     = hm->getOrCreateUniqueID();
        }
        if (ads->getCause() == ActorDamageCause::Fall) {
            fallHeight = getFallDistance();
        }
    }
    return origin(ads);
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    CryatslHurt,
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
    // Schedule::nextTick([]() { isCrystal = false; });
    return res;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    Death1,
    HookPriority::Normal,
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
    deathLog(res);
    fallHeight = -1;
    uid        = ActorUniqueID(-1);
    return res;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    Death2,
    HookPriority::Normal,
    ActorDamageSource,
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
    deathLog(res);
    fallHeight = -1;
    uid        = ActorUniqueID(-1);
    return res;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    Death3,
    HookPriority::Normal,
    ActorDamageSource,
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
    deathLog(res);
    fallHeight = -1;
    uid        = ActorUniqueID(-1);
    return res;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    Death4,
    HookPriority::Normal,
    ActorDamageSource,
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
    deathLog(res);
    fallHeight = -1;
    uid        = ActorUniqueID(-1);
    return res;
}
*/