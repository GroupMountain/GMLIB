#pragma once
#include "GMLIB/Macros.h"
#include "mc/world/scores/Scoreboard.h"
#include "mc/world/scores/ServerScoreboard.h"

class GMLIB_Scoreboard : public Scoreboard {
public:
    using Scoreboard::addObjective;
    using Scoreboard::removeObjective;
    using Scoreboard::resetPlayerScore;

public:
    GMLIB_NDAPI static optional_ref<GMLIB_Scoreboard> getInstance();

public:
    GMLIB_NDAPI optional_ref<ServerScoreboard> getServerScoreboard();

    GMLIB_NDAPI optional_ref<IdentityDictionary> getIdentityDictionary();

    GMLIB_NDAPI std::unordered_map<std::string, DisplayObjective> getDisplayObjectives();

    GMLIB_NDAPI optional_ref<Objective> addObjective(std::string const& name);

    GMLIB_NDAPI optional_ref<Objective> addObjective(std::string const& name, std::string const& displayName);

    GMLIB_API bool removeObjective(std::string const& objective);

    GMLIB_NDAPI ScoreboardId getPlayerScoreboardId(std::string const& serverId);

    GMLIB_NDAPI ScoreboardId getPlayerScoreboardId(mce::UUID const& uuid);

    GMLIB_NDAPI std::optional<int> getScore(Objective& objective, ScoreboardId const& scoreboardId);

    GMLIB_NDAPI std::optional<int> getScore(std::string const& objective, std::string const& name);

    GMLIB_NDAPI std::optional<int> getScore(std::string const& objective, Player& pl);

    GMLIB_NDAPI std::optional<int> getScore(std::string const& objective, Actor& ac);

    GMLIB_NDAPI std::optional<int>
                getScore(std::string const& objective, ActorUniqueID const& uniqueId, bool isPlayer = false);

    GMLIB_NDAPI std::optional<int> getPlayerScore(std::string const& objective, std::string const& serverId);

    GMLIB_NDAPI std::optional<int> getPlayerScore(std::string const& objective, mce::UUID const& uuid);

    GMLIB_NDAPI std::optional<int> getPlayerScore(std::string const& objective, ActorUniqueID const& uniqueId);

    GMLIB_NDAPI std::optional<int> getPlayerScore(std::string const& objective, Player& pl);

    GMLIB_API std::optional<int> setScore(
        Objective&             objective,
        ScoreboardId const&    scoreboardId,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API std::optional<int> setScore(
        std::string const&     objective,
        std::string const&     name,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API std::optional<int> setScore(
        std::string const&     objective,
        Player&                pl,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API std::optional<int> setScore(
        std::string const&     objective,
        Actor&                 ac,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API std::optional<int> setScore(
        std::string const&     objective,
        ActorUniqueID const&   uniqueId,
        int                    value,
        PlayerScoreSetFunction action   = PlayerScoreSetFunction::Set,
        bool                   isPlayer = false
    );

    GMLIB_API std::optional<int> setPlayerScore(
        std::string const&     objective,
        std::string const&     serverId,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API std::optional<int> setPlayerScore(
        std::string const&     objective,
        mce::UUID const&       uuid,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API std::optional<int> setPlayerScore(
        std::string const&     objective,
        ActorUniqueID const&   uniqueId,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API std::optional<int> setPlayerScore(
        std::string const&     objective,
        Player&                pl,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API bool resetScore(Objective& objective, ScoreboardId const& scoreboardId);

    GMLIB_API bool resetScore(std::string const& objective, std::string const& name);

    GMLIB_API bool resetScore(std::string const& objective, Player& pl);

    GMLIB_API bool resetScore(std::string const& objective, Actor& ac);

    GMLIB_API bool resetScore(std::string const& objective, ActorUniqueID const& uniqueId, bool isPlayer = false);

    GMLIB_API bool resetPlayerScore(std::string const& objective, std::string const& serverId);

    GMLIB_API bool resetPlayerScore(std::string const& objective, mce::UUID const& uuid);

    GMLIB_API bool resetPlayerScore(std::string const& objective, ActorUniqueID const& uniqueId);

    GMLIB_API bool resetPlayerScore(std::string const& objective, Player& pl);

    GMLIB_API bool resetScore(ScoreboardId const& scoreboardId);

    GMLIB_API bool resetScore(std::string const& name);

    GMLIB_API bool resetScore(Player& pl);

    GMLIB_API bool resetScore(Actor& ac);

    GMLIB_API bool resetScore(ActorUniqueID const& uniqueId, bool isPlayer = false);

    GMLIB_API bool resetPlayerScore(std::string const& serverId);

    GMLIB_API bool resetPlayerScore(mce::UUID const& uuid);

    GMLIB_API bool resetPlayerScore(Player& pl);

    GMLIB_API bool resetPlayerScore(ActorUniqueID const& uniqueId);

    GMLIB_NDAPI std::vector<ScoreboardId> getAllScoreboardIds();

    GMLIB_NDAPI std::vector<ScoreboardId> getObjectiveTrackedScoreboardIds(Objective& objective);

    GMLIB_NDAPI std::vector<ScoreboardId> getAllScoreboardIds(IdentityDefinition::Type type);

    GMLIB_NDAPI std::vector<ScoreboardId>
                getObjectiveTrackedScoreboardIds(Objective& objective, IdentityDefinition::Type type);

    GMLIB_NDAPI std::vector<ActorUniqueID> getAllPlayers();

    GMLIB_NDAPI std::vector<std::string> getAllPlayerServerIds();

    GMLIB_NDAPI std::vector<mce::UUID> getAllPlayerUuids();

    GMLIB_NDAPI std::vector<ActorUniqueID> getAllEntities();

    GMLIB_NDAPI std::vector<std::string> getAllFakePlayers();

    GMLIB_API void setObjectiveDisplay(
        Objective&         objective,
        std::string const& displaySlot = "sidebar",
        ObjectiveSortOrder order       = ObjectiveSortOrder::Ascending
    );

    GMLIB_API void setObjectiveDisplay(
        std::string const& objective,
        std::string const& displaySlot = "sidebar",
        ObjectiveSortOrder order       = ObjectiveSortOrder::Ascending
    );

    GMLIB_API void clearObjectiveDisplay(std::string const& displaySlot);

    GMLIB_NDAPI std::optional<std::string> getObjectiveDisplayName(std::string const& objective);

    GMLIB_API bool setObjectiveDisplayName(std::string const& objective, std::string const& newName);
};