#include "GMLIB/Server/LevelAPI.h"
#include "Global.h"

namespace GMLIB::LevelAPI {

bool                          mFakeLevelNameEnabled     = false;
std::string                   mFakeLevelName            = "";
bool                          mFakeSeedEnabled          = false;
int64_t                       mFakeSeed                 = 0;
bool                          mForceTrustSkin           = false;
bool                          mCoResourcePack           = false;
std::vector<::AllExperiments> mExperimentsRequireList   = {};
bool                          mForceAchievementsEnabled = false;
bool                          mRegAbilityCommand        = false;
bool                          mEducationEditionEnabled  = false;

GMLIB_API BlockSource* getBlockSource(DimensionType dimid) {
    return &ll::service::getLevel()->getDimension(dimid)->getBlockSourceFromMainChunkSource();
}

GMLIB_API std::vector<Actor*> getAllEntities() { return ll::service::getLevel()->getRuntimeActorList(); }

GMLIB_API std::string getLevelName() { return ll::service::getLevel()->getLevelData().getLevelName(); }

GMLIB_API void setLevelName(std::string newName) { ll::service::getLevel()->getLevelData().setLevelName(newName); }

GMLIB_API void setFakeLevelName(std::string fakeName) {
    mFakeLevelNameEnabled = true;
    mFakeLevelName        = fakeName;
}

GMLIB_API int64_t getSeed() { return ll::service::getLevel()->getLevelData().getSeed().mValue; }

GMLIB_API void setFakeSeed(int64_t fakeSeed) {
    mFakeSeedEnabled = true;
    mFakeSeed        = fakeSeed;
}

GMLIB_API void setCoResourcePack(bool enabled) { mCoResourcePack = enabled; }

GMLIB_API void setForceTrustSkin(bool enabled) { mForceTrustSkin = enabled; }

GMLIB_API bool getExperimentEnabled(::AllExperiments experiment) {
    return ll::service::getLevel()->getLevelData().getExperiments().isExperimentEnabled(experiment);
}

GMLIB_API void setExperimentEnabled(::AllExperiments experiment, bool enabled) {
    ll::service::getLevel()->getLevelData().getExperiments().setExperimentEnabled(experiment, enabled);
}

GMLIB_API void addExperimentsRequire(::AllExperiments experiment) {
    if (std::find(mExperimentsRequireList.begin(), mExperimentsRequireList.end(), experiment)
        == mExperimentsRequireList.end()) {
        mExperimentsRequireList.push_back(experiment);
    }
}

GMLIB_API void setForceAchievementsEnabled() { mForceAchievementsEnabled = true; }

GMLIB_API void forceEnableAbilityCommand() { mRegAbilityCommand = true; }

GMLIB_API void addEducationEditionRequired() { mEducationEditionEnabled = true; }

GMLIB_API int getTime(int time) { return ll::service::getLevel()->getTime(); }

GMLIB_API void setTime(int time) {
    ll::service::getLevel()->setTime(time);
    try {
        SetTimePacket(time).sendToClients();
    } catch (...) {}
}

GMLIB_API WeatherType getWeather() {
    auto data = &ll::service::getLevel()->getLevelData();
    if (data->isLightning()) {
        return WeatherType::Thunder;
    } else if (data->isRaining()) {
        return WeatherType::Rain;
    }
    return WeatherType::Clear;
}

GMLIB_API void setWeather(WeatherType weather, int lastTick) {
    switch (weather) {
    case WeatherType::Thunder: {
        ll::service::getLevel()->updateWeather(1, lastTick, 1, lastTick);
        break;
    }
    case WeatherType::Rain: {
        ll::service::getLevel()->updateWeather(1, lastTick, 0, lastTick);
        break;
    }
    default: {
        ll::service::getLevel()->updateWeather(0, lastTick, 0, lastTick);
        break;
    }
    }
}

GMLIB_API void setWeather(WeatherType weather) {
    int lastTick = 20 * (ll::service::getLevel()->getRandom().nextInt(600) + 300);
    setWeather(weather, lastTick);
}

GMLIB_API void setClientWeather(WeatherType weather, Player* pl) {
    Vec3 pos = {0, 0, 0};
    switch (weather) {
    case WeatherType::Thunder: {
        LevelEventPacket(LevelEvent::StartThunderstorm, pos, 65565).sendTo(*pl);
        LevelEventPacket(LevelEvent::StartRaining, pos, 65565).sendTo(*pl);
        break;
    }
    case WeatherType::Rain: {
        LevelEventPacket(LevelEvent::StopThunderstorm, pos, 0).sendTo(*pl);
        LevelEventPacket(LevelEvent::StartRaining, pos, 65565).sendTo(*pl);
        break;
    }
    default: {
        LevelEventPacket(LevelEvent::StopThunderstorm, pos, 0).sendTo(*pl);
        LevelEventPacket(LevelEvent::StopRaining, pos, 0).sendTo(*pl);
        break;
    }
    }
}

GMLIB_API std::optional<bool> getGameruleBool(GameRuleId id) {
    auto rule = ll::service::bedrock::getLevel()->getGameRules().getRule(id);
    if (rule) {
        if (rule->getType() == GameRule::Type::Bool) {
            return rule->getBool();
        }
    }
    return {};
}

GMLIB_API std::optional<bool> getGameruleBool(std::string name) {
    auto id = ll::service::bedrock::getLevel()->getGameRules().nameToGameRuleIndex(name);
    return getGameruleBool(id);
}

GMLIB_API std::optional<float> getGameruleFloat(GameRuleId id) {
    auto rule = ll::service::bedrock::getLevel()->getGameRules().getRule(id);
    if (rule) {
        if (rule->getType() == GameRule::Type::Float) {
            return rule->getFloat();
        }
    }
    return {};
}

GMLIB_API std::optional<float> getGameruleFloat(std::string name) {
    auto id = ll::service::bedrock::getLevel()->getGameRules().nameToGameRuleIndex(name);
    return getGameruleFloat(id);
}

GMLIB_API std::optional<int> getGameruleInt(GameRuleId id) {
    auto rule = ll::service::bedrock::getLevel()->getGameRules().getRule(id);
    if (rule) {
        if (rule->getType() == GameRule::Type::Int) {
            return rule->getInt();
        }
    }
    return {};
}

GMLIB_API std::optional<int> getGameruleInt(std::string name) {
    auto id = ll::service::bedrock::getLevel()->getGameRules().nameToGameRuleIndex(name);
    return getGameruleInt(id);
}

GMLIB_API void setGamerule(GameRuleId id, bool value) {
    auto pkt = ll::service::bedrock::getLevel()->getGameRules().setRule(id, value, true, nullptr, nullptr, nullptr);
    pkt->sendToClients();
}

GMLIB_API void setGamerule(std::string name, bool value) {
    auto id = ll::service::bedrock::getLevel()->getGameRules().nameToGameRuleIndex(name);
    return setGamerule(id, value);
}

GMLIB_API void setGamerule(GameRuleId id, float value) {
    auto pkt = ll::service::bedrock::getLevel()->getGameRules().setRule(id, value, true, nullptr, nullptr, nullptr);
    pkt->sendToClients();
}

GMLIB_API void setGamerule(std::string name, float value) {
    auto id = ll::service::bedrock::getLevel()->getGameRules().nameToGameRuleIndex(name);
    return setGamerule(id, value);
}

GMLIB_API void setGamerule(GameRuleId id, int value) {
    ll::service::bedrock::getLevel()->getGameRules();
    auto pkt = ll::service::bedrock::getLevel()->getGameRules().setRule(id, value, true, nullptr, nullptr, nullptr);
    pkt->sendToClients();
}

GMLIB_API void setGamerule(std::string name, int value) {
    auto id = ll::service::bedrock::getLevel()->getGameRules().nameToGameRuleIndex(name);
    return setGamerule(id, value);
}

GMLIB_API void createExplosion(
    const Vec3&   pos,
    DimensionType dimensionId,
    float         power,
    Actor*        source,
    bool          breakBlocks,
    bool          causeFire,
    bool          allowUnderwater,
    float         maxResistance
) {
    ll::service::bedrock::getLevel()->explode(
        *getBlockSource(dimensionId),
        source,
        pos,
        power,
        causeFire,
        breakBlocks,
        allowUnderwater,
        maxResistance
    );
}

} // namespace GMLIB::LevelAPI

LL_AUTO_INSTANCE_HOOK(
    Achieve1,
    ll::memory::HookPriority::Normal,
    "?achievementsWillBeDisabledOnLoad@LevelData@@QEBA_NXZ",
    bool
) {
    if (GMLIB::LevelAPI::mForceAchievementsEnabled) {
        return false;
    }
    return origin();
}

LL_AUTO_INSTANCE_HOOK(
    Achieve2,
    ll::memory::HookPriority::Normal,
    "?hasAchievementsDisabled@LevelData@@QEBA_NXZ",
    bool
) {
    if (GMLIB::LevelAPI::mForceAchievementsEnabled) {
        return false;
    }
    return origin();
}

LL_AUTO_INSTANCE_HOOK(
    AllowCheatsSetting,
    ll::memory::HookPriority::Normal,
    "?allowCheats@PropertiesSettings@@QEBA_NXZ",
    bool
) {
    if (GMLIB::LevelAPI::mForceAchievementsEnabled) {
        return false;
    }
    return origin();
}

void initExperiments(LevelData* leveldat) {
    if (GMLIB::LevelAPI::mExperimentsRequireList.size() >= 1) {
        for (auto exp : GMLIB::LevelAPI::mExperimentsRequireList) {
            leveldat->getExperiments().setExperimentEnabled(exp, true);
        }
    }
}

LL_AUTO_INSTANCE_HOOK(
    LevelDatInit,
    ll::memory::HookPriority::Normal,
    "?getLevelData@ExternalFileLevelStorageSource@@UEBA?AVLevelData@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$"
    "allocator@D@2@@std@@@Z",
    LevelData*,
    std::string& a1
) {
    auto res = origin(a1);
    initExperiments(res);
    return res;
}

LL_AUTO_INSTANCE_HOOK(isTrustSkin, ll::memory::HookPriority::Normal, "?isTrustedSkin@SerializedSkin@@QEBA_NXZ", bool) {
    if (GMLIB::LevelAPI::mForceTrustSkin) {
        return true;
    }
    return origin();
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    ResourcePacksInfoPacketWrite,
    ll::memory::HookPriority::Normal,
    ResourcePacksInfoPacket,
    "?write@ResourcePacksInfoPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    class BinaryStream& stream
) {
    if (GMLIB::LevelAPI::mCoResourcePack) {
        this->mData.mResourcePackRequired    = true;
        this->mData.mForceServerPacksEnabled = false;
    }
    return origin(stream);
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    StartGamePacketWrite,
    ll::memory::HookPriority::Normal,
    StartGamePacket,
    "?write@StartGamePacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    class BinaryStream& stream
) {
    if (GMLIB::LevelAPI::mFakeSeedEnabled) {
        this->mSettings.mSeed.mValue = GMLIB::LevelAPI::mFakeSeed;
    }
    if (GMLIB::LevelAPI::mFakeLevelNameEnabled) {
        this->mLevelName = GMLIB::LevelAPI::mFakeLevelName;
    }
    return origin(stream);
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    EduInit,
    ll::memory::HookPriority::Normal,
    LevelData,
    "?educationFeaturesEnabled@LevelData@@QEBA_NXZ",
    bool
) {
    auto res = origin();
    if (GMLIB::LevelAPI::mEducationEditionEnabled) {
        this->setEducationFeaturesEnabled(true);
        return true;
    }
    GMLIB::LevelAPI::mEducationEditionEnabled = res;
    return res;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    RegAbility,
    ll::memory::HookPriority::Normal,
    ChangeSettingCommand,
    "?setup@ChangeSettingCommand@@SAXAEAVCommandRegistry@@@Z",
    void,
    class CommandRegistry& registry
) {
    if (GMLIB::LevelAPI::mEducationEditionEnabled == false && GMLIB::LevelAPI::mRegAbilityCommand == true) {
        AbilityCommand::setup(registry);
    }
    return origin(registry);
}