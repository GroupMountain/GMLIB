#pragma once
#include "GMLIB/Macros.h"
#include "mc/codebuilder/MCRESULT.h"
#include "mc/enums/AllExperiments.h"
#include "mc/network/packet/Packet.h"
#include "mc/network/packet/SetTitlePacket.h"
#include "mc/world/level/ChunkPos.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/levelgen/structure/StructureFeatureType.h"
#include "mc/world/level/storage/DBStorage.h"
#include "mc/world/level/storage/GameRuleId.h"

enum class WeatherType : int {
    Clear   = 0, // Sunny
    Rain    = 1, // Rainy
    Thunder = 2  // Lightning
};

enum class FillMode : int {
    Replace = 0, //
    Keep    = 1, //
    Outline = 2, //
    Hollow  = 3,
    Destroy = 4
};

class GMLIB_Level : public Level {
public:
    GMLIB_NDAPI static optional_ref<GMLIB_Level> getInstance();

    GMLIB_NDAPI static optional_ref<GMLIB_Level> getLevel();

    GMLIB_API static void setFakeSeed(int64_t fakeSeed);

    GMLIB_API static void requireServerResourcePackAndAllowClientResourcePack(bool enabled = true);

    GMLIB_API static void trustAllSkins(bool enabled = true);

    GMLIB_API static void addExperimentsRequire(::AllExperiments experiment);

    GMLIB_API static void tryEnableEducationEdition();

    GMLIB_API static void setForceAchievementsEnabled();

    GMLIB_API static void tryRegisterAbilityCommand();

    GMLIB_API static void setFakeLevelName(std::string_view fakeName);

    GMLIB_NDAPI static std::vector<AllExperiments> getAllExperiments();

    GMLIB_NDAPI static std::string getExperimentTranslateKey(AllExperiments experiment);

public:
    GMLIB_NDAPI BlockSource& getBlockSource(DimensionType dimid);

    GMLIB_NDAPI std::vector<Actor*> getAllEntities();

    GMLIB_NDAPI optional_ref<Actor> getEntity(ActorUniqueID const& uniqueId);

    GMLIB_NDAPI std::vector<Player*> getAllPlayers();

    GMLIB_API MCRESULT executeCommand(std::string_view command, DimensionType dimId = 0);

    GMLIB_API std::pair<bool, std::string> executeCommandEx(std::string_view command, DimensionType dimId = 0);

    GMLIB_API std::string getLevelName();

    GMLIB_API void setLevelName(std::string_view newName);

    GMLIB_API int64_t getSeed();

    GMLIB_API bool getExperimentEnabled(::AllExperiments experiment);

    GMLIB_API void setExperimentEnabled(::AllExperiments experiment, bool enabled = true);

    GMLIB_API void setAndUpdateTime(int time);

    GMLIB_API WeatherType getWeather();

    GMLIB_API void setWeather(WeatherType weather);

    GMLIB_API void setWeather(WeatherType weather, int lastTick);

    GMLIB_API std::optional<bool> getGameruleBool(GameRuleId id);

    GMLIB_API std::optional<bool> getGameruleBool(std::string_view name);

    GMLIB_API std::optional<float> getGameruleFloat(GameRuleId id);

    GMLIB_API std::optional<float> getGameruleFloat(std::string_view name);

    GMLIB_API std::optional<int> getGameruleInt(GameRuleId id);

    GMLIB_API std::optional<int> getGameruleInt(std::string_view name);

    GMLIB_API void setGamerule(GameRuleId id, bool value);

    GMLIB_API void setGamerule(std::string_view name, bool value);

    GMLIB_API void setGamerule(GameRuleId id, float value);

    GMLIB_API void setGamerule(std::string_view name, float value);

    GMLIB_API void setGamerule(GameRuleId id, int value);

    GMLIB_API void setGamerule(std::string_view name, int value);

    GMLIB_API void createExplosion(
        Vec3 const&         pos,
        DimensionType       dimensionId,
        float               power,
        optional_ref<Actor> source          = nullptr,
        bool                breakBlocks     = true,
        bool                causeFire       = false,
        bool                allowUnderwater = false,
        float               maxResistance   = 3.40282347e+38
    );

    GMLIB_NDAPI std::shared_ptr<LevelChunk> getOrLoadChunk(
        BlockPos const& blockPos,
        DimensionType   dimId,
        bool            readOnly                          = true,
        bool            forceImmediateReplacementDataLoad = false
    );

    GMLIB_NDAPI std::shared_ptr<LevelChunk> getOrLoadChunk(
        ChunkPos const& chunkPos,
        DimensionType   dimId,
        bool            readOnly                          = true,
        bool            forceImmediateReplacementDataLoad = false
    );

    GMLIB_NDAPI Block const& getBlock(BlockPos const& pos, DimensionType dimId);

    GMLIB_NDAPI Block const& loadAndGetBlock(BlockPos const& pos, DimensionType dimId);

    GMLIB_API bool setBlock(BlockPos const& pos, DimensionType dimId, Block const& block);

    GMLIB_API bool setBlock(BlockPos const& pos, DimensionType dimId, std::string_view name, short aux = 0);

    GMLIB_API int fillBlocks(
        BlockPos const& startpos,
        BlockPos const& endpos,
        DimensionType   dimId,
        Block const&    block,
        FillMode        mode = FillMode::Replace
    );

    GMLIB_API int fillBlocks(
        BlockPos const&  startpos,
        BlockPos const&  endpos,
        DimensionType    dimId,
        std::string_view name,
        ushort           tileData = 0,
        FillMode         mode     = FillMode::Replace
    );

    GMLIB_API int fillBlocks(
        BlockPos const& startpos,
        BlockPos const& endpos,
        DimensionType   dimId,
        Block const&    oldBlock,
        Block const&    newBlock
    );

    GMLIB_API int fillBlocks(
        BlockPos const&  startpos,
        BlockPos const&  endpos,
        DimensionType    dimId,
        std::string_view oldName,
        ushort           oldTileData,
        std::string_view newName,
        ushort           newTileData
    );

    GMLIB_NDAPI double getServerMspt();

    GMLIB_NDAPI float getServerAverageTps();

    GMLIB_NDAPI float getServerCurrentTps();

    GMLIB_API void setFreezeTick(bool freeze = true);

    GMLIB_API bool isTickFreezed();

    GMLIB_API void setTickScale(float scale = 1.0f);

    GMLIB_NDAPI BlockPos getWorldSpawn();

    GMLIB_API void setWorldSpawn(BlockPos pos);

    GMLIB_API bool isInStructureFeature(StructureFeatureType structure, BlockPos const& pos, DimensionType dimId);

    GMLIB_API bool isInStructureFeature(std::string const& structure, BlockPos const& pos, DimensionType dimId);

    GMLIB_NDAPI StructureFeatureType getStructureFeature(BlockPos const& pos, DimensionType dimId);

    GMLIB_NDAPI std::string_view getStructureFeatureName(BlockPos const& pos, DimensionType dimId);

    GMLIB_NDAPI std::optional<BlockPos> locateNearestStructureFeature(
        StructureFeatureType structure,
        BlockPos const&      pos,
        DimensionType        dimId,
        bool                 useNewChunksOnly = false
    );

    GMLIB_NDAPI std::optional<BlockPos> locateNearestStructureFeature(
        std::string const& structure,
        BlockPos const&    pos,
        DimensionType      dimId,
        bool               useNewChunksOnly = false
    );

    GMLIB_NDAPI DBStorage& getDBStorage();

    GMLIB_API void sendPacketToClients(Packet& packet);

    GMLIB_API void sendPacketToDimension(Packet& packet, DimensionType dimId);

    GMLIB_API void sendPacketTo(Packet& packet, Player& player);

    GMLIB_API void setClientWeather(WeatherType weather, Player& pl);

    GMLIB_API void setClientWeather(WeatherType weather);

    GMLIB_API void broadcast(std::string_view message);

    GMLIB_API void broadcast(std::string const& message, std::vector<std::string> const& params);

    GMLIB_API void broadcastToast(std::string_view title, std::string_view message);

    GMLIB_API void
    broadcastTitle(std::string_view title, SetTitlePacket::TitleType type = SetTitlePacket::TitleType::Title);

    GMLIB_API void broadcastTitle(
        std::string_view          title,
        SetTitlePacket::TitleType type,
        int                       fadeInDuration,
        int                       remainDuration,
        int                       fadeOutDuration
    );

    GMLIB_NDAPI int getMaxPlayerCount();

    GMLIB_NDAPI int getOnlinePlayerCount();

    GMLIB_API int setMaxPlayerCount(int count);

    GMLIB_API void setServerMotd(std::string_view motd);
};
