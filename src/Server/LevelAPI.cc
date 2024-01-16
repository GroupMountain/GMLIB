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

} // namespace GMLIB::LevelAPI

GMLIB_Level* GMLIB_Level::getLevel() { return (GMLIB_Level*)ll::service::getLevel().as_ptr(); }

BlockSource* GMLIB_Level::getBlockSource(DimensionType dimid) {
    return &getDimension(dimid)->getBlockSourceFromMainChunkSource();
}

std::vector<Actor*> GMLIB_Level::getAllEntities() { return getRuntimeActorList(); }

std::string GMLIB_Level::getLevelName() { return getLevelData().getLevelName(); }

void GMLIB_Level::setLevelName(std::string newName) { getLevelData().setLevelName(newName); }

void GMLIB_Level::setFakeLevelName(std::string fakeName) {
    GMLIB::LevelAPI::mFakeLevelNameEnabled = true;
    GMLIB::LevelAPI::mFakeLevelName        = fakeName;
}

int64_t GMLIB_Level::getSeed() { return getLevelData().getSeed().mValue; }

void GMLIB_Level::setFakeSeed(int64_t fakeSeed) {
    GMLIB::LevelAPI::mFakeSeedEnabled = true;
    GMLIB::LevelAPI::mFakeSeed        = fakeSeed;
}

void GMLIB_Level::setCoResourcePack(bool enabled) { GMLIB::LevelAPI::mCoResourcePack = enabled; }

void GMLIB_Level::setForceTrustSkin(bool enabled) { GMLIB::LevelAPI::mForceTrustSkin = enabled; }

bool GMLIB_Level::getExperimentEnabled(::AllExperiments experiment) {
    return getLevelData().getExperiments().isExperimentEnabled(experiment);
}

void GMLIB_Level::setExperimentEnabled(::AllExperiments experiment, bool enabled) {
    getLevelData().getExperiments().setExperimentEnabled(experiment, enabled);
}

void GMLIB_Level::addExperimentsRequire(::AllExperiments experiment) {
    if (std::find(
            GMLIB::LevelAPI::mExperimentsRequireList.begin(),
            GMLIB::LevelAPI::mExperimentsRequireList.end(),
            experiment
        )
        == GMLIB::LevelAPI::mExperimentsRequireList.end()) {
        GMLIB::LevelAPI::mExperimentsRequireList.push_back(experiment);
    }
}

void GMLIB_Level::setForceAchievementsEnabled() { GMLIB::LevelAPI::mForceAchievementsEnabled = true; }

void GMLIB_Level::forceEnableAbilityCommand() { GMLIB::LevelAPI::mRegAbilityCommand = true; }

void GMLIB_Level::addEducationEditionRequired() { GMLIB::LevelAPI::mEducationEditionEnabled = true; }

void GMLIB_Level::setTime(int time) {
    setTime(time);
    try {
        SetTimePacket(time).sendToClients();
    } catch (...) {}
}

WeatherType GMLIB_Level::getWeather() {
    auto data = &getLevelData();
    if (data->isLightning()) {
        return WeatherType::Thunder;
    } else if (data->isRaining()) {
        return WeatherType::Rain;
    }
    return WeatherType::Clear;
}

void GMLIB_Level::setWeather(WeatherType weather, int lastTick) {
    switch (weather) {
    case WeatherType::Thunder: {
        updateWeather(1, lastTick, 1, lastTick);
        break;
    }
    case WeatherType::Rain: {
        updateWeather(1, lastTick, 0, lastTick);
        break;
    }
    default: {
        updateWeather(0, lastTick, 0, lastTick);
        break;
    }
    }
}

void GMLIB_Level::setWeather(WeatherType weather) {
    int lastTick = 20 * (getRandom().nextInt(600) + 300);
    setWeather(weather, lastTick);
}

void GMLIB_Level::setClientWeather(WeatherType weather, Player* pl) {
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

std::optional<bool> GMLIB_Level::getGameruleBool(GameRuleId id) {
    auto rule = ll::service::bedrock::getLevel()->getGameRules().getRule(id);
    if (rule) {
        if (rule->getType() == GameRule::Type::Bool) {
            return rule->getBool();
        }
    }
    return {};
}

std::optional<bool> GMLIB_Level::getGameruleBool(std::string name) {
    auto id = ll::service::bedrock::getLevel()->getGameRules().nameToGameRuleIndex(name);
    return getGameruleBool(id);
}

std::optional<float> GMLIB_Level::getGameruleFloat(GameRuleId id) {
    auto rule = ll::service::bedrock::getLevel()->getGameRules().getRule(id);
    if (rule) {
        if (rule->getType() == GameRule::Type::Float) {
            return rule->getFloat();
        }
    }
    return {};
}

std::optional<float> GMLIB_Level::getGameruleFloat(std::string name) {
    auto id = ll::service::bedrock::getLevel()->getGameRules().nameToGameRuleIndex(name);
    return getGameruleFloat(id);
}

std::optional<int> GMLIB_Level::getGameruleInt(GameRuleId id) {
    auto rule = ll::service::bedrock::getLevel()->getGameRules().getRule(id);
    if (rule) {
        if (rule->getType() == GameRule::Type::Int) {
            return rule->getInt();
        }
    }
    return {};
}

std::optional<int> GMLIB_Level::getGameruleInt(std::string name) {
    auto id = ll::service::bedrock::getLevel()->getGameRules().nameToGameRuleIndex(name);
    return getGameruleInt(id);
}

void GMLIB_Level::setGamerule(GameRuleId id, bool value) {
    auto pkt = ll::service::bedrock::getLevel()->getGameRules().setRule(id, value, true, nullptr, nullptr, nullptr);
    pkt->sendToClients();
}

void GMLIB_Level::setGamerule(std::string name, bool value) {
    auto id = ll::service::bedrock::getLevel()->getGameRules().nameToGameRuleIndex(name);
    return setGamerule(id, value);
}

void GMLIB_Level::setGamerule(GameRuleId id, float value) {
    auto pkt = ll::service::bedrock::getLevel()->getGameRules().setRule(id, value, true, nullptr, nullptr, nullptr);
    pkt->sendToClients();
}

void GMLIB_Level::setGamerule(std::string name, float value) {
    auto id = ll::service::bedrock::getLevel()->getGameRules().nameToGameRuleIndex(name);
    return setGamerule(id, value);
}

void GMLIB_Level::setGamerule(GameRuleId id, int value) {
    ll::service::bedrock::getLevel()->getGameRules();
    auto pkt = ll::service::bedrock::getLevel()->getGameRules().setRule(id, value, true, nullptr, nullptr, nullptr);
    pkt->sendToClients();
}

void GMLIB_Level::setGamerule(std::string name, int value) {
    auto id = ll::service::bedrock::getLevel()->getGameRules().nameToGameRuleIndex(name);
    return setGamerule(id, value);
}

void GMLIB_Level::createExplosion(
    const Vec3&   pos,
    DimensionType dimensionId,
    float         power,
    Actor*        source,
    bool          breakBlocks,
    bool          causeFire,
    bool          allowUnderwater,
    float         maxResistance
) {
    explode(*getBlockSource(dimensionId), source, pos, power, causeFire, breakBlocks, allowUnderwater, maxResistance);
}

Block* GMLIB_Level::getBlock(BlockPos& pos, DimensionType dimid) {
    return (Block*)&getBlockSource(dimid)->getBlock(pos);
}

bool GMLIB_Level::setBlock(Block* block, BlockPos& pos, DimensionType dimid) {
    return getBlockSource(dimid)->setBlock(pos, *block, 3, nullptr, nullptr);
}

bool GMLIB_Level::setBlock(std::string name, short aux, BlockPos& pos, DimensionType dimid) {
    auto block = Block::tryGetFromRegistry(name, aux);
    return getBlockSource(dimid)->setBlock(pos, block, 3, nullptr, nullptr);
}

bool checkFillPos(BlockPos startpos, BlockPos endpos) {
    if (startpos.x <= endpos.x && startpos.y <= endpos.y && startpos.z <= endpos.z) {
        return true;
    }
    return false;
}

bool checkPosInRange(BlockPos pos, BlockPos startpos, BlockPos endpos) {
    if (pos.x > startpos.x && pos.y > startpos.y && pos.z > startpos.z && pos.x < endpos.x && pos.y < endpos.y
        && pos.z < endpos.z) {
        return true;
    }
    return false;
}

int GMLIB_Level::fillBlocks(BlockPos startpos, BlockPos endpos, DimensionType dimensionId, Block* block, FillMode mode) {
    int  count       = 0;
    auto blockSource = getBlockSource(dimensionId);
    if (checkFillPos(startpos, endpos)) {
        int lx = endpos.x - startpos.x;
        int ly = endpos.y - startpos.y;
        int lz = endpos.z - startpos.z;
        for (int i = 0; i <= lx; i++) {
            for (int j = 0; j <= ly; j++) {
                for (int k = 0; k <= lz; k++) {
                    BlockPos pos = {startpos.x + i, startpos.y + j, startpos.z + k};
                    switch (mode) {
                    case FillMode::Hollow:
                        if (checkPosInRange(pos, startpos, endpos)) {
                            blockSource->setBlock(pos, *block, 3, nullptr, nullptr);
                            count++;
                        }
                    case FillMode::Outline:
                        if (!checkPosInRange(pos, startpos, endpos)) {
                            blockSource->setBlock(pos, *block, 3, nullptr, nullptr);
                            count++;
                        }
                        break;

                    case FillMode::Destroy:
                        destroyBlock(*getBlockSource(dimensionId), pos, true);
                    case FillMode::Replace:
                        blockSource->setBlock(pos, *block, 3, nullptr, nullptr);
                        count++;
                        break;

                    case FillMode::Keep:
                        if (blockSource->getBlock(pos).isAir()) {
                            blockSource->setBlock(pos, *block, 3, nullptr, nullptr);
                            count++;
                        }
                        break;
                    default:
                        return 0;
                    }
                }
            }
        }
        return count;
    }
    return 0;
}

int GMLIB_Level::fillBlocks(
    BlockPos       startpos,
    BlockPos       endpos,
    DimensionType  dimId,
    std::string    name,
    unsigned short tileData,
    FillMode       mode
) {
    Block* block = (Block*)Block::tryGetFromRegistry(name, tileData).as_ptr();
    if (block) {
        return fillBlocks(startpos, endpos, dimId, block, mode);
    }
    return 0;
}

int GMLIB_Level::fillBlocks(BlockPos startpos, BlockPos endpos, DimensionType dimId, Block* newblock, Block* oldblock) {
    int  count       = 0;
    auto blockSource = getBlockSource(dimId);
    if (checkFillPos(startpos, endpos)) {
        int lx = endpos.x - startpos.x;
        int ly = endpos.y - startpos.y;
        int lz = endpos.z - startpos.z;
        for (int i = 0; i <= lx; i++) {
            for (int j = 0; j <= ly; j++) {
                for (int k = 0; k <= lz; k++) {
                    BlockPos pos = {startpos.x + i, startpos.y + j, startpos.z + k};
                    if ((Block*)&blockSource->getBlock(pos) == oldblock) {
                        blockSource->setBlock(pos, *newblock, 3, nullptr, nullptr);
                        count++;
                    }
                }
            }
        }
        return count;
    }
    return 0;
}

int GMLIB_Level::fillBlocks(
    BlockPos       startpos,
    BlockPos       endpos,
    DimensionType  dimId,
    std::string    oldName,
    unsigned short oldTileData,
    std::string    newName,
    unsigned short newTileData
) {
    Block* newblock = (Block*)Block::tryGetFromRegistry(newName, newTileData).as_ptr();
    Block* oldblock = (Block*)Block::tryGetFromRegistry(oldName, oldTileData).as_ptr();
    if (newblock && oldblock) {
        return fillBlocks(startpos, endpos, dimId, newblock, oldblock);
    }
    return 0;
}

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