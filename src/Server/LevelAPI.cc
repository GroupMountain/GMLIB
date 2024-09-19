#include "Global.h"
#include <GMLIB/Server/BinaryStreamAPI.h>
#include <GMLIB/Server/I18nAPI.h>
#include <GMLIB/Server/LevelAPI.h>
#include <mc/network/packet/GameRulesChangedPacket.h>
#include <mc/network/packet/LevelEventPacket.h>
#include <mc/network/packet/ResourcePacksInfoPacket.h>
#include <mc/network/packet/SetSpawnPositionPacket.h>
#include <mc/network/packet/SetTimePacket.h>
#include <mc/network/packet/StartGamePacket.h>
#include <mc/network/packet/TextPacket.h>
#include <mc/network/packet/ToastRequestPacket.h>
#include <mc/server/commands/CommandContext.h>
#include <mc/server/commands/CommandOutput.h>
#include <mc/server/commands/MinecraftCommands.h>
#include <mc/server/commands/ServerCommandOrigin.h>
#include <mc/server/commands/edu/AbilityCommand.h>
#include <mc/server/common/PropertiesSettings.h>
#include <mc/server/common/commands/ChangeSettingCommand.h>
#include <mc/util/ProfilerLite.h>
#include <mc/util/Random.h>
#include <mc/world/level/ChunkBlockPos.h>
#include <mc/world/level/levelgen/WorldGenerator.h>
#include <mc/world/level/levelgen/structure/StructureFeatureTypeNames.h>
#include <mc/world/level/storage/Experiments.h>

using namespace ll::chrono_literals;

namespace GMLIB::LevelAPI {

std::string_view              mFakeLevelName;
int64_t                       mFakeSeed;
std::vector<::AllExperiments> mExperimentsRequireList;
bool                          mRegAbilityCommand       = false;
bool                          mEducationEditionEnabled = false;
float                         mAverageTps              = 20;
int                           mTicks                   = 20;
std::list<ushort>             mTickList;

} // namespace GMLIB::LevelAPI

LL_TYPE_INSTANCE_HOOK(
    ForceAchievementHook,
    ll::memory::HookPriority::Normal,
    LevelData,
    "?achievementsWillBeDisabledOnLoad@LevelData@@QEBA_NXZ",
    bool
) {
    if (getGameType() == GameType::Creative) { // IDA: LevelData::achievementsWillBeDisabledOnLoad
        setGameType(GameType::Survival);
    }
    ll::memory::dAccess<bool>(this, 1276) = false; // IDA: LevelData::achievementsWillBeDisabledOnLoad
    ll::memory::dAccess<bool>(this, 1464) = false; // IDA: LevelData::hasCheatsEnabled
    ll::memory::dAccess<bool>(this, 1465) = false; // IDA: LevelData::achievementsWillBeDisabledOnLoad
    return false;
}

LL_TYPE_INSTANCE_HOOK(
    AllowCheatsSettingHook,
    ll::memory::HookPriority::Normal,
    PropertiesSettings,
    &PropertiesSettings::allowCheats,
    bool
) {
    return false;
}

void initExperiments(LevelData* leveldat) {
    if (GMLIB::LevelAPI::mExperimentsRequireList.size() >= 1) {
        for (auto& exp : GMLIB::LevelAPI::mExperimentsRequireList) {
            leveldat->getExperiments().setExperimentEnabled(exp, true);
        }
    }
}

LL_INSTANCE_HOOK(TrustSkinHook, ll::memory::HookPriority::Normal, "?isTrustedSkin@SerializedSkin@@QEBA_NXZ", bool) {
    return true;
}

LL_TYPE_INSTANCE_HOOK(
    ResourcePacksSettingHook,
    ll::memory::HookPriority::Normal,
    PropertiesSettings,
    &PropertiesSettings::texturePackRequired,
    bool
) {
    return false;
}

LL_TYPE_INSTANCE_HOOK(
    ResourcePacksInfoPacketWrite,
    ll::memory::HookPriority::Normal,
    ResourcePacksInfoPacket,
    "?write@ResourcePacksInfoPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    class BinaryStream& stream
) {
    this->mData.mResourcePackRequired    = true;
    this->mData.mForceServerPacksEnabled = false;
    return origin(stream);
}

LL_TYPE_INSTANCE_HOOK(
    FakeSeedHook,
    ll::memory::HookPriority::Normal,
    StartGamePacket,
    "?write@StartGamePacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    class BinaryStream& stream
) {
    this->mSettings.setRandomSeed(LevelSeed64(GMLIB::LevelAPI::mFakeSeed));
    return origin(stream);
}

LL_TYPE_INSTANCE_HOOK(
    FakeLevelNameHook,
    ll::memory::HookPriority::Normal,
    StartGamePacket,
    "?write@StartGamePacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    class BinaryStream& stream
) {
    this->mLevelName = GMLIB::LevelAPI::mFakeLevelName;
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

optional_ref<GMLIB_Level> GMLIB_Level::getInstance() { return (GMLIB_Level*)ll::service::getLevel().as_ptr(); }

optional_ref<GMLIB_Level> GMLIB_Level::getLevel() { return getInstance(); }

BlockSource& GMLIB_Level::getBlockSource(DimensionType dimId) {
    return getOrCreateDimension(dimId)->getBlockSourceFromMainChunkSource();
}

std::vector<Actor*> GMLIB_Level::getAllEntities() { return getRuntimeActorList(); }

optional_ref<Actor> GMLIB_Level::getEntity(ActorUniqueID const& uniqueId) { return fetchEntity(uniqueId); }

std::vector<Player*> GMLIB_Level::getAllPlayers() {
    std::vector<Player*> result = {};
    this->forEachPlayer([&](Player& player) -> bool {
        result.push_back(&player);
        return true;
    });
    return result;
}

std::string GMLIB_Level::getLevelName() { return getLevelData().getLevelName(); }

void GMLIB_Level::setLevelName(std::string_view newName) { getLevelData().setLevelName(std::string(newName)); }

struct FakeLevelName_Impl {
    ll::memory::HookRegistrar<FakeLevelNameHook> r;
};

std::unique_ptr<FakeLevelName_Impl> fakeLevelName_Impl;

void GMLIB_Level::setFakeLevelName(std::string_view fakeName) {
    GMLIB::LevelAPI::mFakeLevelName = fakeName;
    if (!fakeLevelName_Impl) {
        fakeLevelName_Impl = std::make_unique<FakeLevelName_Impl>();
    }
}

int64_t GMLIB_Level::getSeed() { return getLevelData().getSeed().mValue; }

struct FakeSeed_Impl {
    ll::memory::HookRegistrar<FakeSeedHook> r;
};

std::unique_ptr<FakeSeed_Impl> fakeSeed_Impl;

void GMLIB_Level::setFakeSeed(int64_t fakeSeed) {
    GMLIB::LevelAPI::mFakeSeed = fakeSeed;
    if (!fakeSeed_Impl) {
        fakeSeed_Impl = std::make_unique<FakeSeed_Impl>();
    }
}

struct CoResourcePack_Impl {
    ll::memory::HookRegistrar<ResourcePacksSettingHook, ResourcePacksInfoPacketWrite> r;
};

std::unique_ptr<CoResourcePack_Impl> coResourcePack_Impl;

void GMLIB_Level::requireServerResourcePackAndAllowClientResourcePack(bool enabled) {
    if (enabled) {
        if (!coResourcePack_Impl) coResourcePack_Impl = std::make_unique<CoResourcePack_Impl>();
    } else {
        coResourcePack_Impl.reset();
    }
}

struct ForceTrustSkin_Impl {
    ll::memory::HookRegistrar<TrustSkinHook> r;
};

std::unique_ptr<ForceTrustSkin_Impl> forceTrustSkin_Impl;

void GMLIB_Level::trustAllSkins(bool enabled) {
    if (enabled) {
        if (!forceTrustSkin_Impl) forceTrustSkin_Impl = std::make_unique<ForceTrustSkin_Impl>();
    } else {
        forceTrustSkin_Impl.reset();
    }
}

bool GMLIB_Level::getExperimentEnabled(::AllExperiments experiment) {
    return getLevelData().getExperiments().isExperimentEnabled(experiment);
}

void GMLIB_Level::setExperimentEnabled(::AllExperiments experiment, bool enabled) {
    getLevelData().getExperiments().setExperimentEnabled(experiment, enabled);
}

std::string GMLIB_Level::getExperimentTranslateKey(AllExperiments experiment) {
    std::string result;
    try {
        result = Experiments::getExperimentTextID(experiment);
    } catch (...) {}
    return result;
}

std::vector<AllExperiments> GMLIB_Level::getAllExperiments() {
    std::vector<AllExperiments>       result;
    phmap::flat_hash_set<std::string> exist;
    for (int i = 4; i <= 24; i++) {
        std::string text;
        try {
            text = Experiments::getExperimentTextID((AllExperiments)i);
        } catch (...) {}
        if (!text.empty()) {
            if (exist.contains(text)) {
                return result;
            }
            exist.insert(text);
            result.push_back(AllExperiments(i));
        }
    }
    return result;
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

struct ForceAchievements_Impl {
    ll::memory::HookRegistrar<ForceAchievementHook, AllowCheatsSettingHook> r;
};

std::unique_ptr<ForceAchievements_Impl> forceAchievements_Impl;

void GMLIB_Level::setForceAchievementsEnabled() {
    if (!forceAchievements_Impl) {
        forceAchievements_Impl = std::make_unique<ForceAchievements_Impl>();
    }
}

void GMLIB_Level::tryRegisterAbilityCommand() { GMLIB::LevelAPI::mRegAbilityCommand = true; }

void GMLIB_Level::tryEnableEducationEdition() { GMLIB::LevelAPI::mEducationEditionEnabled = true; }

void GMLIB_Level::setAndUpdateTime(int time) {
    setTime(time);
    try {
        auto pkt = SetTimePacket(time);
        sendPacketToClients(pkt);
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

void GMLIB_Level::setClientWeather(WeatherType weather, Player& pl) {
    Vec3 pos = {0, 0, 0};
    switch (weather) {
    case WeatherType::Thunder: {
        auto pkt1 = LevelEventPacket(LevelEvent::StartThunderstorm, pos, 65565);
        sendPacketTo(pkt1, pl);
        auto pkt2 = LevelEventPacket(LevelEvent::StartRaining, pos, 65565);
        sendPacketTo(pkt2, pl);
        break;
    }
    case WeatherType::Rain: {
        auto pkt1 = LevelEventPacket(LevelEvent::StopThunderstorm, pos, 0);
        sendPacketTo(pkt1, pl);
        auto pkt2 = LevelEventPacket(LevelEvent::StartRaining, pos, 65565);
        sendPacketTo(pkt2, pl);
        break;
    }
    default: {
        auto pkt1 = LevelEventPacket(LevelEvent::StopThunderstorm, pos, 0);
        sendPacketTo(pkt1, pl);
        auto pkt2 = LevelEventPacket(LevelEvent::StopRaining, pos, 0);
        sendPacketTo(pkt2, pl);
        break;
    }
    }
}

void GMLIB_Level::setClientWeather(WeatherType weather) {
    Vec3 pos = {0, 0, 0};
    switch (weather) {
    case WeatherType::Thunder: {
        auto pkt1 = LevelEventPacket(LevelEvent::StartThunderstorm, pos, 65565);
        sendPacketToClients(pkt1);
        auto pkt2 = LevelEventPacket(LevelEvent::StartRaining, pos, 65565);
        sendPacketToClients(pkt2);
        break;
    }
    case WeatherType::Rain: {
        auto pkt1 = LevelEventPacket(LevelEvent::StopThunderstorm, pos, 0);
        sendPacketToClients(pkt1);
        auto pkt2 = LevelEventPacket(LevelEvent::StartRaining, pos, 65565);
        sendPacketToClients(pkt2);
        break;
    }
    default: {
        auto pkt1 = LevelEventPacket(LevelEvent::StopThunderstorm, pos, 0);
        sendPacketToClients(pkt1);
        auto pkt2 = LevelEventPacket(LevelEvent::StopRaining, pos, 0);
        sendPacketToClients(pkt2);
        break;
    }
    }
}

void GMLIB_Level::broadcast(std::string_view message) {
    auto pkt = TextPacket::createRawMessage(message);
    sendPacketToClients(pkt);
}

void GMLIB_Level::broadcast(std::string const& message, std::vector<std::string> const& params) {
    auto pkt = TextPacket::createTranslated(message, params);
    sendPacketToClients(pkt);
}

void GMLIB_Level::broadcastToast(std::string_view title, std::string_view message) {
    auto pkt = ToastRequestPacket(std::string(title), std::string(message));
    sendPacketToClients(pkt);
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

std::optional<bool> GMLIB_Level::getGameruleBool(std::string_view name) {
    auto id = ll::service::bedrock::getLevel()->getGameRules().nameToGameRuleIndex(std::string(name));
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

std::optional<float> GMLIB_Level::getGameruleFloat(std::string_view name) {
    auto id = ll::service::bedrock::getLevel()->getGameRules().nameToGameRuleIndex(std::string(name));
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

std::optional<int> GMLIB_Level::getGameruleInt(std::string_view name) {
    auto id = ll::service::bedrock::getLevel()->getGameRules().nameToGameRuleIndex(std::string(name));
    return getGameruleInt(id);
}

void GMLIB_Level::setGamerule(GameRuleId id, bool value) {
    auto pkt = ll::service::bedrock::getLevel()->getGameRules().setRule(id, value, true, nullptr, nullptr, nullptr);
    sendPacketToClients(*pkt);
}

void GMLIB_Level::setGamerule(std::string_view name, bool value) {
    auto id = ll::service::bedrock::getLevel()->getGameRules().nameToGameRuleIndex(std::string(name));
    return setGamerule(id, value);
}

void GMLIB_Level::setGamerule(GameRuleId id, float value) {
    auto pkt = ll::service::bedrock::getLevel()->getGameRules().setRule(id, value, true, nullptr, nullptr, nullptr);
    sendPacketToClients(*pkt);
}

void GMLIB_Level::setGamerule(std::string_view name, float value) {
    auto id = ll::service::bedrock::getLevel()->getGameRules().nameToGameRuleIndex(std::string(name));
    return setGamerule(id, value);
}

void GMLIB_Level::setGamerule(GameRuleId id, int value) {
    ll::service::bedrock::getLevel()->getGameRules();
    auto pkt = ll::service::bedrock::getLevel()->getGameRules().setRule(id, value, true, nullptr, nullptr, nullptr);
    sendPacketToClients(*pkt);
}

void GMLIB_Level::setGamerule(std::string_view name, int value) {
    auto id = ll::service::bedrock::getLevel()->getGameRules().nameToGameRuleIndex(std::string(name));
    return setGamerule(id, value);
}

void GMLIB_Level::createExplosion(
    const Vec3&         pos,
    DimensionType       dimensionId,
    float               power,
    optional_ref<Actor> source,
    bool                breakBlocks,
    bool                causeFire,
    bool                allowUnderwater,
    float               maxResistance
) {
    explode(getBlockSource(dimensionId), source, pos, power, causeFire, breakBlocks, allowUnderwater, maxResistance);
}

std::shared_ptr<LevelChunk> GMLIB_Level::getOrLoadChunk(
    BlockPos const& blockPos,
    DimensionType   dimId,
    bool            readOnly,
    bool            forceImmediateReplacementDataLoad
) {
    auto chunkPos = ChunkPos(blockPos);
    return getOrLoadChunk(chunkPos, dimId, readOnly, forceImmediateReplacementDataLoad);
}

std::shared_ptr<LevelChunk> GMLIB_Level::getOrLoadChunk(
    ChunkPos const& chunkPos,
    DimensionType   dimId,
    bool            readOnly,
    bool            forceImmediateReplacementDataLoad
) {
    auto& chunkSource = getBlockSource(dimId).getChunkSource();
    auto  chunk       = chunkSource.getOrLoadChunk(chunkPos, ChunkSource::LoadMode::Deferred, readOnly);
    chunkSource.loadChunk(*chunk, forceImmediateReplacementDataLoad);
    return std::move(chunk);
}

Block const& GMLIB_Level::getBlock(BlockPos const& pos, DimensionType dimId) {
    return getBlockSource(dimId).getBlock(pos);
}

Block const& GMLIB_Level::loadAndGetBlock(BlockPos const& pos, DimensionType dimId) {
    auto chunk     = getOrLoadChunk(pos, dimId);
    auto dimension = getOrCreateDimension(dimId);
    auto cbp       = ChunkBlockPos(pos, dimension->getMinHeight());
    return chunk->getBlock(cbp);
}

bool GMLIB_Level::setBlock(BlockPos const& pos, DimensionType dimId, Block const& block) {
    return getBlockSource(dimId).setBlock(pos, block, 3, nullptr, nullptr);
}

bool GMLIB_Level::setBlock(BlockPos const& pos, DimensionType dimId, std::string_view name, short aux) {
    if (auto block = Block::tryGetFromRegistry(name, aux)) {
        return getBlockSource(dimId).setBlock(pos, block, 3, nullptr, nullptr);
    }
    return false;
}

bool checkFillPos(BlockPos const& startpos, BlockPos const& endpos) {
    if (startpos.x <= endpos.x && startpos.y <= endpos.y && startpos.z <= endpos.z) {
        return true;
    }
    return false;
}

bool checkPosInRange(BlockPos const& pos, BlockPos const& startpos, BlockPos const& endpos) {
    if (pos.x > startpos.x && pos.y > startpos.y && pos.z > startpos.z && pos.x < endpos.x && pos.y < endpos.y
        && pos.z < endpos.z) {
        return true;
    }
    return false;
}

int GMLIB_Level::fillBlocks(
    BlockPos const& startpos,
    BlockPos const& endpos,
    DimensionType   dimensionId,
    Block const&    block,
    FillMode        mode
) {
    int   count       = 0;
    auto& blockSource = getBlockSource(dimensionId);
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
                            blockSource.setBlock(pos, block, 3, nullptr, nullptr);
                            count++;
                        }
                    case FillMode::Outline:
                        if (!checkPosInRange(pos, startpos, endpos)) {
                            blockSource.setBlock(pos, block, 3, nullptr, nullptr);
                            count++;
                        }
                        break;

                    case FillMode::Destroy:
                        destroyBlock(getBlockSource(dimensionId), pos, true);
                    case FillMode::Replace:
                        blockSource.setBlock(pos, block, 3, nullptr, nullptr);
                        count++;
                        break;

                    case FillMode::Keep:
                        if (blockSource.getBlock(pos).isAir()) {
                            blockSource.setBlock(pos, block, 3, nullptr, nullptr);
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
    BlockPos const&  startpos,
    BlockPos const&  endpos,
    DimensionType    dimId,
    std::string_view name,
    unsigned short   tileData,
    FillMode         mode
) {
    auto block = Block::tryGetFromRegistry(name, tileData).as_ptr();
    if (block) {
        return fillBlocks(startpos, endpos, dimId, *block, mode);
    }
    return 0;
}

int GMLIB_Level::fillBlocks(
    BlockPos const& startpos,
    BlockPos const& endpos,
    DimensionType   dimId,
    Block const&    newblock,
    Block const&    oldblock
) {
    int   count       = 0;
    auto& blockSource = getBlockSource(dimId);
    if (checkFillPos(startpos, endpos)) {
        int lx = endpos.x - startpos.x;
        int ly = endpos.y - startpos.y;
        int lz = endpos.z - startpos.z;
        for (int i = 0; i <= lx; i++) {
            for (int j = 0; j <= ly; j++) {
                for (int k = 0; k <= lz; k++) {
                    BlockPos pos = {startpos.x + i, startpos.y + j, startpos.z + k};
                    if (blockSource.getBlock(pos).getRuntimeId() == oldblock.getRuntimeId()) {
                        blockSource.setBlock(pos, newblock, 3, nullptr, nullptr);
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
    BlockPos const&  startpos,
    BlockPos const&  endpos,
    DimensionType    dimId,
    std::string_view oldName,
    ushort           oldTileData,
    std::string_view newName,
    ushort           newTileData
) {
    auto newblock = Block::tryGetFromRegistry(newName, newTileData).as_ptr();
    auto oldblock = Block::tryGetFromRegistry(oldName, oldTileData).as_ptr();
    if (newblock && oldblock) {
        return fillBlocks(startpos, endpos, dimId, *newblock, *oldblock);
    }
    return 0;
}

std::chrono::nanoseconds GMLIB_Level::getServerTickTime() {
    return ProfilerLite::gProfilerLiteInstance.getServerTickTime();
}

double GMLIB_Level::getServerMspt() { return (double)getServerTickTime().count() / 1000000.0; }

float GMLIB_Level::getServerAverageTps() { return GMLIB::LevelAPI::mAverageTps; }

float GMLIB_Level::getServerCurrentTps() { return getServerMspt() <= 50 ? 20 : (float)(1000.0 / getServerMspt()); }

void GMLIB_Level::setFreezeTick(bool freeze) { ll::service::getMinecraft()->setSimTimePause(freeze); }

void GMLIB_Level::setTickScale(float scale) { ll::service::getMinecraft()->setSimTimeScale(scale); }

bool GMLIB_Level::isTickFreezed() { return ll::service::getMinecraft()->getSimPaused(); }

void CaculateTPS() {
    static ll::schedule::GameTickAsyncScheduler scheduler1;
    scheduler1.add<ll::schedule::RepeatTask>(1_tick, [] { GMLIB::LevelAPI::mTicks++; });
    static ll::schedule::SystemTimeScheduler scheduler2;
    scheduler2.add<ll::schedule::RepeatTask>(1s, [] {
        GMLIB::LevelAPI::mTickList.push_back(GMLIB::LevelAPI::mTicks);
        GMLIB::LevelAPI::mTicks = 0;
        if (GMLIB::LevelAPI::mTickList.size() >= 60) {
            GMLIB::LevelAPI::mTickList.clear();
            return;
        }
        uint ticks_minute = 0;
        for (auto& i : GMLIB::LevelAPI::mTickList) {
            ticks_minute = ticks_minute + i;
        }
        float res                    = (float)ticks_minute / ((float)GMLIB::LevelAPI::mTickList.size());
        GMLIB::LevelAPI::mAverageTps = res >= 20.0f ? 20.0f : res;
    });
}

void GMLIB_Level::broadcastTitle(std::string_view text, SetTitlePacket::TitleType type) {
    auto pkt = SetTitlePacket(type, std::string(text));
    sendPacketToClients(pkt);
}

void GMLIB_Level::broadcastTitle(
    std::string_view          text,
    SetTitlePacket::TitleType type,
    int                       fadeInDuration,
    int                       remainDuration,
    int                       fadeOutDuration
) {
    auto pkt         = SetTitlePacket(type, std::string(text));
    pkt.mFadeInTime  = fadeInDuration;
    pkt.mFadeOutTime = fadeOutDuration;
    pkt.mStayTime    = remainDuration;
    sendPacketToClients(pkt);
}

BlockPos GMLIB_Level::getWorldSpawn() { return getLevelData().getSpawnPos(); }

void GMLIB_Level::setWorldSpawn(BlockPos pos) {
    getLevelData().setSpawnPos(pos);
    auto pkt = SetSpawnPositionPacket((SpawnPositionType)1, 0, pos);
    sendPacketToClients(pkt);
}

MCRESULT GMLIB_Level::executeCommand(std::string_view command, DimensionType dimId) {
    CommandContext context = CommandContext(
        std::string(command),
        std::make_unique<ServerCommandOrigin>(
            ServerCommandOrigin("Server", ll::service::getLevel()->asServer(), CommandPermissionLevel::Internal, dimId)
        )
    );
    return ll::service::getMinecraft()->getCommands().executeCommand(context);
}

std::pair<bool, std::string> GMLIB_Level::executeCommandEx(std::string_view cmd, DimensionType dimId) {
    std::pair<bool, std::string> result;
    auto                         origin =
        ServerCommandOrigin("Server", ll::service::getLevel()->asServer(), CommandPermissionLevel::Internal, dimId);
    auto command = ll::service::getMinecraft()->getCommands().compileCommand(
        std::string(cmd),
        origin,
        (CurrentCmdVersion)CommandVersion::CurrentVersion,
        [&](std::string const& err) { result.second.append(err).append("\n"); }
    );
    if (command) {
        CommandOutput output(CommandOutputType::AllOutput);
        command->run(origin, output);
        for (auto& msg : output.getMessages()) {
            std::string temp;
            getI18n().getCurrentLanguage()->get(msg.getMessageId(), temp, msg.getParams());
            result.second += temp.append("\n");
        }
        if (result.second.ends_with('\n')) {
            result.second.pop_back();
        }
        result.first = output.getSuccessCount() ? true : false;
        return result;
    }
    if (result.second.ends_with('\n')) {
        result.second.pop_back();
    }
    result.first = false;
    ;
    return result;
}

bool GMLIB_Level::isInStructureFeature(StructureFeatureType structure, BlockPos const& pos, DimensionType dimId) {
    return getDimension(dimId)->getWorldGenerator()->isStructureFeatureTypeAt(pos, structure);
}

StructureFeatureType GMLIB_Level::getStructureFeature(BlockPos const& pos, DimensionType dimId) {
    return getDimension(dimId)->getWorldGenerator()->findStructureFeatureTypeAt(pos);
}

bool GMLIB_Level::isInStructureFeature(std::string const& structure, BlockPos const& pos, DimensionType dimId) {
    auto type = StructureFeatureTypeNames::getFeatureType(structure);
    return isInStructureFeature(type, pos, dimId);
}

std::string_view GMLIB_Level::getStructureFeatureName(BlockPos const& pos, DimensionType dimId) {
    return StructureFeatureTypeNames::getFeatureName(getStructureFeature(pos, dimId));
}

std::optional<BlockPos> GMLIB_Level::locateNearestStructureFeature(
    StructureFeatureType structure,
    BlockPos const&      pos,
    DimensionType        dimId,
    bool                 useNewChunksOnly
) {
    BlockPos result = {0, 64, 0};
    if (auto find = getOrCreateDimension(dimId)
                        ->getWorldGenerator()
                        ->findNearestStructureFeature(structure, pos, result, useNewChunksOnly, {})) {
        return result;
    }
    return {};
}

std::optional<BlockPos> GMLIB_Level::locateNearestStructureFeature(
    std::string const& structure,
    BlockPos const&    pos,
    DimensionType      dimId,
    bool               useNewChunksOnly
) {
    auto type = StructureFeatureTypeNames::getFeatureType(structure);
    return locateNearestStructureFeature(type, pos, dimId, useNewChunksOnly);
}

DBStorage& GMLIB_Level::getDBStorage() { return *GMLIB::Global<DBStorage>; }

void GMLIB_Level::sendPacketToClients(Packet& packet) {
    GMLIB_BinaryStream bs;
    packet.writeWithHeader(SubClientId::PrimaryClient, bs);
    bs.sendToClients();
}

void GMLIB_Level::sendPacketToDimension(Packet& packet, DimensionType dimId) {
    GMLIB_BinaryStream bs;
    packet.writeWithHeader(SubClientId::PrimaryClient, bs);
    bs.sendToDimension(dimId);
}

void GMLIB_Level::sendPacketTo(Packet& packet, Player& player) {
    GMLIB_BinaryStream bs;
    packet.writeWithHeader(SubClientId::PrimaryClient, bs);
    bs.sendTo(player);
}

int GMLIB_Level::getMaxPlayerCount() {
    // IDA: ServerNetworkHandler::setMaxNumPlayers
    return ll::memory::dAccess<int>(ll::service::getServerNetworkHandler().as_ptr(), 200 * 4);
}

int GMLIB_Level::setMaxPlayerCount(int count) {
    auto result = ll::service::getServerNetworkHandler()->setMaxNumPlayers(count);
    ll::service::getServerNetworkHandler()->updateServerAnnouncement();
    return result;
}

void GMLIB_Level::setServerMotd(std::string_view motd) {
    ll::service::getServerNetworkHandler()->allowIncomingConnections(std::string(motd), true);
}