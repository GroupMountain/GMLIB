#pragma once
#include "GMLIB/GMLIB.h"
#include "mc/enums/AllExperiments.h"
#include "mc/network/packet/SetTitlePacket.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/storage/GameRuleId.h"

enum class WeatherType : int {
    Clear   = 0, // Sunny
    Rain    = 1, // Rainy
    Thunder = 2  // Lightning
};

enum class FillMode : int {
    Replace = 0, // Replace Air
    Keep    = 1,
    Outline = 2,
    Hollow  = 3,
    Destroy = 4
};

class GMLIB_Level : public Level {
public:
    GMLIB_API static GMLIB_Level* getInstance();

    GMLIB_API static GMLIB_Level* getLevel();

    GMLIB_API static void setClientWeather(WeatherType weather, Player* pl);

    GMLIB_API static void setClientWeather(WeatherType weather);

    GMLIB_API static void setFakeSeed(int64_t fakeSeed);

    GMLIB_API static void setCoResourcePack(bool enabled = true);

    GMLIB_API static void setForceTrustSkin(bool enabled = true);

    GMLIB_API static void addExperimentsRequire(::AllExperiments experiment);

    GMLIB_API static void addEducationEditionRequired();

    GMLIB_API static void setForceAchievementsEnabled();

    GMLIB_API static void forceEnableAbilityCommand();

    GMLIB_API static void setFakeLevelName(std::string_view fakeName);

    GMLIB_API static std::map<int, std::string> getAllExperiments();

    GMLIB_API static std::map<int, std::string> getAllExperimentsTranslateKeys();

    GMLIB_API static void broadcast(std::string_view message);

    GMLIB_API static void broadcastToast(std::string_view title, std::string_view message);

    GMLIB_API static void
    broadcastTitle(std::string_view title, SetTitlePacket::TitleType type = SetTitlePacket::TitleType::Title);

    GMLIB_API static void broadcastTitle(
        std::string_view          title,
        SetTitlePacket::TitleType type,
        int                       fadeInDuration,
        int                       remainDuration,
        int                       fadeOutDuration
    );

public:
    GMLIB_API BlockSource* getBlockSource(DimensionType dimid);

    GMLIB_API std::vector<Actor*> getAllEntities();

    GMLIB_API Actor* getEntity(ActorUniqueID auid);

    GMLIB_API std::vector<Player*> getAllPlayers();

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
        const Vec3&   pos,
        DimensionType dimensionId,
        float         power,
        Actor*        source          = nullptr,
        bool          breakBlocks     = true,
        bool          causeFire       = false,
        bool          allowUnderwater = false,
        float         maxResistance   = 3.40282347e+38
    );

    GMLIB_API Block* getBlock(BlockPos& pos, DimensionType dimId);

    GMLIB_API bool setBlock(BlockPos& pos, DimensionType dimId, Block* block);

    GMLIB_API bool setBlock(BlockPos& pos, DimensionType dimId, std::string_view name, short aux = 0);

    GMLIB_API int fillBlocks(
        BlockPos      startpos,
        BlockPos      endpos,
        DimensionType dimId,
        Block*        block,
        FillMode      mode = FillMode::Replace
    );

    GMLIB_API int fillBlocks(
        BlockPos         startpos,
        BlockPos         endpos,
        DimensionType    dimId,
        std::string_view name,
        ushort           tileData = 0,
        FillMode         mode     = FillMode::Replace
    );

    GMLIB_API int fillBlocks(BlockPos startpos, BlockPos endpos, DimensionType dimId, Block* oldBlock, Block* newBlock);

    GMLIB_API int fillBlocks(
        BlockPos         startpos,
        BlockPos         endpos,
        DimensionType    dimId,
        std::string_view oldName,
        ushort           oldTileData,
        std::string_view newName,
        ushort           newTileData
    );

    GMLIB_API double getServerMspt();

    GMLIB_API float getServerAverageTps();

    GMLIB_API float getServerCurrentTps();

    GMLIB_API void setFreezeTick(bool freeze = true);

    GMLIB_API bool isTickFreezed();

    GMLIB_API void setTickScale(float scale = 1.0f);

    GMLIB_API BlockPos getWorldSpawn();

    GMLIB_API void setWorldSpawn(BlockPos pos);
};
