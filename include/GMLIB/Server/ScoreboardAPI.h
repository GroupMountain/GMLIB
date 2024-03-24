#pragma once
#include "GMLIB/GMLIB.h"
#include "mc/world/scores/Scoreboard.h"
#include "mc/world/scores/ServerScoreboard.h"

class GMLIB_Scoreboard : public Scoreboard {
public:
    using Scoreboard::addObjective;
    using Scoreboard::removeObjective;
    using Scoreboard::resetPlayerScore;

public:
    GMLIB_API static GMLIB_Scoreboard* getInstance();

public:
    GMLIB_API ServerScoreboard* getServerScoreboard();

    GMLIB_API IdentityDictionary* getIdentityDictionary();

    GMLIB_API std::unordered_map<std::string, DisplayObjective> getDisplayObjectives();

    GMLIB_API Objective* addObjective(std::string name);

    GMLIB_API Objective* addObjective(std::string name, std::string displayName);

    GMLIB_API bool removeObjective(std::string objective);

    GMLIB_API ScoreboardId getPlayerScoreboardId(std::string serverId);

    GMLIB_API ScoreboardId getPlayerScoreboardId(mce::UUID const& uuid);

    GMLIB_API std::optional<int> getScore(Objective* objective, ScoreboardId& scoreboardId);

    GMLIB_API std::optional<int> getScore(std::string objective, std::string name);

    GMLIB_API std::optional<int> getScore(std::string objective, Player* pl);

    GMLIB_API std::optional<int> getScore(std::string objective, Actor* ac);

    GMLIB_API std::optional<int> getScore(std::string objective, ActorUniqueID uniqueId, bool isPlayer = false);

    GMLIB_API std::optional<int> getPlayerScore(std::string objective, std::string serverId);

    GMLIB_API std::optional<int> getPlayerScore(std::string objective, mce::UUID const& uuid);

    GMLIB_API std::optional<int> getPlayerScore(std::string objective, ActorUniqueID uniqueId);

    GMLIB_API std::optional<int> getPlayerScore(std::string objective, Player* pl);

    GMLIB_API std::optional<int> setScore(
        Objective*             objective,
        ScoreboardId&          scoreboardId,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API std::optional<int> setScore(
        std::string            objective,
        std::string            name,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API std::optional<int>
    setScore(std::string objective, Player* pl, int value, PlayerScoreSetFunction action = PlayerScoreSetFunction::Set);

    GMLIB_API std::optional<int>
    setScore(std::string objective, Actor* ac, int value, PlayerScoreSetFunction action = PlayerScoreSetFunction::Set);

    GMLIB_API std::optional<int> setScore(
        std::string            objective,
        ActorUniqueID          uniqueId,
        int                    value,
        PlayerScoreSetFunction action   = PlayerScoreSetFunction::Set,
        bool                   isPlayer = false
    );

    GMLIB_API std::optional<int> setPlayerScore(
        std::string            objective,
        std::string            serverId,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API std::optional<int> setPlayerScore(
        std::string            objective,
        mce::UUID const&       uuid,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API std::optional<int> setPlayerScore(
        std::string            objective,
        ActorUniqueID          uniqueId,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API std::optional<int> setPlayerScore(
        std::string            objective,
        Player*                pl,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API bool resetScore(Objective* objective, ScoreboardId& scoreboardId);

    GMLIB_API bool resetScore(std::string objective, std::string name);

    GMLIB_API bool resetScore(std::string objective, Player* pl);

    GMLIB_API bool resetScore(std::string objective, Actor* ac);

    GMLIB_API bool resetScore(std::string objective, ActorUniqueID uniqueId, bool isPlayer = false);

    GMLIB_API bool resetPlayerScore(std::string objective, std::string serverId);

    GMLIB_API bool resetPlayerScore(std::string objective, mce::UUID const& uuid);

    GMLIB_API bool resetPlayerScore(std::string objective, ActorUniqueID uniqueId);

    GMLIB_API bool resetPlayerScore(std::string objective, Player* pl);

    GMLIB_API bool resetScore(ScoreboardId& scoreboardId);

    GMLIB_API bool resetScore(std::string name);

    GMLIB_API bool resetScore(Player* pl);

    GMLIB_API bool resetScore(Actor* ac);

    GMLIB_API bool resetScore(ActorUniqueID uniqueId, bool isPlayer = false);

    GMLIB_API bool resetPlayerScore(std::string serverId);

    GMLIB_API bool resetPlayerScore(mce::UUID const& uuid);

    GMLIB_API bool resetPlayerScore(Player* pl);

    GMLIB_API bool resetPlayerScore(ActorUniqueID uniqueId);

    GMLIB_API std::vector<ScoreboardId> getAllScoreboardIds();

    GMLIB_API std::vector<ScoreboardId> getObjectiveTrackedScoreboardIds(Objective* objective);

    GMLIB_API std::vector<ScoreboardId> getAllScoreboardIds(IdentityDefinition::Type type);

    GMLIB_API std::vector<ScoreboardId>
              getObjectiveTrackedScoreboardIds(Objective* objective, IdentityDefinition::Type type);

    GMLIB_API std::vector<ActorUniqueID> getAllPlayers();

    GMLIB_API std::vector<std::string> getAllPlayerServerIds();

    GMLIB_API std::vector<mce::UUID> getAllPlayerUuids();

    GMLIB_API std::vector<ActorUniqueID> getAllEntities();

    GMLIB_API std::vector<std::string> getAllFakePlayers();

    GMLIB_API void setObjectiveDisplay(
        Objective*         objective,
        std::string        displaySlot = "sidebar",
        ObjectiveSortOrder order       = ObjectiveSortOrder::Ascending
    );

    GMLIB_API void setObjectiveDisplay(
        std::string        objective,
        std::string        displaySlot = "sidebar",
        ObjectiveSortOrder order       = ObjectiveSortOrder::Ascending
    );

    GMLIB_API void clearObjectiveDisplay(std::string displaySlot);

    GMLIB_API std::optional<std::string> getObjectiveDisplayName(std::string objective);

    GMLIB_API bool setObjectiveDisplayName(std::string objective, std::string newName);
};